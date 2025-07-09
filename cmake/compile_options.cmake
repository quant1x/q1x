# cmake/compile_options.cmake

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(CMakeDependentOption)
include(CMakeDetermineSystem)

# -----------------------------------------------------------------------------
# 1. 检测构建类型
# -----------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
        "Choose the default build type: Debug, Release, RelWithDebInfo, MinSizeRel")
endif()

message(STATUS "Build Type: \"${CMAKE_BUILD_TYPE}\"")
set(CMAKE_INSTALL_CONFIG_NAME ${CMAKE_BUILD_TYPE})
message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")

# 设置默认的构建类型
if (CMAKE_GENERATOR MATCHES "^Visual Studio.*$")
    message(STATUS "Note: when building with Visual Studio the build type is specified when building.")
    message(STATUS "For example: 'cmake --build . --config=Release")
endif()

option(USE_STATIC_CRT "Use static CRT" ON)
if(MSVC AND USE_STATIC_CRT)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# -----------------------------------------------------------------------------
# 2. 系统环境
# -----------------------------------------------------------------------------
message("CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")
message("     CMAKE_HOST_SYSTEM: ${CMAKE_HOST_SYSTEM}")
message("     CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message("          CMAKE_SYSTEM: ${CMAKE_SYSTEM}")
message("       CMAKE_HOST_UNIX: ${CMAKE_HOST_UNIX}")
message("      CMAKE_HOST_WIN32: ${CMAKE_HOST_WIN32}")
message("      CMAKE_HOST_WIN64: ${CMAKE_HOST_WIN64}")

# CPU类型
#include(CMakeDetermineSystem OPTIONAL)
message("                System: ${CMAKE_SYSTEM}")
message("             Processor: ${CMAKE_SYSTEM_PROCESSOR}")

message(STATUS "CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpmachine
        OUTPUT_VARIABLE MACHINE_DUMP
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if (MACHINE_DUMP MATCHES "mingw" OR
        MACHINE_DUMP MATCHES "windows" OR
        CMAKE_CXX_COMPILER MATCHES "mingw")
        set(IS_MINGW TRUE)
        message(STATUS "Detected MinGW GCC (${MACHINE_DUMP})")
    else ()
        set(IS_MINGW FALSE)
        message(STATUS "Detected regular GCC (${MACHINE_DUMP})")
    endif ()
endif ()
# 打印检测到的 C 和 C++ 编译器
message(STATUS "  C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")

# 打印编译器版本
execute_process(
    COMMAND "${CMAKE_CXX_COMPILER}" --version
    OUTPUT_VARIABLE COMPILER_VERSION
)
message(STATUS "Compiler version:\n${COMPILER_VERSION}")

set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

# -----------------------------------------------------------------------------
# 3. 创建全局接口库（核心）
# -----------------------------------------------------------------------------
# 3.1 定义接口传播
add_library(quant1x_global_compile_options INTERFACE)

# 3.2 标准配置（强制传播）
set_target_properties(quant1x_global_compile_options PROPERTIES
    INTERFACE_C_STANDARD 17
    INTERFACE_CXX_STANDARD 20
    INTERFACE_C_STANDARD_REQUIRED ON
    INTERFACE_CXX_STANDARD_REQUIRED ON
    INTERFACE_C_EXTENSIONS OFF
    INTERFACE_CXX_EXTENSIONS OFF
)

# 3.3 字符集处理（全平台覆盖）
if(MSVC)
    target_compile_options(quant1x_global_compile_options INTERFACE /utf-8)
    target_compile_definitions(quant1x_global_compile_options INTERFACE _UNICODE UNICODE)
else()
    target_compile_options(quant1x_global_compile_options INTERFACE
        -finput-charset=UTF-8
        -fexec-charset=UTF-8
    )
endif()

# 3.4 警告级别
if(MSVC)
    # 对于MSVC编译器
    target_compile_options(quant1x_global_compile_options INTERFACE /W3 /WX) # /W4显示所有警告，/WX将警告视为错误
else()
    # 对于GCC/Clang等编译器
    target_compile_options(quant1x_global_compile_options INTERFACE
        -Wall -Wextra
        -Wuninitialized
        -pedantic -Werror
        -Wunused -Wno-shadow #-Wshadow -Wconversion
    )
    # GNU 特有选项
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_compile_options(quant1x_global_compile_options INTERFACE -Wmaybe-uninitialized)
        target_compile_options(quant1x_global_compile_options INTERFACE -Wreturn-local-addr)
        target_compile_options(quant1x_global_compile_options INTERFACE -ftrivial-auto-var-init=zero)
        if (IS_MINGW)
            # MinGW 特有设置
            target_compile_options(quant1x_global_compile_options INTERFACE -DMINGW_COMPILER)
        else ()
            # 其他系统 GCC 设置
        endif ()
    endif ()
    # Clang 特有选项
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(quant1x_global_compile_options INTERFACE -Wmove)
    endif ()
endif()
#
## 3.5. 优化配置（Debug/Release分离）
#target_compile_options(quant1x_global_compile_options INTERFACE
#    "$<$<CONFIG:Debug>:-O0 -g3 -fno-omit-frame-pointer>"
#    "$<$<CONFIG:Release>:-O3 -fomit-frame-pointer>"
#)
if(MSVC)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(quant1x_global_compile_options INTERFACE /Od /GS)
    else ()
        target_compile_options(quant1x_global_compile_options INTERFACE /O2 /Gy /GF /GS)
    endif ()
else ()
    target_compile_options(quant1x_global_compile_options INTERFACE -O2 -m64 -march=native -mtune=native -fstack-protector-strong)
    # 启用 function/data sections
    #target_compile_options(quant1x_global_compile_options INTERFACE -ffunction-sections -fdata-sections)
endif()

# 6.6. 异常处理（强制启用）
set_target_properties(quant1x_global_compile_options PROPERTIES INTERFACE_CXX_EXCEPTIONS ON)
if(MSVC)
    # 对于 MSVC 编译器
    target_compile_options(quant1x_global_compile_options INTERFACE /EHsc) # /EHsc 启用 C++ 异常处理
else()
    # 对于 GCC/Clang 等编译器
    target_compile_options(quant1x_global_compile_options INTERFACE -fexceptions) # 启用 C++ 异常处理
endif()
## 或方法 3: 使用内置变量（推荐）
#set(CMAKE_CXX_EXCEPTIONS ON)
#set(CMAKE_VERBOSE_MAKEFILE ON)

# 仅对新版MSVC添加高级保护（需版本检测）
if(MSVC_VERSION GREATER_EQUAL 1920)  # VS2019+
    # 合法选项替代方案
    target_compile_options(quant1x_global_compile_options INTERFACE
        "/volatile:iso"  # 严格内存顺序
        "/Zc:threadSafeInit-"      # 禁用线程安全初始化（测试编译器bug）
        #"/d2:-IncrementalLink"     # 增强链接时检查
    )
    # 确保静态变量在独立段
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SECTION:.staticvars,RWS")
    target_link_options(quant1x_global_compile_options INTERFACE "/SECTION:.staticvars,RWS")
else()
    # 通用保护方案
    target_compile_options(quant1x_global_compile_options INTERFACE -DSTATIC_VAR_PROTECTION)
endif()

# 关于debug和release模式的编译选项
if (MSVC)
    # 确保 Debug 模式使用正确的调试信息格式
    target_compile_options(quant1x_global_compile_options INTERFACE
        "$<$<CONFIG:Debug>:/Zi /DEBUG>"
    )
    # 确保 Release 模式不包含调试符号（可选）
    target_compile_options(quant1x_global_compile_options INTERFACE
        "$<$<CONFIG:Release>:/O2>"
    )
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG")
    #set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else ()
    target_compile_options(quant1x_global_compile_options INTERFACE "$<$<CONFIG:Debug>:-g>")
    target_compile_options(quant1x_global_compile_options INTERFACE "$<$<CONFIG:Release>:-O2>")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")
endif ()

if (MINGW OR GNU)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++" CACHE STRING "Use libstdc++")
    target_compile_options(quant1x_global_compile_options INTERFACE "-stdlib=libstdc++")
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libstdc++" CACHE STRING "")
    target_link_options(quant1x_global_compile_options INTERFACE "-stdlib=libstdc++")
    #set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libstdc++" CACHE STRING "")
    #target_link_options(quant1x_global_compile_options INTERFACE "/SECTION:.staticvars,RWS")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++" CACHE STRING "Use libstdc++")
    target_compile_options(quant1x_global_compile_options INTERFACE "-stdlib=libc++")
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++" CACHE STRING "")
    #set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++" CACHE STRING "")
    target_link_options(quant1x_global_compile_options INTERFACE "-stdlib=libc++")
endif ()

# 7. 平台特定配置
if(MSVC)
    # MSVC特有设置
    target_compile_options(quant1x_global_compile_options INTERFACE
        /MP           # 多进程编译
        /bigobj       # 大对象支持
    )
elseif (MINGW OR GNU)
    # MinGW设置
    target_compile_options(quant1x_global_compile_options INTERFACE -static)
    # 关键修复：减少段数量
    target_compile_options(quant1x_global_compile_options INTERFACE
        -Wa,-mbig-obj  # 启用COFF大对象格式支持
    )
    # 优化段生成
    target_link_options(quant1x_global_compile_options INTERFACE
        -Wl,--gc-sections  # 链接时回收未使用段
    )
elseif(UNIX AND NOT APPLE)
    # Linux设置
    target_compile_options(quant1x_global_compile_options INTERFACE -pthread -Wl,--as-needed)
else ()
    # APPLE
endif()

# 10. 验证输出（构建时可见）
get_target_property(ALL_OPTS quant1x_global_compile_options INTERFACE_COMPILE_OPTIONS)
message(STATUS "Global compile options: ${ALL_OPTS}")