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
    if(APPLE)
        set(PROTOBUF_LIB_NAME "protobuf")
    else ()
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            # Debug模式下链接
            set(PROTOBUF_LIB_NAME "libprotobufd")
        else ()
            # Release模式下链接
            set(PROTOBUF_LIB_NAME "libprotobuf")
        endif ()
    endif(APPLE)
else ()
    set(PROTOBUF_LIB_NAME "protobuf")
endif ()

target_link_libraries(third_libs INTERFACE ${PROTOBUF_LIB_NAME})
echo_lib_version(protobuf ${Protobuf_VERSION})
