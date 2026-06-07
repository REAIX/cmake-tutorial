# CMake 常用命令参考

> 📂 **对应示例**：本节内容对应示例代码 [examples/variables/](../../examples/variables/) 和 [examples/targets/](../../examples/targets/)，建议结合示例代码一起学习。

## 项目配置命令

### cmake\_minimum\_required

> 💡 **比喻**：这就像餐厅门口挂的"本店厨师至少 5 年经验"——不是歧视新人，而是有些菜（新特性）只有老师傅才做得出来。版本太低？对不起，这道菜做不了。

设置 CMake 的最低版本要求：

```cmake
cmake_minimum_required(VERSION 3.20)

# 设置版本范围（CMake 3.12+ 支持）
# 含义：最低需要 3.15，最高测试到 4.0
cmake_minimum_required(VERSION 3.15...4.0)
```

> ⚠️ **CMake 4.0 注意**：CMake 4.0 不再兼容 3.5 之前的版本。如果 `cmake_minimum_required(VERSION)` 设置低于 3.5，CMake 4.0+ 会报错。建议始终设置版本范围的上限为你测试过的最高版本。

### project

> 💡 **比喻**：这就像给新生儿上户口——起名字、登记出生日期（版本号）、报国籍（语言）。没上户口的项目是"黑户"，CMake 不认。

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

---

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

> 💡 **比喻**：这是"创建预制模块"的命令。你要告诉 CMake：要什么类型的模块——静态库像"浇筑好的混凝土"（搬不动但结实）、动态库像"乐高积木"（随时拆装）、头文件库像"纯图纸"（只有说明书没有实物）、对象库像"切好的菜"（半成品，等下锅）。

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

---

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

---

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

---

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

---

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

---

## 查找命令

### find\_package

> 💡 **比喻**：这就像去建材市场找材料——你喊一嗓子"我要东鹏瓷砖！"，CMake 就满城帮你找。找到了告诉你仓库地址和提货方式（导入目标），找不到就看你有没有写 REQUIRED——写了就罢工，没写就默默跳过。

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

#### find\_package 的两种工作模式

> 💡 **比喻**：找材料有两种方式——Module 模式就像请了个"采购员"（FindXXX.cmake），他满大街跑，知道去哪个市场找、怎么验货，但每个采购员的报告格式不一样；Config 模式就像供应商直接递了张"供货清单"（XXXConfig.cmake），上面明码标价、仓库地址一清二楚，靠谱多了。

**Module 模式**（优先使用）：

CMake 在 `CMAKE_MODULE_PATH` 中搜索 `Find<Package>.cmake` 脚本文件。这些脚本是 CMake 或项目自己编写的"查找逻辑"。

```cmake
# CMake 自带的 Find 模块（部分列表）
# FindBoost.cmake, FindOpenCV.cmake, FindProtobuf.cmake 等

# 指定额外的 Find 模块搜索路径
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(MyCustomLib REQUIRED)
```

Module 模式的特点：
- 查找逻辑由 `Find<Package>.cmake` 脚本定义
- 不同包的变量命名可能不一致（如 `MYLIB_FOUND`、`MyLib_FOUND`）
- CMake 逐步淘汰内置 Find 模块，推荐使用 Config 模式

**Config 模式**（更可靠）：

CMake 搜索 `<Package>Config.cmake` 或 `<package>-config.cmake` 文件。这些文件由库的安装过程生成，包含精确的目标信息。

```cmake
# Config 模式搜索路径（按顺序）：
# 1. <Package>_DIR（缓存变量）
# 2. CMAKE_PREFIX_PATH
# 3. 系统标准路径（/usr, /usr/local 等）

# 指定 Config 文件路径
cmake -DMyLib_DIR=/path/to/lib/cmake/MyLib ..

# 或使用 CMAKE_PREFIX_PATH
cmake -DCMAKE_PREFIX_PATH=/path/to/install ..
```

Config 模式的特点：
- 由库的安装过程自动生成，信息准确
- 提供现代 CMake 导入目标（如 `MyLib::mylib`）
- 变量命名统一规范

**两种模式的选择顺序**：

```
find_package(MyLib)
    ↓
先尝试 Module 模式（查找 FindMyLib.cmake）
    ↓ 找不到
回退到 Config 模式（查找 MyLibConfig.cmake）
    ↓ 找不到
报错（如果指定了 REQUIRED）
```

```cmake
# 强制使用 Config 模式
find_package(MyLib CONFIG REQUIRED)

# 强制使用 Module 模式
find_package(MyLib MODULE REQUIRED)
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

---

## 安装命令

### install

> 💡 **比喻**：这就像搬家后的"物品归位指南"——你得告诉 CMake：电视放客厅（可执行文件→bin）、书柜放书房（库文件→lib）、说明书塞抽屉（头文件→include）。不写的话，东西全堆门口，你自己翻。

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

---

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

> 💡 **比喻**：这就像说"把隔壁那栋楼的设计图也拿来"——你的项目可能有很多栋楼（子目录），每栋楼有自己的 CMakeLists.txt，这个命令就是把它们都纳入统一规划。

添加子目录：

```cmake
add_subdirectory(src)
add_subdirectory(lib)
```

### include

> 💡 **比喻**：这就像借邻居的工具箱——你写了一些可复用的 CMake 函数放在别的文件里，`include` 就是"拿来主义"，直接搬过来用，不用重新造轮子。

包含其他 CMake 文件：

```cmake
include(cmake/MyFunctions.cmake)
include(GNUInstallDirs)
```

### execute\_process

> 💡 **比喻**：这就像施工时派工人去跑腿——"去仓库查一下这批钢筋的批次号！"（执行 git 命令查版本）、"去隔壁工地借个工具！"（运行脚本获取信息）。配置阶段的"跑腿小弟"，帮你搞定 CMake 自己做不了的事。

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

### cmake\_parse\_arguments

> 💡 **比喻**：这就像快递分拣员——一堆包裹（参数）扔过来，分拣员按标签分类：大件放这边（多值参数）、小件放那边（单值参数）、易碎品贴标签（布尔选项），整整齐齐码好等你来取。

解析函数/宏的参数（CMake 3.5+）：

```cmake
# 定义函数时使用
function(my_install target)
    # 定义可选参数名（单值）
    set(options OPTIONAL FAST)
    # 定义单值关键字参数
    set(oneValueArgs DESTINATION RENAME)
    # 定义多值关键字参数
    set(multiValueArgs TARGETS CONFIGS)

    # 解析参数
    cmake_parse_arguments(
        MY_INSTALL              # 前缀，生成的变量以 MY_INSTALL_ 开头
        "${options}"            # 布尔选项
        "${oneValueArgs}"       # 单值参数
        "${multiValueArgs}"     # 多值参数
        ${ARGN}                 # 传入的参数
    )

    # 使用解析后的参数
    if(MY_INSTALL_OPTIONAL)
        message(STATUS "Optional install")
    endif()

    message(STATUS "Destination: ${MY_INSTALL_DESTINATION}")
    message(STATUS "Targets: ${MY_INSTALL_TARGETS}")
endfunction()

# 调用函数
my_install(my_target
    DESTINATION bin
    TARGETS app1 app2
    OPTIONAL
)
# 结果：
# MY_INSTALL_OPTIONAL = TRUE
# MY_INSTALL_DESTINATION = "bin"
# MY_INSTALL_TARGETS = "app1;app2"
# MY_INSTALL_FAST = FALSE
# MY_INSTALL_RENAME = 未定义
```

### string

字符串操作：

```cmake
# 拼接
set(FULL_NAME "${FIRST_NAME} ${LAST_NAME}")

# 字符串替换
string(REPLACE "old" "new" OUTPUT "old value old")  # OUTPUT = "new value new"

# 正则替换
string(REGEX REPLACE "[0-9]+" "N" OUTPUT "abc123def456")  # OUTPUT = "abcNdefN"

# 正则匹配
string(REGEX MATCH "[0-9]+" VERSION_NUM "v2.5.1")  # VERSION_NUM = "2"

# 查找子串
string(FIND "Hello World" "World" POS)  # POS = 6

# 字符串长度
string(LENGTH "Hello" LEN)  # LEN = 5

# 子串
string(SUBSTRING "Hello World" 0 5 RESULT)  # RESULT = "Hello"

# 大小写转换
string(TOUPPER "hello" UPPER)    # UPPER = "HELLO"
string(TOLOWER "HELLO" LOWER)    # LOWER = "hello"

# 去除空白
string(STRIP "  hello  " RESULT)  # RESULT = "hello"

# 比较
string(COMPARE EQUAL "a" "a" RESULT)    # RESULT = TRUE
string(COMPARE NOTEQUAL "a" "b" RESULT) # RESULT = TRUE
```

---

## 小结

CMake 命令分类：

- **项目配置**：`cmake_minimum_required`、`project`
- **目标定义**：`add_executable`、`add_library`、`add_custom_target`
- **目标属性**：`target_*` 系列命令
- **变量操作**：`set`、`unset`、`list`、`option`、`string`
- **条件循环**：`if`、`foreach`、`while`
- **文件操作**：`file`
- **查找命令**：`find_*` 系列命令
- **安装命令**：`install`
- **其他常用命令**：`message`、`add_subdirectory`、`include`、`execute_process`、`add_custom_command`、`cmake_parse_arguments`

掌握这些命令，可以应对大部分 CMake 项目需求！

---

> 📖 **相关阅读**：
> - [FAQ 常见问题](../faq/README.md) - 命令使用中的常见问题
> - [05-CMake进阶技巧.md](../practices/05-CMake进阶技巧.md) - 更高级的用法和技巧
