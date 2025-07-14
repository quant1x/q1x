
# 定义一个宏，用于创建 一般性的测试 可执行文件
macro(add_app_executable source_file)
    # 获取源文件名（不含扩展名）
    get_filename_component(test_name ${source_file} NAME_WE)

    # 添加前缀 "app-"
    set(target_name "app-${test_name}")

    # 创建带前缀的可执行文件
    add_executable(${target_name} ${source_file})

    # 添加头文件路径
    target_include_directories(${target_name} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/third_party/include
    )

    # 链接依赖库（例如你提到的 utils）
    target_link_libraries(${target_name} PRIVATE
        q1x
    )
endmacro()

# 定义一个宏，用于创建 Catch2单元测试 可执行文件
macro(add_catch2_executable source_file)
    # 获取源文件名（不含扩展名）
    get_filename_component(test_name ${source_file} NAME_WE)

    # 添加前缀 "catch2-"
    set(target_name "catch2-${test_name}")

    # 创建带前缀的可执行文件
    add_executable(${target_name} ${source_file})

    # 添加头文件路径
    target_include_directories(${target_name} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/third_party/include
    )

    # 链接依赖库（例如你提到的 utils）
    target_link_libraries(${target_name} PRIVATE
        q1x test-catch2 test-benchmark
    )
endmacro()

# 定义一个宏，用于创建 GTest单元测试 可执行文件
macro(add_gtest_executable source_file)
    # 获取源文件名（不含扩展名）
    get_filename_component(test_name ${source_file} NAME_WE)

    # 添加前缀 "gtest-"
    set(target_name "gtest-${test_name}")

    # 创建带前缀的可执行文件
    add_executable(${target_name} ${source_file})

    # 添加头文件路径
    target_include_directories(${target_name} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/third_party/include
    )

    # 链接依赖库（例如你提到的 utils）
    target_link_libraries(${target_name} PRIVATE
        q1x test-gtest
    )
endmacro()

# 定义一个宏，用于创建 benchmark基准测试 可执行文件
macro(add_benchmark_executable source_file)
    # 获取源文件名（不含扩展名）
    get_filename_component(test_name ${source_file} NAME_WE)

    # 添加前缀 "benchmark-"
    set(target_name "benchmark-${test_name}")

    # 创建带前缀的可执行文件
    add_executable(${target_name} ${source_file})

    # 添加头文件路径
    target_include_directories(${target_name} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/third_party/include
    )

    # 链接依赖库（例如你提到的 utils）
    target_link_libraries(${target_name} PRIVATE
        q1x test-benchmark
    )
endmacro()

# crash测试
if (WIN32)
    add_app_executable(crash-windows.cpp)
elseif (LINUX)
    add_app_executable(crash-linux.cpp)
endif ()
add_app_executable(test-crash.cpp)

# 错误码测试
add_catch2_executable(tdd-error.cpp)

# 测试宿主目录
add_catch2_executable(tdd-homedir.cpp)
# 字符集测试
add_catch2_executable(tdd-iconv.cpp)

# cron测试
add_catch2_executable(tdd-cron.cpp)

# 测试缓存
add_catch2_executable(test-calender.cpp)

# formula测试
add_catch2_executable(tdd-formula.cpp)

# f10
add_catch2_executable(tdd-notices.cpp)
add_catch2_executable(tdd-safety-score.cpp)
add_catch2_executable(tdd-margin-trading.cpp)

# 筹码分布
add_catch2_executable(tdd-chips.cpp)