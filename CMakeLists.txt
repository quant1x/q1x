cmake_minimum_required(VERSION 3.30)
project(api VERSION 0.0.1 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
        "Choose the default build type: Debug, Release, RelWithDebInfo, MinSizeRel")
endif()
message(STATUS "Build Type: \"${CMAKE_BUILD_TYPE}\"")
set(CMAKE_INSTALL_CONFIG_NAME ${CMAKE_BUILD_TYPE})
message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")

# 全局编码设置
if(MSVC)
    add_compile_options(/utf-8)
    add_definitions(-D_UNICODE -DUNICODE)
else()
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

if (DEFINED ENV{MSF_RUNTIME})
    file(TO_CMAKE_PATH "$ENV{MSF_RUNTIME}" RUNTIME)
    string(REGEX REPLACE "/$" "" RUNTIME "${RUNTIME}")
else ()
    message(FATAL_ERROR "NOT found ENV MSF_RUNTIME")
endif ()
MESSAGE(STATUS "RUNTIME = ${RUNTIME}")
set(CMAKE_INSTALL_PREFIX ${RUNTIME})
MESSAGE(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")

# 设置输出目录
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 添加头文件路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# 头文件
set(HEADERS
    ${PROJECT_SOURCE_DIR}/include/api.h
)
# 源文件
set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/api.cpp
)

# 添加库目标
add_library(api SHARED ${HEADERS} ${SOURCES})
add_library(api-static STATIC ${HEADERS} ${SOURCES})

# 为动态库定义导出宏
target_compile_definitions(api PRIVATE API_DECLARE_EXPORT)
target_compile_definitions(api-static PRIVATE API_DECLARE_STATIC)
# 为静态库和动态库分别设置目标名称，避免冲突
set_target_properties(api PROPERTIES OUTPUT_NAME "api")
set_target_properties(api-static PROPERTIES OUTPUT_NAME "api-static")


## 可选：为 Windows 平台生成 dll 时生成 .def 文件
# if(WIN32)
#     set_target_properties(api PROPERTIES PREFIX "" SUFFIX ".dll")
# endif()

include(GNUInstallDirs)

# 安装配置（可选）
install(TARGETS api api-static EXPORT api
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(DIRECTORY include/ DESTINATION include)
# 安装 CMake 配置文件
install(EXPORT api
        FILE api.cmake
        NAMESPACE api::
        DESTINATION lib/cmake/api)