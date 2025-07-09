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

if (MSVC)
    # 强制 cl.exe 显示命令行（/Bv = 显示调用的完整命令）
    #add_compile_options(/Bv)
endif()
# 6. 编译器选项
include(cmake/compile_options.cmake)

# 查看全局 C 选项
message(STATUS "  CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
# 查看全局 C++ 选项
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

## 在CMake中启用内存检测
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

# 设置标准属性
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
# 禁用 GNU 扩展（关键！）
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 禁用 GNU 扩展（关键！）
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================
# vcpkg Options/Dependencies
# ============================================================
add_library(third_libs INTERFACE)
# 可选：显式链接静态版 pthread
find_package(Threads REQUIRED)
target_link_libraries(third_libs INTERFACE Threads::Threads -lpthread)
if (MINGW OR GNU)
    target_link_libraries(third_libs INTERFACE -static)
    # 静态链接所有库（包括 libwinpthread-1.dll）
    set(CMAKE_EXE_LINKER_FLAGS "-static -lpthread")
    set(CMAKE_SHARED_LINKER_FLAGS "-static -lpthread")
    set(CMAKE_MODULE_LINKER_FLAGS "-static -lpthread")
endif ()

# 通过环境变量获取全局路径
if (DEFINED ENV{VCPKG_ROOT})
    file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" VCPKG_ROOT)
    string(REGEX REPLACE "/$" "" VCPKG_ROOT "${VCPKG_ROOT}")
else ()
    message(FATAL_ERROR "请设置 VCPKG_ROOT 环境变量指向全局 vcpkg 目录")
endif ()

set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
MESSAGE(STATUS "CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")

## 如果是MSVC, 且使用了MSVC内置的vcpkg, 需要改成本地安装的vcpkg
#if(VCPKG_ROOT MATCHES "Visual Studio")
#    message(STATUS "This is a Visual Studio generator")
#    if (DEFINED ENV{RUNTIME_VCPKG})
#        file(TO_CMAKE_PATH "$ENV{RUNTIME_VCPKG}" VCPKG_ROOT)
#        string(REGEX REPLACE "/$" "" VCPKG_ROOT "${VCPKG_ROOT}")
#    endif ()
#endif()

# 修正VCPKG的installed目录
if(MSVC)
    set(VCPKG_INSTALLED_DIR "${PROJECT_SOURCE_DIR}/vcpkg_installed")
else()
    set(VCPKG_INSTALLED_DIR "${VCPKG_ROOT}/installed")
endif()

# 在 Triplet 文件中添加
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_CRT_LINKAGE static)
message(STATUS "        MSVC_VERSION = ${MSVC_VERSION}")
message(STATUS "MSVC_TOOLSET_VERSION = ${MSVC_TOOLSET_VERSION}")
# 根据平台自动选择 Triplet
if (WIN32)
    if (MSVC)
        set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "Default Triplet for Windows")
    else ()
        set(VCPKG_TARGET_TRIPLET "x64-mingw-static" CACHE STRING "Default Triplet for Windows")
    endif ()
elseif (UNIX AND NOT APPLE)
    set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "Default Triplet for Linux")
elseif (APPLE)
    set(VCPKG_TARGET_TRIPLET "x64-osx" CACHE STRING "Default Triplet for macOS")
    #set(VCPKG_TARGET_TRIPLET "x64-mingw-static" CACHE STRING "Default Triplet for macOS")
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

#include_directories(${RUNTIME}/include)
#link_directories(${RUNTIME}/lib)
#set(RUNTIME_INCLUDE_DIRS ${RUNTIME}/include)
#set(RUNTIME_LIBRARY_DIRS ${RUNTIME}/lib)

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
#add_library(CrashReport INTERFACE)
#find_package(dlfcn-win32 CONFIG REQUIRED)

message(STATUS "================== 基础功能库 ==================")
# OpenSSL
find_package(OpenSSL REQUIRED)
# 尝试直接输出版本（如果存在）
if (DEFINED OPENSSL_VERSION)
    message(STATUS "OpenSSL version (predefined): ${OPENSSL_VERSION}")
else ()
    # 如果 OPENSSL_VERSION 未定义，尝试通过命令行工具获取版本
    find_program(OPENSSL_EXECUTABLE openssl)
    if (OPENSSL_EXECUTABLE)
        execute_process(
            COMMAND "${OPENSSL_EXECUTABLE}" version
            OUTPUT_VARIABLE OPENSSL_VERSION_OUTPUT
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        message(STATUS "OpenSSL version (from command line): ${OPENSSL_VERSION_OUTPUT}")
    else ()
        message(WARNING "Could not determine OpenSSL version. Please check your installation.")
    endif ()
endif ()
target_link_libraries(third_libs INTERFACE OpenSSL::SSL OpenSSL::Crypto)
echo_lib_version(OpenSSL ${OPENSSL_VERSION})

# libiconv
find_package(Iconv REQUIRED)  # CMake 官方模块名称是 Iconv（大写 I）
if (NOT DEFINED Iconv_VERSION) # 如果没有定义Iconv_VERSION变量, 去查找iconv.h文件路径
    if (DEFINED CACHE{ICONV_INCLUDE_DIR})
        message(WARNING "Cache exists: ICONV_INCLUDE_DIR=${ICONV_INCLUDE_DIR}")
        unset(ICONV_INCLUDE_DIR CACHE)
    endif ()
    find_path(ICONV_INCLUDE_DIR iconv.h PATHS /usr/local/include /usr/include /usr/local/opt/libiconv/include ${VCPKG_INCLUDE_DIRS} NO_DEFAULT_PATH)
    if (ICONV_INCLUDE_DIR)
        message(WARNING "Iconv exists: ICONV_INCLUDE_DIR=${ICONV_INCLUDE_DIR}")
        # 读取版本宏定义行
        file(STRINGS "${ICONV_INCLUDE_DIR}/iconv.h" ICONV_VERSION_LINE
            REGEX "^#define _LIBICONV_VERSION 0x[0-9A-Fa-f]+")

        # 提取纯十六进制数值部分（去掉注释等）
        string(REGEX MATCH "0x[0-9A-Fa-f]+" ICONV_VERSION_HEX "${ICONV_VERSION_LINE}")

        if (ICONV_VERSION_HEX)
            # 计算主次版本号
            math(EXPR ICONV_VERSION_MAJOR "${ICONV_VERSION_HEX} >> 8" OUTPUT_FORMAT DECIMAL)
            math(EXPR ICONV_VERSION_MINOR "${ICONV_VERSION_HEX} & 0xFF" OUTPUT_FORMAT DECIMAL)

            set(Iconv_VERSION "${ICONV_VERSION_MAJOR}.${ICONV_VERSION_MINOR}")
            message(STATUS "Found libiconv version: ${Iconv_VERSION}")
        else ()
            message(WARNING "Could not extract version number from iconv.h")
        endif ()
    else ()
        message(WARNING "Could not find iconv.h")
    endif ()
endif ()
target_link_libraries(third_libs INTERFACE Iconv::Iconv iconv charset)
echo_lib_version(iconv ${Iconv_VERSION})
if (APPLE OR LINUX)
    # 获取头文件所在的基础目录（通常是 "installed/x64-osx"）
    get_filename_component(ICONV_BASE_DIR "${ICONV_INCLUDE_DIR}" DIRECTORY)

    # 拼接 lib 目录
    set(ICONV_LIB_DIR "${ICONV_BASE_DIR}/lib")

    # 验证 lib 目录是否存在
    if (NOT EXISTS "${ICONV_LIB_DIR}")
        message(FATAL_ERROR "libiconv 的库目录不存在: ${ICONV_LIB_DIR}")
    else ()
        target_link_directories(third_libs INTERFACE ${ICONV_LIB_DIR})
    endif ()
endif ()

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
# BS::thread_pool(bshoshany-thread-pool), header-only方式的第三方库收敛到third_party
# find_path(BSHOSHANY_THREAD_POOL_INCLUDE_DIRS "BS_thread_pool.hpp")
set(BSHOSHANY_THREAD_POOL_INCLUDE_DIRS "${INNER_THIRD_PARTY_INCLUDE_DIRS}")
MESSAGE(STATUS "BSHOSHANY_THREAD_POOL_INCLUDE_DIRS = ${BSHOSHANY_THREAD_POOL_INCLUDE_DIRS}")
## 查找 thread_pool.hpp 文件路径
## find_path(THREAD_POOL_INCLUDE_DIR BS_thread_pool.hpp PATHS ${CMAKE_PREFIX_PATH}/include /usr/include /usr/local/include ${CMAKE_SOURCE_DIR}/third_party)
if (BSHOSHANY_THREAD_POOL_INCLUDE_DIRS)
    # 提取 BS_THREAD_POOL_VERSION_MAJOR 宏
    file(STRINGS "${BSHOSHANY_THREAD_POOL_INCLUDE_DIRS}/BS_thread_pool.hpp" THREAD_POOL_VERSION_MAJOR_LINE REGEX "#define BS_THREAD_POOL_VERSION_MAJOR")
    if (THREAD_POOL_VERSION_MAJOR_LINE)
        string(REGEX MATCH "[0-9]+" THREAD_POOL_VERSION_MAJOR "${THREAD_POOL_VERSION_MAJOR_LINE}")
    endif ()

    # 提取 BS_THREAD_POOL_VERSION_MINOR 宏
    file(STRINGS "${BSHOSHANY_THREAD_POOL_INCLUDE_DIRS}/BS_thread_pool.hpp" THREAD_POOL_VERSION_MINOR_LINE REGEX "#define BS_THREAD_POOL_VERSION_MINOR")
    if (THREAD_POOL_VERSION_MINOR_LINE)
        string(REGEX MATCH "[0-9]+" THREAD_POOL_VERSION_MINOR "${THREAD_POOL_VERSION_MINOR_LINE}")
    endif ()

    # 提取 BS_THREAD_POOL_VERSION_PATCH 宏
    file(STRINGS "${BSHOSHANY_THREAD_POOL_INCLUDE_DIRS}/BS_thread_pool.hpp" THREAD_POOL_VERSION_PATCH_LINE REGEX "#define BS_THREAD_POOL_VERSION_PATCH")
    if (THREAD_POOL_VERSION_PATCH_LINE)
        string(REGEX MATCH "[0-9]+" THREAD_POOL_VERSION_PATCH "${THREAD_POOL_VERSION_PATCH_LINE}")
    endif ()

    # 组合完整版本号
    set(THREAD_POOL_VERSION_FULL "${THREAD_POOL_VERSION_MAJOR}.${THREAD_POOL_VERSION_MINOR}.${THREAD_POOL_VERSION_PATCH}")

    # 显示版本信息
    message(STATUS "bshoshany-thread-pool version: ${THREAD_POOL_VERSION_FULL}")
    set(BSHOSHANY_THREAD_POOL_VERSION ${THREAD_POOL_VERSION_FULL})
else ()
    message(WARNING "bshoshany-thread-pool headers not found.")
endif ()
echo_lib_version(BS::thread_pool ${BSHOSHANY_THREAD_POOL_VERSION})

# ==============================
# xsimd 13.2.0
# find_package(xsimd CONFIG REQUIRED)
# ==============================
#message("xsimd_INCLUDE_DIRS = ${xsimd_INCLUDE_DIRS}")
file(STRINGS "${INNER_THIRD_PARTY_INCLUDE_DIRS}/xsimd/config/xsimd_config.hpp" xsimd_version_defines
    REGEX "#define XSIMD_VERSION_(MAJOR|MINOR|PATCH)")
foreach (ver ${xsimd_version_defines})
    if (ver MATCHES "#define XSIMD_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
        set(XSIMD_VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}" CACHE INTERNAL "")
    endif ()
endforeach ()
set(xsimd_VERSION ${XSIMD_VERSION_MAJOR}.${XSIMD_VERSION_MINOR}.${XSIMD_VERSION_PATCH})
echo_lib_version(xsimd ${xsimd_VERSION})

# AVX2
if (MSVC)
    # Microsoft compiler
    # AVX2 for Release
    string(APPEND CMAKE_C_FLAGS_RELEASE " /arch:AVX2")
    string(APPEND CMAKE_CXX_FLAGS_RELEASE " /arch:AVX2")

    # AVX for Debug
    string(APPEND CMAKE_C_FLAGS_DEBUG " /arch:AVX")
    string(APPEND CMAKE_CXX_FLAGS_DEBUG " /arch:AVX")
else ()
    # gcc, clang, ...
    #set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mavx -mavx2 -march=native -mtune=native -fno-tree-vectorize")
    #set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mavx -mavx2 -march=native -mtune=native -fno-tree-vectorize")
    #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mavx -mavx2 -march=native -mtune=native -fno-tree-vectorize")
    #set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mavx -mavx2 -march=native -mtune=native -fno-tree-vectorize")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mavx -mavx2 -march=native -mtune=native")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mavx -mavx2 -march=native -mtune=native")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mavx -mavx2 -march=native -mtune=native")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mavx -mavx2 -march=native -mtune=native")
endif ()

# ==============================
# xtensor 0.26.0
# find_package(xtensor CONFIG REQUIRED)
# ==============================
file(STRINGS "${INNER_THIRD_PARTY_INCLUDE_DIRS}/xtensor/core/xtensor_config.hpp" xtensor_version_defines
    REGEX "#define XTENSOR_VERSION_(MAJOR|MINOR|PATCH)")
foreach (ver ${xtensor_version_defines})
    if (ver MATCHES "#define XTENSOR_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
        set(XTENSOR_VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}" CACHE INTERNAL "")
    endif ()
endforeach ()
set(xtensor_VERSION ${XTENSOR_VERSION_MAJOR}.${XTENSOR_VERSION_MINOR}.${XTENSOR_VERSION_PATCH})
echo_lib_version(xtensor ${xtensor_VERSION})

# ==============================
# xtl 0.8.0
# find_package(xtl CONFIG REQUIRED)
# ==============================
file(STRINGS "${INNER_THIRD_PARTY_INCLUDE_DIRS}/xtl/xtl_config.hpp" xtl_version_defines
    REGEX "#define XTL_VERSION_(MAJOR|MINOR|PATCH)")
foreach (ver ${xtl_version_defines})
    if (ver MATCHES "#define XTL_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
        set(XTL_VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}" CACHE INTERNAL "")
    endif ()
endforeach ()
set(xtl_VERSION ${XTL_VERSION_MAJOR}.${XTL_VERSION_MINOR}.${XTL_VERSION_PATCH})
echo_lib_version(xtl ${xtl_VERSION})

# nlohmann-json 依赖 inja:x64-mingw-static@3.4.0
set(inja_VERSION 3.4.0)
set(nlohmann_json_VERSION "3.1.1")
add_library(xtensor_optimize INTERFACE)
if(MSVC)
    target_compile_options(xtensor_optimize INTERFACE /EHsc /MP /bigobj)
else()
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG(-march=native arch_native_supported)
    if(arch_native_supported)
        target_compile_options(xtensor_optimize INTERFACE -march=native)
    endif()
endif()
target_link_libraries(third_libs INTERFACE xtensor_optimize)

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
    add_compile_definitions(KJ_USE_FIBERS=0)
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
#find_package(asio CONFIG REQUIRED)
## 查找 Asio 头文件
if (DEFINED CACHE{ASIO_INCLUDE_DIR})
    message(WARNING "Cache exists: ASIO_INCLUDE_DIR=${ASIO_INCLUDE_DIR}")
    unset(ASIO_INCLUDE_DIR CACHE)
endif ()
find_path(ASIO_INCLUDE_DIR asio.hpp PATHS ${INNER_THIRD_PARTY_INCLUDE_DIRS})
if (ASIO_INCLUDE_DIR)
    # 提取版本信息
    file(STRINGS "${ASIO_INCLUDE_DIR}/asio/version.hpp" ASIO_VERSION_LINE REGEX "#define ASIO_VERSION")
    if (ASIO_VERSION_LINE)
        string(REGEX MATCH "[0-9]+" ASIO_VERSION "${ASIO_VERSION_LINE}")
        math(EXPR ASIO_MAJOR_VERSION "${ASIO_VERSION} / 100000")
        math(EXPR ASIO_MINOR_VERSION "(${ASIO_VERSION} / 100) % 1000")
        math(EXPR ASIO_PATCH_VERSION "${ASIO_VERSION} % 100")
        set(ASIO_VERSION_FULL "${ASIO_MAJOR_VERSION}.${ASIO_MINOR_VERSION}.${ASIO_PATCH_VERSION}")
    endif ()

    file(STRINGS "${ASIO_INCLUDE_DIR}/asio/version.hpp" ASIO_VERSION_STR_LINE REGEX "#define ASIO_VERSION_STR")
    if (ASIO_VERSION_STR_LINE)
        string(REGEX MATCH "\"[^\"]+\"" ASIO_VERSION_STR "${ASIO_VERSION_STR_LINE}")
        string(REPLACE "\"" "" ASIO_VERSION_STR "${ASIO_VERSION_STR}")
    endif ()

    # 显示版本信息
    message(STATUS "asio version (numeric): ${ASIO_VERSION_FULL}")
    message(STATUS "asio version (string): ${ASIO_VERSION_STR}")

    # 添加头文件路径
    include_directories(${ASIO_INCLUDE_DIR})
    set(ASIO_VERSION ${ASIO_VERSION_FULL})
else ()
    message(FATAL_ERROR "Asio headers not found. Please install Asio and specify its path.")
endif ()

# mimalloc
find_package(mimalloc CONFIG REQUIRED)
target_link_libraries(third_libs INTERFACE $<IF:$<TARGET_EXISTS:mimalloc-static>,mimalloc-static,mimalloc>)
target_compile_definitions(third_libs INTERFACE MI_OVERRIDE=1)
echo_lib_version(mimalloc ${mimalloc_VERSION})

# C++ Requests Library (Cpr for curl)
find_package(cpr CONFIG REQUIRED)
target_link_libraries(third_libs INTERFACE cpr::cpr)
echo_lib_version(cpr ${cpr_VERSION})

# zlib
find_package(ZLIB REQUIRED)
target_link_libraries(third_libs INTERFACE ZLIB::ZLIB)
echo_lib_version(ZLIB ${ZLIB_VERSION})

# fmt
find_package(fmt CONFIG REQUIRED)
target_link_libraries(third_libs INTERFACE fmt::fmt)
echo_lib_version(fmt ${fmt_VERSION})
find_package(date CONFIG REQUIRED)
target_link_libraries(third_libs INTERFACE date::date date::date-tz)

# spdlog
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(third_libs INTERFACE spdlog::spdlog_header_only)
#set(SPDLOG_FMT_EXTERNAL OFF)
#set(SPDLOG_NO_EXTERNAL_FMT ON)
#set(SPDLOG_HEADER_PATH "${INNER_THIRD_PARTY_INCLUDE_DIRS}/spdlog/version.h")
## 检查文件是否存在
#if(EXISTS ${SPDLOG_HEADER_PATH})
#    # 读取并解析版本号
#    file(READ ${SPDLOG_HEADER_PATH} SPDLOG_HEADER_CONTENTS)
#
#    string(REGEX MATCH "#define SPDLOG_VER_MAJOR ([0-9]+)" _ ${SPDLOG_HEADER_CONTENTS})
#    if(CMAKE_MATCH_1)
#        set(SPDLOG_VERSION_MAJOR ${CMAKE_MATCH_1})
#    else()
#        message(WARNING "Could not determine SPDLOG_VER_MAJOR from version.h")
#        set(SPDLOG_VERSION_MAJOR 0)
#    endif()
#
#    string(REGEX MATCH "#define SPDLOG_VER_MINOR ([0-9]+)" _ ${SPDLOG_HEADER_CONTENTS})
#    if(CMAKE_MATCH_1)
#        set(SPDLOG_VERSION_MINOR ${CMAKE_MATCH_1})
#    else()
#        message(WARNING "Could not determine SPDLOG_VER_MINOR from version.h")
#        set(SPDLOG_VERSION_MINOR 0)
#    endif()
#
#    string(REGEX MATCH "#define SPDLOG_VER_PATCH ([0-9]+)" _ ${SPDLOG_HEADER_CONTENTS})
#    if(CMAKE_MATCH_1)
#        set(SPDLOG_VERSION_PATCH ${CMAKE_MATCH_1})
#    else()
#        message(WARNING "Could not determine SPDLOG_VER_PATCH from version.h")
#        set(SPDLOG_VERSION_PATCH 0)
#    endif()
#
#    set(spdlog_VERSION "${SPDLOG_VERSION_MAJOR}.${SPDLOG_VERSION_MINOR}.${SPDLOG_VERSION_PATCH}")
#    message(STATUS "Detected spdlog version: ${SPDLOG_VERSION}")
#else()
#    message(WARNING "spdlog version.h not found at: ${SPDLOG_HEADER_PATH}")
#    set(spdlog_VERSION "0.0.0")
#endif()
echo_lib_version(spdlog ${spdlog_VERSION})

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
    include/q1x/strategies/strategy.h
    include/q1x/strategies/rule-engine.h
    include/q1x/strategies/rule-error.h
    include/q1x/strategies/rule-context.h

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
    src/q1x/strategies/rule-engine.cpp
    src/q1x/strategies/strategy.cpp

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

# 链接不同平台的库
if (WIN32)
    target_link_libraries(third_libs INTERFACE ws2_32)   # Windows 网络库, 标准 Winsock API 支持, TCP/UDP 通信、客户端/服务器, winsock2.h
    target_link_libraries(third_libs INTERFACE mswsock)  # Windows 网络库, 微软扩展函数（高性能网络）, IOCP、异步网络、高性能服务端, mswsock.h
    target_link_libraries(third_libs INTERFACE Imagehlp) # Windows Crash Dump, PE 文件结构解析
    target_link_libraries(third_libs INTERFACE Dbghelp)  # Windows Crash Dump, 堆栈回溯（Stack Walking）
    target_link_libraries(third_libs INTERFACE advapi32) # Windows 服务管理
    #find_package(libdwarf CONFIG REQUIRED)
    #target_link_libraries(third_libs INTERFACE libdwarf::dwarf)
    target_link_libraries(third_libs INTERFACE psapi)
elseif (LINUX)
    # vcpkg版本的libdwarf未实验成功
    #target_link_libraries(third_libs PUBLIC libdwarf::dwarf)
    # #define BACKWARD_HAS_DW 1
    target_link_libraries(third_libs INTERFACE dw bfd) # apt-get install libdw-dev
    # #define BACKWARD_HAS_DWARF 1
    #target_link_libraries(third_libs INTERFACE elf dwarf) # apt-get install libdwarf-dev
elseif (APPLE)
    target_link_libraries(third_libs INTERFACE dwarf) # apt-get install libdw-dev
endif ()

get_target_property(_libs third_libs INTERFACE_LINK_LIBRARIES)
message(STATUS "Library 'third_libs' requires: ${_libs}")

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
target_compile_options(q1x PUBLIC ${quant1x_global_compile_options})
target_link_libraries(q1x PUBLIC
    third_libs
)

if (MINGW)
    # MinGW (GCC)，Windows 下使用 -mthreads
    target_link_options(q1x PUBLIC -mthreads)
elseif (MSVC)
    # Visual Studio，不需要 -mthreads 或 -pthread
    # Windows 上 MSVC 的多线程由编译器自动处理
    # 可以什么都不加，或根据需要添加特定选项
    message(STATUS "Using MSVC, no thread flags needed.")
else ()
    # Linux/macOS 使用 -pthread
    target_compile_options(q1x PUBLIC -pthread)
    target_link_options(q1x PUBLIC -pthread)
endif ()

get_target_property(_libs q1x INTERFACE_LINK_LIBRARIES)
message(STATUS "Library 'q1x' requires: ${_libs}")

# 检查编译器选项
get_target_property(_includes q1x INCLUDE_DIRECTORIES)
get_target_property(_libs q1x LINK_LIBRARIES)
get_target_property(_defines q1x COMPILE_DEFINITIONS)
get_target_property(_options q1x COMPILE_OPTIONS)

message(STATUS "Includes: ${_includes}")
message(STATUS "Libs: ${_libs}")
message(STATUS "Defines: ${_defines}")
message(STATUS "Options: ${_options}")

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

# 设置安装路径变量
include(GNUInstallDirs)
MESSAGE(STATUS "RUNTIME = ${RUNTIME}")
set(CMAKE_INSTALL_PREFIX ${RUNTIME})
MESSAGE(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
set(THIRD_PARTY_INSTALL_INCDIR "${CMAKE_INSTALL_INCLUDEDIR}/third_party")

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
install(TARGETS q1x third_libs xtensor_optimize quant1x_global_compile_options EXPORT q1x
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

# 安装主库头文件
install(
    DIRECTORY include/ DESTINATION include
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