cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(NetworkUtils)

# 设置C标准
set(CMAKE_C_STANDARD 99)

# 设置构建类型为Debug，以包含调试信息
set(CMAKE_BUILD_TYPE Debug)

# 查找check库
set(CHECK_INCLUDE_DIRS "/usr/include")
set(CHECK_LIBRARIES "-pthread -lcheck_pic -pthread -lrt -lm -lsubunit")

# 包含check库的头文件
include_directories(${CHECK_INCLUDE_DIRS})
include_directories(${CHECK_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)  # 添加包含目录，以便找到 network.h

# 将所有源文件赋值给变量SOURCES
file(GLOB SOURCES "src/*.c" "../log/*.c")

# 创建一个可执行文件
add_executable(test_network ${SOURCES})

# 链接check库到可执行文件
target_link_libraries(test_network ${CHECK_LIBRARIES} pthread m rt)
