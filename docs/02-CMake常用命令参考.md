# CMake 常用命令参考

> 📂 **对应示例**：本节内容对应示例代码 [examples/variables/](../examples/variables/) 和 [examples/targets/](../examples/targets/)，建议结合示例代码一起学习。

## 项目配置命令

### cmake\_minimum\_required

> 💡 **比喻**：这就像告诉施工队"你至少需要5年以上经验的老师傅"，确保他们有足够的能力来完成工作。

设置 CMake 的最低版本要求：

```cmake
cmake_minimum_required(VERSION 3.20)

# 设置版本范围
cmake_minimum_required(VERSION 3.15...3.25)
```

### project

> 💡 **比喻**：这就像给项目起名字、登记备案。你需要告诉CMake这个项目叫什么、从哪来、做什么用的。

定义项目名称和属性：

```cmake
# 基本用法
project(MyProject)

# 完整用法
project(MyProject
    VERSION 1.0.0
    LANGUAGES CXX C
    DESCRIPTION "My awesome project"
    HOMEPAGE_URL "https://example.com"
)
```

项目版本变量：

- `PROJECT_VERSION` - 完整版本号（1.0.0）
- `PROJECT_VERSION_MAJOR` - 主版本号（1）
- `PROJECT_VERSION_MINOR` - 次版本号（0）
- `PROJECT_VERSION_PATCH` - 补丁版本号（0）

***

## 目标定义命令

### add\_executable

创建可执行目标：

```cmake
# 基本用法
add_executable(my_app main.cpp)

# 添加多个源文件
add_executable(my_app
    main.cpp
    utils.cpp
    helper.cpp
)

# 从变量获取源文件
set(SOURCES main.cpp utils.cpp)
add_executable(my_app ${SOURCES})

# Windows 窗口程序（不显示控制台黑框）
add_executable(my_app WIN32 main.cpp)
```

### add\_library

> 💡 **比喻**：这是"创建预制房间"的命令。你要告诉CMake：要什么样的房间（静态/动态/头文件）、用什么材料（源文件）。

创建库目标：

```cmake
# 静态库（.a / .lib） - 就像浇筑好的混凝土块
add_library(my_lib STATIC lib.cpp)

# 动态库（.so / .dll） - 就像可拆卸的预制房间
add_library(my_lib SHARED lib.cpp)

# 模块库（插件） - 就像可选的家具包
add_library(my_lib MODULE plugin.cpp)

# 头文件库（header-only） - 就像纯说明书，没有实体材料
add_library(my_lib INTERFACE)

# 对象库（编译但不归档） - 就像已经切好但还没组装好的木材
add_library(my_lib OBJECT lib.cpp)
```

### add\_custom\_target

创建自定义目标：

```cmake
# 基本用法
add_custom_target(documentation
    COMMAND doxygen Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating documentation"
)

# 添加依赖
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND}
    DEPENDS my_test_executable
)
```

***

## 目标属性命令

### target\_include\_directories

设置目标的包含目录：

```cmake
target_include_directories(my_target
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_BINARY_DIR}
)
```

### target\_compile\_definitions

设置编译定义：

```cmake
target_compile_definitions(my_target
    PUBLIC
        API_EXPORTS
    PRIVATE
        DEBUG_MODE
        VERSION="1.0.0"
)
```

### target\_compile\_options

设置编译选项：

```cmake
target_compile_options(my_target
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>
)
```

### target\_compile\_features

设置编译特性（C++ 标准）：

```cmake
# 要求 C++20
target_compile_features(my_target PUBLIC cxx_std_20)

# 要求特定特性
target_compile_features(my_target PRIVATE cxx_lambdas cxx_constexpr)
```

### target\_link\_libraries

链接库到目标：

```cmake
# 链接库目标
target_link_libraries(my_app
    PRIVATE
        my_lib
        another_lib
)

# 链接系统库
target_link_libraries(my_app PRIVATE pthread)

# 链接选项
target_link_libraries(my_app PRIVATE -static)

# 链接框架（macOS）
target_link_libraries(my_app PRIVATE "-framework Cocoa")
```

### target\_link\_directories

设置链接目录（不推荐，优先使用 `target_link_libraries`）：

```cmake
target_link_directories(my_target
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
)
```

### target\_link\_options

设置链接选项：

```cmake
target_link_options(my_target
    PRIVATE
        -Wl,--as-needed
        $<$<PLATFORM_ID:Windows>:/SUBSYSTEM:CONSOLE>
)
```

### target\_sources

添加源文件到目标：

```cmake
add_executable(my_app main.cpp)
target_sources(my_app
    PRIVATE
        utils.cpp
        helper.cpp
)
```

***

## 变量操作命令

### set

设置变量：

```cmake
# 设置普通变量
set(MY_VAR "value")

# 设置列表
set(MY_LIST item1 item2 item3)

# 设置缓存变量
set(MY_CACHE_VAR "default" CACHE STRING "Description")

# 设置环境变量
set(ENV{MY_ENV} "value")
```

### unset

取消变量：

```cmake
# 取消普通变量
unset(MY_VAR)

# 取消缓存变量
unset(MY_VAR CACHE)
```

### list

列表操作：

```cmake
set(MY_LIST a b c)

# 添加元素
list(APPEND MY_LIST d e)

# 插入元素
list(INSERT MY_LIST 0 x)

# 删除元素
list(REMOVE_ITEM MY_LIST b)

# 删除重复元素
list(REMOVE_DUPLICATES MY_LIST)

# 列表长度
list(LENGTH MY_LIST len)

# 获取元素
list(GET MY_LIST 0 first_item)

# 连接为字符串
list(JOIN MY_LIST "," result)
```

### option

定义选项：

```cmake
option(ENABLE_TESTS "Enable unit tests" ON)
option(USE_OPENMP "Use OpenMP" OFF)

if(ENABLE_TESTS)
    message(STATUS "Tests enabled")
endif()
```

***

## 条件与循环命令

### if / elseif / else / endif

条件判断：

```cmake
# 基本用法
if(ENABLE_FEATURE)
    message(STATUS "Feature enabled")
endif()

# 多条件
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release build")
else()
    message(STATUS "Other build type")
endif()

# 逻辑运算
if(ENABLE_A AND ENABLE_B)
    message(STATUS "Both enabled")
endif()

if(ENABLE_A OR ENABLE_B)
    message(STATUS "At least one enabled")
endif()

if(NOT ENABLE_A)
    message(STATUS "A is disabled")
endif()

# 比较运算
if(${VERSION} VERSION_GREATER "1.0.0")
    message(STATUS "Version > 1.0.0")
endif()

if(${COUNT} LESS 10)
    message(STATUS "Count < 10")
endif()

# 检查变量定义
if(DEFINED MY_VAR)
    message(STATUS "MY_VAR is defined")
endif()

# 检查目标存在
if(TARGET my_target)
    message(STATUS "Target exists")
endif()
```

### foreach

循环遍历：

```cmake
# 遍历列表
set(SOURCES a.cpp b.cpp c.cpp)
foreach(SRC ${SOURCES})
    message(STATUS "Source: ${SRC}")
endforeach()

# 遍历范围
foreach(i RANGE 0 9)
    message(STATUS "Number: ${i}")
endforeach()

# 遍历步长
foreach(i RANGE 0 10 2)
    message(STATUS "Even: ${i}")
endforeach()

# 同时遍历多个列表
set(NAMES a b c)
set(VALUES 1 2 3)
foreach(NAME VALUE IN ZIP_LISTS NAMES VALUES)
    message(STATUS "${NAME} = ${VALUE}")
endforeach()
```

### while

条件循环：

```cmake
set(COUNT 0)
while(COUNT LESS 5)
    message(STATUS "Count: ${COUNT}")
    math(EXPR COUNT "${COUNT} + 1")
endwhile()
```

***

## 文件操作命令

### file

文件系统操作：

```cmake
# 读取文件
file(READ "input.txt" CONTENT)

# 写入文件
file(WRITE "output.txt" "Hello, World!")

# 追加内容
file(APPEND "log.txt" "New log entry\n")

# 复制文件
file(COPY "source.txt" DESTINATION "dest/")

# 复制文件（如果不同）
file(COPY_FILE "source.txt" "dest.txt")

# 删除文件
file(REMOVE "temp.txt")

# 创建目录
file(MAKE_DIRECTORY "output")

# 获取文件名
file(GLOB SOURCES "src/*.cpp")

# 递归获取文件
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 获取文件名（不含路径）
file(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")

# 下载文件
file(DOWNLOAD "https://example.com/file.txt" "local.txt")

# 计算文件哈希
file(SHA256 "file.txt" HASH)
```

***

## 查找命令

### find\_package

> 💡 **比喻**：这就像去建材市场找材料。你要告诉CMake"帮我找某某品牌的某某材料"，它会帮你搜索系统中是否已安装这个库，如果找到了就告诉你怎么使用它。

查找外部包：

```cmake
# 基本用法
find_package(Boost REQUIRED)

# 指定版本
find_package(Boost 1.70 REQUIRED)

# 查找特定组件
find_package(Boost REQUIRED COMPONENTS filesystem system)

# 使用查找到的包
if(Boost_FOUND)
    target_link_libraries(my_app PRIVATE Boost::filesystem Boost::system)
endif()

# QUIET 模式（不显示找不到的消息）
find_package(OpenCV QUIET)

# 可选包
find_package(Qt6 COMPONENTS Core Widgets)
```

### find\_library

查找库文件：

```cmake
find_library(MY_LIB
    NAMES mylib mylib64
    PATHS /usr/local/lib /usr/lib
)

if(MY_LIB)
    target_link_libraries(my_app PRIVATE ${MY_LIB})
endif()
```

### find\_path

查找包含目录：

```cmake
find_path(MY_INCLUDE_DIR
    NAMES mylib.h
    PATHS /usr/local/include /usr/include
)

if(MY_INCLUDE_DIR)
    target_include_directories(my_app PRIVATE ${MY_INCLUDE_DIR})
endif()
```

### find\_file

查找文件：

```cmake
find_file(CONFIG_FILE
    NAMES config.ini
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/config
)
```

### find\_program

查找可执行程序：

```cmake
find_program(PYTHON_EXECUTABLE
    NAMES python3 python
    PATHS /usr/bin /usr/local/bin
)
```

***

## 安装命令

### install

> 💡 **比喻**：这就像装修完成后的"家具摆放指南"。你得告诉CMake：房子盖好后，门把手装在哪（可执行文件→bin）、沙发搬到哪里（库文件→lib）、说明书放到哪个抽屉（头文件→include）。

定义安装规则：

```cmake
# 安装可执行文件
install(TARGETS my_app
    RUNTIME DESTINATION bin
)

# 安装库
install(TARGETS my_lib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# 安装头文件
install(FILES mylib.h
    DESTINATION include
)

# 安装目录
install(DIRECTORY include/
    DESTINATION include
)

# 安装脚本
install(SCRIPT cmake/install_script.cmake)

# 安装导出目标
install(TARGETS my_lib
    EXPORT my_lib-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(EXPORT my_lib-targets
    FILE my_lib-config.cmake
    NAMESPACE MyLib::
    DESTINATION lib/cmake/my_lib
)
```

***

## 其他常用命令

### message

输出消息：

```cmake
# 状态消息
message(STATUS "Configuring project...")

# 警告消息
message(WARNING "This is a warning")

# 错误消息（继续执行）
message(SEND_ERROR "This is an error")

# 致命错误（停止执行）
message(FATAL_ERROR "This is a fatal error")

# 调试消息
message("Simple message")
```

### add\_subdirectory

> 💡 **比喻**：这就像在说"把那个房间的设计图也加进来"。你的项目可能有很多房间（子目录），每个房间有自己的CMakeLists.txt，你需要用这个命令把它们都包含进来。

添加子目录：

```cmake
add_subdirectory(src)
add_subdirectory(lib)
```

### include

> 💡 **比喻**：这就像把别人的"工具箱"直接拿过来用。你写了一些可重用的CMake函数放在别的文件里，用include就能把它们加载到当前脚本中。

包含其他 CMake 文件：

```cmake
include(cmake/MyFunctions.cmake)
include(GNUInstallDirs)
```

### execute\_process

> 💡 **比喻**：这就像在施工过程中让工人去跑腿——"去仓库拿一下材料"、"去问一下设计师这个尺寸对不对"。这个命令让CMake可以在配置阶段执行外部程序。

执行外部命令：

```cmake
execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
```

### add\_custom\_command

添加自定义构建命令：

```cmake
# 生成文件
add_custom_command(
    OUTPUT generated.h
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/generate.py
    DEPENDS generate.py
    COMMENT "Generating header file"
)

# 构建后执行
add_custom_command(
    TARGET my_app
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/config.ini
        ${CMAKE_CURRENT_BINARY_DIR}/config.ini
)
```

***

## 小结

CMake 命令分类：

- **项目配置**：`cmake_minimum_required`、`project`
- **目标定义**：`add_executable`、`add_library`、`add_custom_target`
- **目标属性**：`target_*` 系列命令
- **变量操作**：`set`、`unset`、`list`、`option`
- **条件循环**：`if`、`foreach`、`while`
- **文件操作**：`file`
- **查找命令**：`find_*` 系列命令
- **安装命令**：`install`

掌握这些命令，可以应对大部分 CMake 项目需求！
