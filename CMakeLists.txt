cmake_minimum_required(VERSION 3.30)

# =============================
# 对齐输出宏定义, 可用于中间对齐的KV显示
# =============================
macro(pretty_print_label label value)
    set(max_len 36)
    string(LENGTH "${label}" len)
    math(EXPR padding "${max_len} - ${len}")
    if(padding GREATER 0)
        string(REPEAT " " ${padding} pad_str)
    else()
        set(pad_str "")
    endif()
    message(STATUS "${pad_str}${label} : ${value}")
endmacro()

# =============================
# 对齐输出宏定义
# =============================
macro(echo_lib_version label value)
    message("Found ${label}, version = ${value}")
endmacro()

project(quant1x-api VERSION 0.0.1 LANGUAGES CXX)

# ============================================================
# 设置 C++ 标准为 C++20
# ============================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # 可选：禁用编译器扩展（如 MSVC 的 /Zc:__cplusplus）

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
        "Choose the default build type: Debug, Release, RelWithDebInfo, MinSizeRel")
endif()

message(STATUS "Build Type: \"${CMAKE_BUILD_TYPE}\"")
set(CMAKE_INSTALL_CONFIG_NAME ${CMAKE_BUILD_TYPE})
message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")

# ============================================================
# 全局编码设置
# ============================================================
if(MSVC)
    add_compile_options(/utf-8)
    add_definitions(-D_UNICODE -DUNICODE)
else()
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

# ============================================================
# vcpkg Options/Dependencies
# ============================================================
add_library(third_libs INTERFACE)
# 可选：显式链接静态版 pthread
find_package(Threads REQUIRED)
target_link_libraries(third_libs INTERFACE Threads::Threads -lpthread)
if (WIN32)
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
#unset(VCPKG_TARGET_TRIPLET CACHE)
# 根据平台自动选择 Triplet
if (WIN32)
    if (MSVC)
        set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "Default Triplet for Windows")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") # 设置 MSVC 静态链接运行时库
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
#set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "Default Triplet for Windows")
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


## spdlog
#find_package(spdlog CONFIG REQUIRED)
#target_link_libraries(third_libs INTERFACE spdlog::spdlog_header_only)
#
## fmt
#find_package(fmt CONFIG REQUIRED)
#target_link_libraries(third_libs INTERFACE fmt::fmt)
#echo_lib_version(fmt ${fmt_VERSION})
#find_package(date CONFIG REQUIRED)
#target_link_libraries(third_libs INTERFACE date::date date::date-tz)
#
# C++ Requests Library (Cpr for curl)
find_package(cpr CONFIG REQUIRED)
target_link_libraries(third_libs INTERFACE cpr::cpr)
echo_lib_version(cpr ${cpr_VERSION})

# ============================================================
# api 基础设施库
# ============================================================

# 添加头文件路径
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/api-third-party)

# 头文件
set(HEADERS
    include/api/api.h
    include/api/base/cache1d.h
    include/api/base/once.h
    include/api/base/scheduler.h
    include/api/crash/handler.h
    include/api/encoding/csv.h
    include/api/encoding/json.h
    include/api/encoding/yaml.h
    include/api/io/csv-reader.h
    include/api/io/csv-writer.h
    include/api/io/file.h
    include/api/io/http.h
    include/api/logger.h
    include/api/os/service.h
    include/api/affinity.h
    include/api/algo.h
    include/api/base.h
    include/api/buffer.h
    include/api/config.h.in
    include/api/except.h
    include/api/feature_detection.h
    include/api/format.h
    include/api/iconv.h
    include/api/mmap.h
    include/api/numerics.h
    include/api/object.h
    include/api/std.h
    include/api/strings.h
    include/api/time.h
    include/api/util.h
    include/api/runtime.h

    include/api/test/test.h
    
    include/api/dataframe/dataframe.h
    include/api/dataframe/ewm.h
    include/api/dataframe/periods.h
    include/api/dataframe/series.h

    include/api/net/base.h
    include/api/net/connection_pool.h
    include/api/net/endpoint.h
    include/api/net/operation_handler.h
)
# 源文件
set(SOURCES
    src/api/api.cpp
    src/api/crash/handler.cpp
    src/api/encoding/json.cpp
    src/api/io/file.cpp
    src/api/io/http.cpp
    src/api/affinity.cpp
    src/api/iconv.cpp
    src/api/numerics.cpp
    src/api/strings.cpp
    src/api/time.cpp
    src/api/util.cpp
    src/api/runtime.cpp
    src/api/logger.cpp
)

if (WIN32)
    list(APPEND SOURCES src/api/os/service.cpp)
elseif (APPLE)
    list(APPEND SOURCES src/api/os/daemon_macosx.cpp)
elseif (UNIX)
    # LINUX 是非官方变量，我们手动检测
    set(LINUX FALSE)
    if(CMAKE_SYSTEM_NAME MATCHES "Linux")
        set(LINUX TRUE)
    endif()

    if(LINUX)
        list(APPEND SOURCES src/api/os/daemon_systemd.cpp)
    else()
        message(WARNING "Unsupported Unix OS: ${CMAKE_SYSTEM_NAME}")
    endif()
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif ()

# 添加库目标
add_library(api SHARED ${HEADERS} ${SOURCES})
add_library(api-static STATIC ${HEADERS} ${SOURCES})

# 为动态库定义导出宏
target_compile_definitions(api PRIVATE API_DECLARE_EXPORT)
target_compile_definitions(api-static PRIVATE API_DECLARE_STATIC)

# 清除DLL文件的前缀
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_IMPORT_LIBRARY_PREFIX "")
# 为静态库和动态库分别设置目标名称，避免冲突
set_target_properties(api PROPERTIES OUTPUT_NAME "api" PREFIX "")
set_target_properties(api-static PROPERTIES OUTPUT_NAME "api-static" PREFIX "")

if (DEFINED ENV{MSF_RUNTIME})
    file(TO_CMAKE_PATH "$ENV{MSF_RUNTIME}" RUNTIME)
    string(REGEX REPLACE "/$" "" RUNTIME "${RUNTIME}")
else ()
    message(FATAL_ERROR "NOT found ENV MSF_RUNTIME")
endif ()
MESSAGE(STATUS "RUNTIME = ${RUNTIME}")
set(CMAKE_INSTALL_PREFIX ${RUNTIME})
MESSAGE(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")

target_link_libraries(api PUBLIC
    third_libs
)

target_link_libraries(api-static PUBLIC
    third_libs
)

## 可选：为 Windows 平台生成 dll 时生成 .def 文件
# if(WIN32)
#     set_target_properties(api PROPERTIES PREFIX "" SUFFIX ".dll")
# endif()

# ============================================================
# Crash BackTrace
# ============================================================
if(MSVC)
    # 仅在 MSVC 下设置符号生成选项
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Zi")
endif()
if (WIN32)
    target_link_libraries(api PUBLIC ws2_32)   # Windows 网络库, 标准 Winsock API 支持, TCP/UDP 通信、客户端/服务器, winsock2.h
    target_link_libraries(api PUBLIC mswsock)  # Windows 网络库, 微软扩展函数（高性能网络）, IOCP、异步网络、高性能服务端, mswsock.h
    target_link_libraries(api PUBLIC Imagehlp) # Windows Crash Dump, PE 文件结构解析
    target_link_libraries(api PUBLIC Dbghelp)  # Windows Crash Dump, 堆栈回溯（Stack Walking）
    target_link_libraries(api PUBLIC advapi32) # Windows 服务管理
elseif(LINUX)
    # vcpkg版本的libdwarf未实验成功
    #target_link_libraries(api PUBLIC libdwarf::dwarf)
    # #define BACKWARD_HAS_DW 1
    target_link_libraries(api PUBLIC dw) # apt-get install libdw-dev
    # #define BACKWARD_HAS_DWARF 1
    #target_link_libraries(api PUBLIC elf dwarf) # apt-get install libdwarf-dev
elseif(APPLE)
    target_link_libraries(api PUBLIC dwarf) # apt-get install libdw-dev
endif ()

if (WIN32)
    target_link_libraries(api-static PUBLIC ws2_32)
    target_link_libraries(api-static PUBLIC mswsock)
    target_link_libraries(api-static PUBLIC Imagehlp)
    target_link_libraries(api-static PUBLIC Dbghelp)
    target_link_libraries(api-static PUBLIC advapi32)
elseif(LINUX)
    target_link_libraries(api-static PUBLIC dw)
elseif(APPLE)
    target_link_libraries(api-static PUBLIC dwarf)
endif ()

include(GNUInstallDirs)

# 安装配置（可选）
install(TARGETS api api-static third_libs EXPORT api
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)
# 安装 CMake 配置文件
install(EXPORT api
        FILE api.cmake
        NAMESPACE api::
        DESTINATION lib/cmake/api)

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

# 单元测试
add_subdirectory(tests)