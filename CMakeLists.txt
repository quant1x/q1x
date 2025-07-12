cmake_minimum_required(VERSION 3.30)
set(CMAKE_VERBOSE_MAKEFILE ON)
# 加载基础 CMake 配置
include(cmake/base.cmake)

# ============================================================
# 检测 项目git仓库当前分支的最新tag, 用tag作为版本号
# ============================================================
# 通过git获取版本信息
include(cmake/git_tag_info.cmake)
get_latest_git_tag_info(VERSION_FROM_GIT LATEST_TAG TAG_COMMIT_HASH GIT_AUTHOR_NAME GIT_AUTHOR_EMAIL GIT_AUTHOR_DATE)

project(quant1x-q1x 
        VERSION ${VERSION_FROM_GIT}
        LANGUAGES C CXX)

# 6. 编译器选项
include(cmake/compile_options.cmake)
message(STATUS "                         C++ flags debug : ${CMAKE_CXX_FLAGS_DEBUG}")
# ============================================================
# vcpkg Options/Dependencies
# ============================================================
add_library(third_libs INTERFACE)
# 可选：显式链接静态版 pthread
find_package(Threads REQUIRED)
target_link_libraries(third_libs INTERFACE Threads::Threads)
if (MINGW OR GNU)
    target_link_libraries(third_libs INTERFACE -static)
    # 静态链接所有库（包括 libwinpthread-1.dll）
    set(CMAKE_EXE_LINKER_FLAGS "-static -lpthread")
    set(CMAKE_SHARED_LINKER_FLAGS "-static -lpthread")
    set(CMAKE_MODULE_LINKER_FLAGS "-static -lpthread")
endif ()

message(STATUS "detect VCPKG")
# 通过环境变量获取全局路径
if (DEFINED ENV{VCPKG_ROOT})
    file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" VCPKG_ROOT)
    string(REGEX REPLACE "/$" "" VCPKG_ROOT "${VCPKG_ROOT}")
else ()
    message(FATAL_ERROR "请设置 VCPKG_ROOT 环境变量指向全局 vcpkg 目录")
endif ()

set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
MESSAGE(STATUS "CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")

# 修正VCPKG的installed目录
if(MSVC OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(VCPKG_INSTALLED_DIR "${PROJECT_SOURCE_DIR}/vcpkg_installed")
else()
    set(VCPKG_INSTALLED_DIR "${VCPKG_ROOT}/installed")
endif()

# 在 Triplet 文件中添加
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_CRT_LINKAGE static)
#if (MSVC)
#    message(STATUS "        MSVC_VERSION = ${MSVC_VERSION}")
#    message(STATUS "MSVC_TOOLSET_VERSION = ${MSVC_TOOLSET_VERSION}")
#endif ()
# 根据平台自动选择 Triplet
if (WIN32)
    message("triplet windows...")
    if (MSVC OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "Default Triplet for Windows")
    else ()
        set(VCPKG_TARGET_TRIPLET "x64-mingw-static" CACHE STRING "Default Triplet for Windows")
    endif ()
elseif (UNIX AND NOT APPLE)
    set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "Default Triplet for Linux")
elseif (APPLE)
    set(VCPKG_TARGET_TRIPLET "x64-osx" CACHE STRING "Default Triplet for macOS")
else ()
    message(status "other...")
endif ()
MESSAGE(STATUS "VCPKG_TARGET_TRIPLET = ${VCPKG_TARGET_TRIPLET}")

# 设置cmake搜索路径
set(VCPKG_LIBS_PATH "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}")
message(STATUS "VCPKG_LIBS_PATH = ${VCPKG_LIBS_PATH}")
set(CMAKE_PREFIX_PATH "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}")
MESSAGE(STATUS "CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")
include_directories(${CMAKE_PREFIX_PATH}/include)
link_directories(${CMAKE_PREFIX_PATH}/lib)
set(VCPKG_INCLUDE_DIRS ${VCPKG_LIBS_PATH}/include)
set(VCPKG_LIBRARY_DIRS ${VCPKG_LIBS_PATH}/lib)

if (DEFINED ENV{MSF_RUNTIME})
    file(TO_CMAKE_PATH "$ENV{MSF_RUNTIME}" RUNTIME)
    string(REGEX REPLACE "/$" "" RUNTIME "${RUNTIME}")
else ()
    message(FATAL_ERROR "NOT found ENV MSF_RUNTIME")
endif ()
MESSAGE(STATUS "RUNTIME = ${RUNTIME}")

# 项目内聚合的第三方库
set(inner_third_party_dir "${CMAKE_SOURCE_DIR}/third_party")
set(INNER_THIRD_PARTY_INCLUDE_DIRS ${inner_third_party_dir}/include)
set(INNER_THIRD_PARTY_LIBRARY_DIRS ${inner_third_party_dir}/lib)
if(WIN32)
    link_directories(${CMAKE_PREFIX_PATH}/lib ${INNER_THIRD_PARTY_LIBRARY_DIRS})
else ()
    link_directories(${CMAKE_PREFIX_PATH}/lib ${RUNTIME}/lib)
endif ()

message(STATUS "================== Crash Report Integration ==================")
#include(cmake/crash.cmake)
include(cmake/backward.cmake)

# check if compiler is nvcc or nvcc_wrapper
set(COMPILER_IS_NVCC false)
get_filename_component(COMPILER_NAME ${CMAKE_CXX_COMPILER} NAME)
if (COMPILER_NAME MATCHES "^nvcc")
    set(COMPILER_IS_NVCC true)
endif()

if (DEFINED ENV{OMPI_CXX} OR DEFINED ENV{MPICH_CXX})
    if ( ($ENV{OMPI_CXX} MATCHES "nvcc") OR ($ENV{MPICH_CXX} MATCHES "nvcc") )
        set(COMPILER_IS_NVCC true)
    endif()
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    if (NOT ${COMPILER_IS_NVCC})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

#add_library(backward_interface INTERFACE)
#set_target_properties(backward_interface PROPERTIES EXPORT_NAME Interface)
target_compile_definitions(third_libs INTERFACE ${BACKWARD_DEFINITIONS})
target_include_directories(third_libs INTERFACE ${BACKWARD_INCLUDE_DIRS})
if(BACKWARD_HAS_EXTERNAL_LIBRARIES)
    target_link_libraries(third_libs INTERFACE ${BACKWARD_LIBRARIES})
endif()
message(STATUS "                         C++ flags debug : ${CMAKE_CXX_FLAGS_DEBUG}")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Xclang -gcodeview -D_DEBUG -D_DLL -D_MT -Xclang --dependent-lib=msvcrtd")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Xclang -gcodeview -D_DEBUG -D_MT -Xclang --dependent-lib=msvcrtd")
message(STATUS "================== 基础功能库 ==================")
# OpenSSL
include(cmake/openssl.cmake)

# zlib
find_package(ZLIB REQUIRED)
target_link_libraries(third_libs INTERFACE ZLIB::ZLIB)
echo_lib_version(ZLIB ${ZLIB_VERSION})

# libiconv
include(cmake/iconv.cmake)

# argparse, argparse::argparse, header-only方式库, 收敛到项目内
#find_package(argparse CONFIG REQUIRED)
set(argparse_VERSION "3.2.0")
target_include_directories(third_libs INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/include>
    $<INSTALL_INTERFACE:include>
)
echo_lib_version(argparse ${argparse_VERSION})

# yaml-cpp yaml-cpp::yaml-cpp
find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(third_libs INTERFACE yaml-cpp::yaml-cpp)
echo_lib_version(yaml-cpp ${yaml-cpp_VERSION})

# tsl::robin_map(robin-map)
#find_package(tsl-robin-map CONFIG REQUIRED)
#target_link_libraries(main PRIVATE tsl::robin_map)
set(tsl-robin-map_VERSION "1.4.0")
echo_lib_version(tsl-robin-map ${tsl-robin-map_VERSION})

# croncpp
#find_package(croncpp CONFIG REQUIRED)
set(croncpp_VERSION "2023-03-30")
echo_lib_version(croncpp ${croncpp_VERSION})

# ==============================
# BS::thread_pool(bshoshany-thread-pool)
# find_path(BSHOSHANY_THREAD_POOL_INCLUDE_DIRS "BS_thread_pool.hpp")
# ==============================
include(cmake/threadpool.cmake)

#simd
include(cmake/simd.cmake)

## ==============================
## DataFrame
## find_package(Protobuf REQUIRED)
## ==============================
#find_package(DataFrame CONFIG REQUIRED)
#echo_lib_version(DataFrame ${DataFrame_VERSION})
#target_link_libraries(third_libs INTERFACE DataFrame::DataFrame)

message(STATUS "================== 编解码工具库 ==================")
## RapidJSON
#find_package(RapidJSON CONFIG REQUIRED)

# ==============================
# protobuf 3.21.11 固定版本, 3.21.12以上版本带有abseil库, 放弃高版本
# find_package(Protobuf REQUIRED)
# ==============================
#find_package(Protobuf REQUIRED)
# 显示版本信息
if (DEFINED Protobuf_VERSION)
    message(STATUS "Protobuf version: ${Protobuf_VERSION}")
else ()
    if (DEFINED CACHE{PROTOBUF_INCLUDE_DIR})
        message(WARNING "Cache exists: PROTOBUF_INCLUDE_DIR=${PROTOBUF_INCLUDE_DIR}")
        unset(PROTOBUF_INCLUDE_DIR CACHE)
    endif ()
    message(WARNING "Protobuf version not found. Falling back to header parsing.")
    find_path(PROTOBUF_INCLUDE_DIR google/protobuf/port_def.inc PATHS "${RUNTIME}/include" NO_DEFAULT_PATH)
    message(STATUS "PROTOBUF_INCLUDE_DIR=${PROTOBUF_INCLUDE_DIR}")
    if (PROTOBUF_INCLUDE_DIR)
        file(STRINGS "${PROTOBUF_INCLUDE_DIR}/google/protobuf/port_def.inc" PROTOBUF_VERSION_LINE REGEX "#define PROTOBUF_VERSION")
        if (PROTOBUF_VERSION_LINE)
            string(REGEX MATCH "[0-9]+" GOOGLE_PROTOBUF_VERSION "${PROTOBUF_VERSION_LINE}")
            math(EXPR PROTOBUF_MAJOR_VERSION "${GOOGLE_PROTOBUF_VERSION} / 1000000")
            math(EXPR PROTOBUF_MINOR_VERSION "(${GOOGLE_PROTOBUF_VERSION} / 1000) % 1000")
            math(EXPR PROTOBUF_PATCH_VERSION "${GOOGLE_PROTOBUF_VERSION} % 1000")
            set(PROTOBUF_VERSION_FULL "${PROTOBUF_MAJOR_VERSION}.${PROTOBUF_MINOR_VERSION}.${PROTOBUF_PATCH_VERSION}")
            message(STATUS "Protobuf version (from header): ${PROTOBUF_VERSION_FULL}")
            set(Protobuf_VERSION ${PROTOBUF_VERSION_FULL})
        else ()
            message(WARNING "Could not extract Protobuf version from header file.")
        endif ()
    else ()
        message(WARNING "Protobuf headers not found.")
    endif ()
endif ()

if (MSVC)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        # Debug模式下链接
        set(PROTOBUF_LIB_NAME "libprotobufd")
    else ()
        # Release模式下链接
        set(PROTOBUF_LIB_NAME "libprotobuf")
    endif ()
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
else ()
    set(PROTOBUF_LIB_NAME "protobuf")
endif ()

target_link_libraries(third_libs INTERFACE ${PROTOBUF_LIB_NAME})
echo_lib_version(protobuf ${Protobuf_VERSION})

# ==============================
# CapnProto
# find_package(CapnProto CONFIG REQUIRED)
# ==============================
# 查找 Cap'n Proto 包（需要 pkg-config 或 find_package）
find_package(CapnProto CONFIG REQUIRED)
#include_directories(${CAPNPROTO_INCLUDE_DIRS})
target_link_libraries(third_libs INTERFACE CapnProto::capnp CapnProto::capnp-json)
echo_lib_version(CapnProto ${CapnProto_VERSION})
if (MSVC)
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_definitions(KJ_USE_FIBERS=0)
    endif ()
endif ()

# ==============================
# cista
# find_package(cista CONFIG REQUIRED)
# ==============================
# Cista
#find_package(cista CONFIG REQUIRED)
set(CISTA_VERSION "0.15") # 无法获取, 手动设定
echo_lib_version(cista ${CISTA_VERSION})

message(STATUS "================== 底层 工具库 ==================")
# ASIO
include(cmake/network.cmake)

## mimalloc
#find_package(mimalloc CONFIG REQUIRED)
#target_link_libraries(third_libs INTERFACE $<IF:$<TARGET_EXISTS:mimalloc-static>,mimalloc-static,mimalloc>)
#target_compile_definitions(third_libs INTERFACE
#    #MI_OVERRIDE=1
#    MI_OVERRIDE_NEW_DELETE=1  # 让mimalloc只重载operator new/delete
#    MI_INTERPOSE=0            # 禁用拦截标准库函数
#)
#echo_lib_version(mimalloc ${mimalloc_VERSION})

# spdlog
include(cmake/spdlog.cmake)

message(STATUS "================== JavaScript工具库 ==================")
## MuJS
#find_package(PkgConfig)
#pkg_check_modules(MUJS REQUIRED IMPORTED_TARGET mujs)
# duktape
find_package(unofficial-duktape CONFIG REQUIRED)
target_link_libraries(third_libs INTERFACE unofficial::duktape::duktape)
echo_lib_version(duktape ${unofficial-duktape_VERSION})

message(STATUS "================== 测试工具库 ==================")
# 查找 Google Benchmark 包
find_package(benchmark REQUIRED)
add_library(test-benchmark INTERFACE)
target_link_libraries(test-benchmark INTERFACE benchmark::benchmark benchmark::benchmark_main)
echo_lib_version(benchmark ${benchmark_VERSION})
# Catch2
find_package(Catch2 CONFIG REQUIRED)
echo_lib_version(Catch2 ${Catch2_VERSION})
add_library(test-catch2 INTERFACE)
target_link_libraries(test-catch2 INTERFACE Catch2::Catch2 Catch2::Catch2WithMain)
# 启用Catch2 Unicode支持
target_compile_definitions(third_libs INTERFACE
    CATCH_CONFIG_WINDOWS_SEH
    CATCH_CONFIG_USE_ANSI_COLOR_CODES
)
# GTest
find_package(GTest CONFIG REQUIRED)
add_library(test-gtest INTERFACE)
target_link_libraries(test-gtest INTERFACE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
echo_lib_version(GTest ${GTest_VERSION})

# ============================================================
# Language Standards Detection and Configuration
# ============================================================
# 设置构建信息变量
set(BUILD_TYPE "${CMAKE_BUILD_TYPE}")

# C 标准相关
set(C_STANDARD "${CMAKE_C_STANDARD}")
set(C_STANDARD_REQUIRED "${CMAKE_C_STANDARD_REQUIRED}")
set(C_EXTENSIONS "${CMAKE_C_EXTENSIONS}")

set(C_FLAGS "${CMAKE_C_FLAGS}")
set(C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
set(C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")

# C++ 标准相关
set(CXX_STANDARD "${CMAKE_CXX_STANDARD}")
set(CXX_STANDARD_REQUIRED "${CMAKE_CXX_STANDARD_REQUIRED}")
set(CXX_EXTENSIONS "${CMAKE_CXX_EXTENSIONS}")

set(CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

# 链接器参数
set(EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

# 编译器信息
set(C_COMPILER_ID "${CMAKE_C_COMPILER_ID}")
set(C_COMPILER_VERSION "${CMAKE_C_COMPILER_VERSION}")
set(C_COMPILER_PATH "${CMAKE_C_COMPILER}")

set(CXX_COMPILER_ID "${CMAKE_CXX_COMPILER_ID}")
set(CXX_COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
set(CXX_COMPILER_PATH "${CMAKE_CXX_COMPILER}")

# CUDA 参数（如果启用）
if (ENABLE_CUDA)
    set(CUDA_FLAGS_DEBUG "${CMAKE_CUDA_FLAGS_DEBUG}")
    set(CUDA_FLAGS_RELEASE "${CMAKE_CUDA_FLAGS_RELEASE}")
else ()
    set(CUDA_FLAGS_DEBUG "N/A")
    set(CUDA_FLAGS_RELEASE "N/A")
endif ()

# 第三方库版本定义
set(DEPENDENCY_OPENSSL "v${OPENSSL_VERSION}")
set(DEPENDENCY_ASIO "v${ASIO_VERSION_FULL}")
set(DEPENDENCY_ICONV "v${Iconv_VERSION}")
set(DEPENDENCY_FMT "v${fmt_VERSION}")
set(DEPENDENCY_ZLIB "v${ZLIB_VERSION}")
set(DEPENDENCY_XSIMD "v${xsimd_VERSION}")
set(DEPENDENCY_BSHOSHANY_THREAD_POOL "v${BSHOSHANY_THREAD_POOL_VERSION}")
set(DEPENDENCY_CRONCPP "v${croncpp_VERSION}")
set(DEPENDENCY_TSL_ROBIN_MAP "v${tsl-robin-map_VERSION}")
set(DEPENDENCY_ARGPARSE "v${argparse_VERSION}")
#set(DEPENDENCY_RAPIDJSON "v${RapidJSON_VERSION}")
set(DEPENDENCY_YAML_CPP "v${yaml-cpp_VERSION}")
set(DEPENDENCY_PROTOBUF "v${Protobuf_VERSION}")
set(DEPENDENCY_CAPNP "v${CapnProto_VERSION}")
set(DEPENDENCY_CISTA "v${CISTA_VERSION}")
set(DEPENDENCY_MIMALLOC "v${mimalloc_VERSION}")
set(DEPENDENCY_SPDLOG "v${spdlog_VERSION}")
set(DEPENDENCY_CPR "v${cpr_VERSION}")
set(DEPENDENCY_DUKTAPE "v${unofficial-duktape_VERSION}")
set(DEPENDENCY_BENCHMARK "v${benchmark_VERSION}")
set(DEPENDENCY_CATCH2 "v${Catch2_VERSION}")

# ============================================================
# 量化基础设施
# ============================================================

set(q1x-headers
    include/q1x/std/api.h
    include/q1x/std/base.h
    include/q1x/std/buffer.h
    include/q1x/std/strings.h
    include/q1x/std/util.h
    include/q1x/std/format.h
    include/q1x/std/feature_detection.h
    include/q1x/std/numerics.h
    include/q1x/std/safe.h

    # i/o 相关
    include/q1x/io/csv-reader.h
    include/q1x/io/csv-writer.h
    include/q1x/io/file.h
    include/q1x/io/http.h

    # 编码相关
    include/q1x/encoding/iconv.h
    include/q1x/encoding/csv.h
    include/q1x/encoding/json.h
    include/q1x/encoding/yaml.h

    # 运行时
    include/q1x/std/affinity.h
    include/q1x/runtime/crash.h
    include/q1x/runtime/service.h
    include/q1x/runtime/cache1d.h
    include/q1x/runtime/once.h
    include/q1x/runtime/core.h
    include/q1x/std/scheduler.h
    include/q1x/ta/simd.h

    # 配置
    include/q1x/config/rule-parameter.h
    include/q1x/config/strategy-parameter.h
    include/q1x/config/trader-parameter.h
    include/q1x/config/trading-session.h

    # 业务引擎
    include/q1x/engine/action.h

    # 网络相关
    include/q1x/net/base.h
    include/q1x/net/connection_pool.h
    include/q1x/net/endpoint.h
    include/q1x/net/operation_handler.h

    # 数据帧, 列式存储
    include/q1x/dataframe/dataframe.h
    include/q1x/dataframe/periods.h
    include/q1x/dataframe/series.h

    # 数据协议
    include/q1x/proto/data.h
    include/q1x/proto/data.pb.h
    include/q1x/proto/data.pb.cc
    include/q1x/proto/kline.pb.h
    include/q1x/proto/kline.pb.cc
    include/q1x/proto/snapshot.capnp.h
    include/q1x/proto/snapshot.capnp.c++
    include/q1x/proto/xdxr.pb.h
    include/q1x/proto/xdxr.pb.cc

    # 技术分析
    include/q1x/ta/boll.h
    include/q1x/ta/ta.h
    include/q1x/ta/ma.h
    include/q1x/ta/rsi.h
    include/q1x/ta/macd.h
    include/q1x/ta/type_default.h
    include/q1x/ta/rsi.h
    include/q1x/ta/rolling.h
    include/q1x/ta/type_default.h
    include/q1x/ta/sma.h

    # 数据集
    include/q1x/datasets/chips.h
    include/q1x/datasets/kline_raw.h
    include/q1x/datasets/kline.h
    include/q1x/datasets/trans.h
    include/q1x/datasets/minute.h
    include/q1x/datasets/xdxr.h
    include/q1x/datasets/xdxr_adjust_factor.h

    # 实时模块
    include/q1x/realtime/snapshot.h

    # 回测
    include/q1x/backtest/backtest.h
    include/q1x/backtest/order.h
    include/q1x/backtest/trade.h
    include/q1x/backtest/position.h

    # 交易所相关
    include/q1x/exchange/blocks.h
    include/q1x/exchange/calendar.h
    include/q1x/exchange/code.h
    include/q1x/exchange/margin-trading.h
    include/q1x/exchange/session.h
    include/q1x/exchange/timestamp.h

    # level1
    include/q1x/level1/minute_time.h
    include/q1x/level1/client.h
    include/q1x/level1/protocol.h

    # 策略
    include/q1x/engine/strategy.h
    include/q1x/engine/rule-engine.h
    include/q1x/engine/rule-error.h
    include/q1x/engine/rule-context.h

    # 插件
    include/q1x/models/plugin-loader.h
    include/q1x/models/plugin-api.h

    # 因子
    include/q1x/factors/f10.h
    include/q1x/factors/share-holder.h
    include/q1x/factors/base.h
    include/q1x/factors/notice.h
    include/q1x/factors/financial_report.h
    include/q1x/factors/safety-score.h
    include/q1x/factors/history.h

    # 交易柜台
    include/q1x/trader/fee.h
    include/q1x/trader/order.h
    include/q1x/trader/constants.h
    include/q1x/trader/account.h
    include/q1x/trader/trader.h
    include/q1x/trader/holding.h
    include/q1x/trader/order_cache.h
    include/q1x/trader/position.h
    include/q1x/trader/tracker.h
    include/q1x/trader/order_state.h

    # 内嵌资源文件
    include/q1x/resources/meta/blocks.h
    include/q1x/quant1x.h
)

set(q1x-sources
    # 标准库兼容
    src/q1x/std/strings.cpp
    src/q1x/std/time.cpp
    src/q1x/std/util.cpp
    src/q1x/std/numerics.cpp
    src/q1x/std/safe.cpp

    # 运行时
    src/q1x/std/affinity.cpp
    src/q1x/runtime/core.cpp
    src/q1x/runtime/crash.cpp
    src/q1x/std/scheduler.cpp

    # i/o 相关
    src/q1x/io/file.cpp
    src/q1x/io/http.cpp

    # 编码相关
    src/q1x/encoding/iconv.cpp
    src/q1x/encoding/json.cpp

    # 引擎
    src/q1x/engine/adapter.cpp

    # level1
    src/q1x/level1/client.cpp
    src/q1x/level1/protocol.cpp

    # 配置信息
    src/q1x/config/config.cpp
    src/q1x/config/detail/trading-session.cpp
    src/q1x/config/detail/trader-parameter.cpp
    src/q1x/config/detail/strategy-parameter.cpp

    src/q1x/cache.cpp
    src/q1x/command.cpp
    src/q1x/formula.cpp
    src/q1x/exchange.cpp

    # 回测
    src/q1x/backtest/backtest.cpp
    src/q1x/backtest/position.cpp
    src/q1x/backtest/engine.cpp
    src/q1x/backtest/order.cpp
    src/q1x/backtest/trade.cpp

    # 数据集
    src/q1x/datasets/minute.cpp
    src/q1x/datasets/xdxr_adjust_factor.cpp
    src/q1x/datasets/xdxr.cpp
    src/q1x/datasets/trans.cpp
    src/q1x/datasets/klines.cpp
    src/q1x/datasets/kline_raw.cpp
    src/q1x/datasets.cpp

    # 交易所相关
    src/q1x/exchange/blocks.cpp
    src/q1x/exchange/security.cpp
    src/q1x/exchange/code.cpp
    src/q1x/exchange/calendar.cpp
    src/q1x/exchange/session.cpp
    src/q1x/exchange/timestamp.cpp
    src/q1x/exchange/margin-trading.cpp

    # 因子
    src/q1x/factors/f10.cpp
    src/q1x/factors/share-holder.cpp
    src/q1x/factors/base.cpp
    src/q1x/factors/history.cpp
    src/q1x/factors/notice.cpp
    src/q1x/factors/financial_report.cpp
    src/q1x/factors/safety-score.cpp

    # 实时
    src/q1x/realtime/snapshot.cpp

    # 插件
    src/q1x/plugins/plugin-loader.cpp

    # 策略
    src/q1x/engine/rule-engine.cpp
    src/q1x/engine/strategy.cpp

    # 交易柜台
    src/q1x/trader/fee.cpp
    src/q1x/trader/order.cpp
    src/q1x/trader/account.cpp
    src/q1x/trader/trader.cpp
    src/q1x/trader/holding.cpp
    src/q1x/trader/order_cache.cpp
    src/q1x/trader/position.cpp
    src/q1x/trader/tracker.cpp
    src/q1x/trader/order_state.cpp
    src/q1x/quant1x.cpp
)

if (WIN32)
    list(APPEND q1x-sources src/q1x/runtime/daemon_service.cpp)
elseif (APPLE)
    list(APPEND q1x-sources src/q1x/runtime/daemon_macosx.cpp)
elseif (UNIX)
    # LINUX 是非官方变量，我们手动检测
    set(LINUX FALSE)
    if (CMAKE_SYSTEM_NAME MATCHES "Linux")
        set(LINUX TRUE)
    endif ()

    if (LINUX)
        list(APPEND q1x-sources src/q1x/runtime/daemon_systemd.cpp)
    else ()
        message(WARNING "Unsupported Unix OS: ${CMAKE_SYSTEM_NAME}")
    endif ()
else ()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif ()

## 链接不同平台的库
#if (WIN32)
#    target_link_libraries(third_libs INTERFACE ws2_32)   # Windows 网络库, 标准 Winsock API 支持, TCP/UDP 通信、客户端/服务器, winsock2.h
#    target_link_libraries(third_libs INTERFACE mswsock)  # Windows 网络库, 微软扩展函数（高性能网络）, IOCP、异步网络、高性能服务端, mswsock.h
#    target_link_libraries(third_libs INTERFACE Imagehlp) # Windows Crash Dump, PE 文件结构解析
#    target_link_libraries(third_libs INTERFACE Dbghelp)  # Windows Crash Dump, 堆栈回溯（Stack Walking）
#    target_link_libraries(third_libs INTERFACE advapi32) # Windows 服务管理
#    target_link_libraries(third_libs INTERFACE psapi)
#    if (MINGW)
#        #find_package(libdwarf CONFIG REQUIRED)
#        #target_link_libraries(third_libs INTERFACE libdwarf::dwarf)
#        target_link_libraries(third_libs INTERFACE bfd)
#        target_compile_definitions(third_libs INTERFACE BACKWARD_HAS_BFD=1)
#    endif ()
#elseif (LINUX)
#    # vcpkg版本的libdwarf未实验成功
#    #target_link_libraries(third_libs PUBLIC libdwarf::dwarf)
#    # #define BACKWARD_HAS_DW 1
#    target_link_libraries(third_libs INTERFACE dw bfd) # apt-get install libdw-dev
#    # #define BACKWARD_HAS_DWARF 1
#    #target_link_libraries(third_libs INTERFACE elf dwarf) # apt-get install libdwarf-dev
#elseif (APPLE)
#    target_link_libraries(third_libs INTERFACE dwarf) # apt-get install libdw-dev
#endif ()

#get_target_property(_libs third_libs INTERFACE_LINK_LIBRARIES)
#message(STATUS "Library 'third_libs' requires: ${_libs}")

# 检查编译器选项
get_target_property(_includes third_libs INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(_libs     third_libs INTERFACE_LINK_LIBRARIES)
get_target_property(_defines  third_libs INTERFACE_COMPILE_DEFINITIONS)
get_target_property(_options  third_libs INTERFACE_COMPILE_OPTIONS)

message(STATUS "third_libs Includes: ${_includes}")
message(STATUS "third_libs     Libs: ${_libs}")
message(STATUS "third_libs  Defines: ${_defines}")
message(STATUS "third_libs  Options: ${_options}")

add_library(q1x STATIC
    ${q1x-headers}
    ${q1x-sources}
)
target_include_directories(q1x
    SYSTEM        # 关键选项：标记为系统头文件路径
    PUBLIC        # 仅当前目标可见（或 PUBLIC/INTERFACE）
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third_party/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(q1x PUBLIC
    third_libs global_compile_options
)

get_target_property(_libs q1x INTERFACE_LINK_LIBRARIES)
message(STATUS "Library 'q1x' requires: ${_libs}")

MESSAGE(STATUS "RUNTIME = ${RUNTIME}")
set(CMAKE_INSTALL_PREFIX ${RUNTIME})

STRING(TOUPPER "${CMAKE_BUILD_TYPE}" buildtype)
message(STATUS "")
message(STATUS "")
message(STATUS "quant1x configuration summary:")
message(STATUS "")
message(STATUS "                              Build type : ${CMAKE_BUILD_TYPE}")
message(STATUS "                        Operating System : ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "                            Build System : ${CMAKE_SYSTEM}")
message(STATUS "                  Processor Architecture : ${CMAKE_SYSTEM_PROCESSOR}")
# C 编译选项
message(STATUS "------------------------------ C Standard Configuration ------------------------------")
message(STATUS "                              C_STANDARD : ${CMAKE_C_STANDARD}")
message(STATUS "                     C_STANDARD_REQUIRED : ${CMAKE_C_STANDARD_REQUIRED}")
message(STATUS "                            C_EXTENSIONS : ${CMAKE_C_EXTENSIONS}")
message(STATUS "                         C flags(CFLAGS) : ${CMAKE_C_FLAGS}")
message(STATUS "                           C flags debug : ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "                         C flags release : ${CMAKE_C_FLAGS_RELEASE}")
# C++ 编译选项
message(STATUS "------------------------------ C++ Standard Configuration ------------------------------")
message(STATUS "                            CXX_STANDARD : ${CMAKE_CXX_STANDARD}")
message(STATUS "                   CXX_STANDARD_REQUIRED : ${CMAKE_CXX_STANDARD_REQUIRED}")
message(STATUS "                          CXX_EXTENSIONS : ${CMAKE_CXX_EXTENSIONS}")
message(STATUS "                       C++ flags(CFLAGS) : ${CMAKE_CXX_FLAGS}")
message(STATUS "                         C++ flags debug : ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "                       C++ flags release : ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "------------------------------ C++ linker ------------------------------")
message(STATUS "                  CMAKE_EXE_LINKER_FLAGS : ${CMAKE_EXE_LINKER_FLAGS}")
# 输出当前编译器信息
message(STATUS "------------------------------ C compiler ------------------------------")
message(STATUS "                           C compiler ID : ${CMAKE_C_COMPILER_ID}")
message(STATUS "                      C compiler version : ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "                         C compiler path : ${CMAKE_C_COMPILER}")
message(STATUS "------------------------------ C++ compiler ------------------------------")
message(STATUS "                         C++ compiler ID : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "                    C++ compiler version : ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "                       C++ compiler path : ${CMAKE_CXX_COMPILER}")
message(STATUS "------------------------------ CUDA compiler ------------------------------")
message(STATUS "                  CMAKE_CUDA_FLAGS_DEBUG : ${CMAKE_CUDA_FLAGS_DEBUG}")
message(STATUS "                CMAKE_CUDA_FLAGS_RELEASE : ${CMAKE_CUDA_FLAGS_RELEASE}")
message(STATUS "")
message(STATUS "")

message(STATUS "   Build type .......................... : ${CMAKE_BUILD_TYPE}")
message(STATUS "   Install .pdb (if available).......... : ${INSTALL_PDB}")
message(STATUS "   Install prefix ...................... : ${CMAKE_INSTALL_PREFIX}")
message(STATUS "     Directory for binary files ........ : PREFIX/${QUANT1X_INSTALL_BIN_DIR}")
message(STATUS "     Directory for library files ....... : PREFIX/${QUANT1X_INSTALL_LIB_DIR}")
message(STATUS "     Directory for include files ....... : PREFIX/${QUANT1X_INSTALL_INCLUDE_DIR}")
message(STATUS "   C compiler .......................... : ${CMAKE_C_COMPILER}")
message(STATUS "   CXX compiler ........................ : ${CMAKE_CXX_COMPILER}")
message(STATUS "")
message(STATUS "                                 OpenSSL : v${OPENSSL_VERSION}")
message(STATUS "                                    asio : v${ASIO_VERSION_FULL}")
message(STATUS "                         Iconv(libiconv) : v${Iconv_VERSION}")
message(STATUS "                                     fmt : v${fmt_VERSION}")
message(STATUS "                                    zlib : v${ZLIB_VERSION}")
message(STATUS "                                    inja : v${inja_VERSION}")
message(STATUS "                           nlohmann_json : v${nlohmann_json_VERSION}")
message(STATUS "                                     xtl : v${xtl_VERSION}")
message(STATUS "                                   xsimd : v${xsimd_VERSION}")
message(STATUS "                                 xtensor : v${xtensor_VERSION}")
message(STATUS "  BS::thread_pool(bshoshany-thread-pool) : v${BSHOSHANY_THREAD_POOL_VERSION}")
message(STATUS "                                 croncpp : v${croncpp_VERSION}")
message(STATUS "            tsl::robin_map(robin-map)(√) : v${tsl-robin-map_VERSION}")
message(STATUS "                                argparse : v${argparse_VERSION}")
#message(STATUS "                               RapidJSON : v${RapidJSON_VERSION}")
message(STATUS "                                yaml-cpp : v${yaml-cpp_VERSION}")
message(STATUS "                                Protobuf : v${Protobuf_VERSION}")
message(STATUS "                             Cap'n Proto : v${CapnProto_VERSION}")
message(STATUS "                                Cista(√) : v${CISTA_VERSION}")
message(STATUS "                                mimalloc : v${mimalloc_VERSION}")
message(STATUS "                                  spdlog : v${spdlog_VERSION}")
message(STATUS "     C++ Requests Library (Cpr for curl) : v${cpr_VERSION}")
message(STATUS "                   unofficial-duktape(√) : v${unofficial-duktape_VERSION}")
message(STATUS "                               benchmark : v${benchmark_VERSION}")
message(STATUS "                                  Catch2 : v${Catch2_VERSION}")
message(STATUS "                                   GTest : v${GTest_VERSION}")

message(STATUS "q1x found: ${q1x_FOUND}")
message(STATUS "q1x version: ${q1x_VERSION}")
message(STATUS "q1x include dirs: ${q1x_INCLUDE_DIRS}")
message(STATUS "q1x library dirs: ${q1x_LIBRARY_DIRS}")
get_target_property(q1x_opts q1x COMPILE_OPTIONS)
get_target_property(q1x_defs q1x COMPILE_DEFINITIONS)
message(STATUS "继承的编译选项: ${q1x_opts}")
message(STATUS "继承的宏定义: ${q1x_defs}")

# 检查编译器选项
get_target_property(_includes global_compile_options INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(_libs     global_compile_options INTERFACE_LINK_LIBRARIES)
get_target_property(_defines  global_compile_options INTERFACE_COMPILE_DEFINITIONS)
get_target_property(_options  global_compile_options INTERFACE_COMPILE_OPTIONS)

message(STATUS "quant1x Includes: ${_includes}")
message(STATUS "quant1x     Libs: ${_libs}")
message(STATUS "quant1x  Defines: ${_defines}")
message(STATUS "quant1x  Options: ${_options}")

function(get_all_linked_targets target result_var)
    set(result "")

    # 获取该目标直接链接的库
    get_target_property(libs ${target} LINK_LIBRARIES)

    foreach(lib IN LISTS libs)
        if(TARGET ${lib})
            list(APPEND result ${lib})
            # 递归处理子依赖
            get_all_linked_targets(${lib} sub_result)
            list(APPEND result ${sub_result})
        endif()
    endforeach()

    list(REMOVE_DUPLICATES result)
    set(${result_var} ${result} PARENT_SCOPE)
endfunction()

# 使用函数
get_all_linked_targets(q1x ALL_LINKED_TARGETS)
message(STATUS "All linked targets (recursive): ${ALL_LINKED_TARGETS}")

# 清除DLL文件的前缀
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_IMPORT_LIBRARY_PREFIX "")
# 为静态库和动态库分别设置目标名称，避免冲突
set_target_properties(q1x PROPERTIES OUTPUT_NAME "q1x" PREFIX "")

# Generate build-info.h from build-info.h.in
configure_file(
    "${CMAKE_SOURCE_DIR}/include/q1x/build-info.h.in"
    "${CMAKE_BINARY_DIR}/include/private/build-info.h"
)

add_executable(q2x
    main.cpp
)
target_include_directories(q2x
    SYSTEM        # 关键选项：标记为系统头文件路径
    PUBLIC       # 仅当前目标可见（或 PUBLIC/INTERFACE）
    ${CMAKE_SOURCE_DIR}/include  # 头文件目录
    ${CMAKE_SOURCE_DIR}/third_party/include  # 头文件目录
    ${CMAKE_BINARY_DIR}/include  # 私有头文件
)

target_link_libraries(q2x PRIVATE q1x)

# 设置安装路径变量
include(GNUInstallDirs)
MESSAGE(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
set(THIRD_PARTY_INSTALL_INCDIR "${CMAKE_INSTALL_INCLUDEDIR}/third_party")

# 设置 Debug 版本后缀, 并标记为高级选项(默认不显示在 CMake GUI 中)
set(lib_suffix "d" CACHE STRING "Default debug postfix")
if (WIN32)
    set(lib_suffix "${lib_suffix}-win")
endif ()
set(q1x_DEBUG_POSTFIX ${lib_suffix})
message(STATUS ${q1x_DEBUG_POSTFIX})

mark_as_advanced(q1x_DEBUG_POSTFIX)
set_target_properties(q1x PROPERTIES
    DEBUG_POSTFIX "${q1x_DEBUG_POSTFIX}"
)
# 将第三方头文件声明为安装目标
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/include/
    DESTINATION ${THIRD_PARTY_INSTALL_INCDIR}
    #FILES_MATCHING
    #PATTERN "*.h"
    #PATTERN "*.hpp"
    #PATTERN "*.inl"
)

# 安装配置（可选）
install(TARGETS q1x third_libs xtensor_optimize global_compile_options EXPORT q1x
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

# 安装主库头文件
install(
    DIRECTORY include/ ${CMAKE_BINARY_DIR}/include/private/
    DESTINATION include/q1x
)

# 安装 CMake 配置文件
install(EXPORT q1x
        FILE q1x.cmake
        NAMESPACE quant1x::
        DESTINATION lib/cmake/q1x)

include(CMakePackageConfigHelpers)

# 生成版本文件
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/q1x-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(EXPORT q1x
    FILE q1x-targets.cmake
    NAMESPACE quant1x::
    DESTINATION lib/cmake/q1x
)

# 安装配置文件
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/q1x-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/q1x-config.cmake"
    INSTALL_DESTINATION lib/cmake/q1x
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/q1x-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/q1x-config-version.cmake"
    DESTINATION lib/cmake/q1x
)


# 单元测试
add_subdirectory(tests)