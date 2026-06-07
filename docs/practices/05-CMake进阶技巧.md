# CMake 进阶技巧

> 📂 **对应示例**：本节内容对应示例代码 [examples/advanced/](../../examples/advanced/) 和 [examples/fetchcontent/](../../examples/fetchcontent/)，建议结合示例代码一起学习。

## 1. 自定义 CMake 模块

### 创建可重用的 CMake 模块

> 💡 **比喻**：这就像把你的"祖传菜谱"整理成一本小册子——下次做菜直接翻，不用每次都打电话问老妈"那个红烧肉放几颗八角来着？"

在 `cmake/` 目录下创建自定义模块：

```cmake
# cmake/MyFunctions.cmake

# 定义函数：添加编译警告
function(add_warnings target)
    target_compile_options(${target}
        PRIVATE
            $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic>
            $<$<CXX_COMPILER_ID:MSVC>:/W4>
    )
endfunction()

# 定义函数：设置目标输出目录
function(set_output_directories target)
    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )
endfunction()

# 定义函数：创建可执行文件并设置标准属性
function(add_executable_with_defaults target)
    cmake_parse_arguments(ARG "" "" "SOURCES" ${ARGN})
    
    add_executable(${target} ${ARG_SOURCES})
    
    target_compile_features(${target} PRIVATE cxx_std_20)
    add_warnings(${target})
    set_output_directories(${target})
endfunction()
```

使用自定义模块：

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(MyProject)

# 包含自定义模块
include(cmake/MyFunctions.cmake)

# 使用自定义函数
add_executable_with_defaults(my_app
    SOURCES main.cpp utils.cpp
)
```

---

## 2. 代码生成

### 使用 configure_file 生成配置文件

> 💡 **比喻**：这就像批量印名片——先做个模板"姓名：___，职位：___"，然后填入张三李四的信息，咔咔印出来。`.in` 文件就是名片模板，`configure_file` 就是那台印名片机。

创建模板文件 `config.h.in`：

```c
// config.h.in
#ifndef CONFIG_H
#define CONFIG_H

#define PROJECT_NAME "@PROJECT_NAME@"
#define PROJECT_VERSION "@PROJECT_VERSION@"
#define PROJECT_VERSION_MAJOR @PROJECT_VERSION_MAJOR@
#define PROJECT_VERSION_MINOR @PROJECT_VERSION_MINOR@
#define PROJECT_VERSION_PATCH @PROJECT_VERSION_PATCH@

#cmakedefine ENABLE_FEATURE_A
#cmakedefine ENABLE_FEATURE_B

#define DATA_PATH "@CMAKE_INSTALL_PREFIX@/share/@PROJECT_NAME@"

#endif // CONFIG_H
```

在 CMakeLists.txt 中配置：

```cmake
# 设置选项
option(ENABLE_FEATURE_A "Enable feature A" ON)
option(ENABLE_FEATURE_B "Enable feature B" OFF)

# 生成配置文件
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
    @ONLY  # 只替换 @VAR@ 格式的变量
)

# 包含生成的头文件
target_include_directories(my_target
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)
```

---

### 使用自定义命令生成代码

```cmake
# 使用 Python 生成代码
find_package(Python3 REQUIRED COMPONENTS Interpreter)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated.cpp
    COMMAND ${Python3_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/generate.py
        --output ${CMAKE_CURRENT_BINARY_DIR}/generated.cpp
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/generate.py
    COMMENT "Generating C++ code from Python script"
    VERBATIM
)

# 添加生成的源文件到目标
add_executable(my_app
    main.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/generated.cpp
)
```

---

### 使用 protoc 生成 Protocol Buffers 代码

```cmake
find_package(Protobuf REQUIRED)

# 生成 C++ 代码
add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/message.pb.cc
        ${CMAKE_CURRENT_BINARY_DIR}/message.pb.h
    COMMAND ${Protobuf_PROTOC_EXECUTABLE}
        --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
        --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/proto
        ${CMAKE_CURRENT_SOURCE_DIR}/proto/message.proto
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/proto/message.proto
    COMMENT "Generating Protocol Buffers C++ code"
    VERBATIM
)

# 添加生成的源文件
add_library(my_proto_lib
    ${CMAKE_CURRENT_BINARY_DIR}/message.pb.cc
)

target_include_directories(my_proto_lib
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(my_proto_lib
    PUBLIC ${Protobuf_LIBRARIES}
)
```

---

## 3. 跨平台构建配置

### 使用工具链文件

> 💡 **比喻**：这就像你要去外国盖房子——当地用英制螺丝刀（ARM 编译器），你带的是公制（x86 编译器），拧不上！工具链文件就是一份"出国施工指南"，告诉 CMake："到了那边用这把螺丝刀、买这种螺丝、按他们的图纸来。"

创建 `cmake/toolchains/arm-linux.cmake`：

```cmake
# ARM Linux 交叉编译工具链
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# 指定编译器
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

# 指定 sysroot
set(CMAKE_SYSROOT /path/to/arm/sysroot)

# 指定查找路径
set(CMAKE_FIND_ROOT_PATH /path/to/arm/sysroot)

# 调整查找行为
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
```

使用工具链文件：

```bash
cmake -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/arm-linux.cmake ..
```

---

### 使用预设（Presets）

> 💡 **比喻**：这就像保存不同的"游戏存档"——"休闲模式"（Debug，慢慢玩）、"速通模式"（Release，追求最快）、"地狱难度"（MSVC，Windows 限定挑战）。选个存档就能直接开玩，不用每次重新调设置。

创建 `CMakePresets.json`：

```json
{
    "version": 6,
    "configurePresets": [
        {
            "name": "default",
            "displayName": "Default Config",
            "binaryDir": "${sourceDir}/build",
            "cacheVariables": {
                "CMAKE_BUILD_TYPE": "Release",
                "CMAKE_CXX_STANDARD": "20"
            }
        },
        {
            "name": "debug",
            "displayName": "Debug Config",
            "inherits": "default",
            "cacheVariables": {
                "CMAKE_BUILD_TYPE": "Debug"
            }
        },
        {
            "name": "release",
            "displayName": "Release Config",
            "inherits": "default",
            "cacheVariables": {
                "CMAKE_BUILD_TYPE": "Release"
            }
        },
        {
            "name": "msvc",
            "displayName": "MSVC Config",
            "inherits": "default",
            "generator": "Visual Studio 17 2022",
            "cacheVariables": {
                "CMAKE_CXX_COMPILER": "cl"
            }
        }
    ],
    "buildPresets": [
        {
            "name": "debug",
            "configurePreset": "debug"
        },
        {
            "name": "release",
            "configurePreset": "release"
        }
    ],
    "testPresets": [
        {
            "name": "debug",
            "configurePreset": "debug",
            "output": {
                "outputOnFailure": true
            }
        }
    ]
}
```

> 💡 **提示**：`version` 字段建议使用 6（CMake 3.25+ 支持），它支持 `configurePresets`、`buildPresets`、`testPresets`、`packagePresets` 和 `workflowPresets`。version 2（CMake 3.14+）已支持 `testPresets`，version 6（CMake 3.25+）新增了 `packagePresets` 和 `workflowPresets`。CMake 4.0+ 推荐使用 version 9（支持更多功能）。

使用预设：

```bash
# 列出所有预设
cmake --list-presets

# 使用预设配置
cmake --preset debug

# 使用预设构建
cmake --build --preset debug
```

---

## 4. 高级目标属性

### 设置目标属性

```cmake
add_executable(my_app main.cpp)

# 设置输出名称
set_target_properties(my_app PROPERTIES
    OUTPUT_NAME "my_application"
)

# 设置版本信息
set_target_properties(my_app PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1  # 兼容版本
)

# 设置 C++ 标准
set_target_properties(my_app PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# 设置位置无关代码（用于共享库）
set_target_properties(my_lib PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# 设置自动生成 MOC/UIC/RCC（Qt）
set_target_properties(my_qt_app PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# 设置文件夹（IDE 组织）
set_target_properties(my_app PROPERTIES
    FOLDER "Applications"
)
set_target_properties(my_lib PROPERTIES
    FOLDER "Libraries"
)
```

---

### 使用自定义属性

```cmake
# 定义自定义属性
define_property(TARGET PROPERTY MY_CUSTOM_PROPERTY
    BRIEF_DOCS "A custom property"
    FULL_DOCS "A custom property for demonstration"
)

# 设置自定义属性
set_target_properties(my_target PROPERTIES
    MY_CUSTOM_PROPERTY "custom_value"
)

# 获取自定义属性
get_target_property(CUSTOM_VALUE my_target MY_CUSTOM_PROPERTY)
message(STATUS "Custom property: ${CUSTOM_VALUE}")
```

---

## 5. 高级安装配置

### 安装导出目标

```cmake
# 创建库
add_library(my_lib src/lib.cpp)

target_include_directories(my_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 安装目标并导出
install(TARGETS my_lib
    EXPORT my_lib-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# 安装头文件
install(DIRECTORY include/
    DESTINATION include
)

# 导出目标
install(EXPORT my_lib-targets
    FILE my_lib-targets.cmake
    NAMESPACE MyLib::
    DESTINATION lib/cmake/my_lib
)

# 创建配置文件
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/my_lib-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/my_lib-config.cmake
    INSTALL_DESTINATION lib/cmake/my_lib
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/my_lib-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/my_lib-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/my_lib-config-version.cmake
    DESTINATION lib/cmake/my_lib
)
```

配置文件模板 `cmake/my_lib-config.cmake.in`：

```cmake
@PACKAGE_INIT@

include("${CMAKE_CURRENT_LIST_DIR}/my_lib-targets.cmake")

check_required_components(my_lib)
```

---

### 安装时执行脚本

```cmake
# 安装时运行脚本
install(SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/cmake/post_install.cmake)

# 或使用代码
install(CODE "
    message(STATUS \"Running post-install script\")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E echo \"Installation complete\"
    )
")
```

---

## 6. 使用 FetchContent 管理依赖

### 从 Git 仓库获取依赖

> 💡 **比喻**：这就像点外卖——你不用亲自去菜市场买食材、不用自己炒菜，手机点一下（`FetchContent_Declare`），骑手就把做好的菜（编译好的库）送上门。懒人福音！

```cmake
include(FetchContent)

# 声明依赖
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

# 获取并添加依赖
FetchContent_MakeAvailable(googletest)

# 使用依赖
enable_testing()
add_executable(test_mylib tests/test_mylib.cpp)
target_link_libraries(test_mylib PRIVATE GTest::gtest_main)
```

### 从 URL 获取依赖

```cmake
FetchContent_Declare(
    my_dependency
    URL https://example.com/mylib-1.0.0.tar.gz
    URL_HASH SHA256=abc123...
)

FetchContent_MakeAvailable(my_dependency)
```

### 自定义 FetchContent 配置

> ⚠️ **注意**：`FetchContent_Populate` 在 CMake 3.28+ 中已被弃用，推荐使用 `FetchContent_MakeAvailable` 配合选项覆盖。

```cmake
# 推荐方式：使用 FetchContent_MakeAvailable + 选项覆盖
FetchContent_Declare(
    external_lib
    GIT_REPOSITORY https://github.com/user/external_lib.git
    GIT_TAG v1.0.0
)

# 在 MakeAvailable 之前设置选项，控制依赖的构建行为
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(external_lib)

# 使用依赖
target_link_libraries(my_app PRIVATE external_lib::external_lib)
```

### FetchContent 与 find_package 配合

```cmake
# 方式 1：优先使用系统安装的包，找不到时自动下载
# 使用 OVERRIDE_FIND_PACKAGE 让 FetchContent 覆盖 find_package
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.2.1
    OVERRIDE_FIND_PACKAGE  # CMake 3.24+：让 find_package(fmt) 使用 FetchContent
)

# 优先查找系统安装，找不到则自动下载
find_package(fmt REQUIRED)
target_link_libraries(my_app PRIVATE fmt::fmt)

# 方式 2：通过选项控制是否下载
option(FETCH_DEPS "Fetch dependencies instead of finding them" OFF)

if(FETCH_DEPS)
    FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.2.1
    )
    FetchContent_MakeAvailable(fmt)
else()
    find_package(fmt REQUIRED)
endif()

target_link_libraries(my_app PRIVATE fmt::fmt)
```

### 离线使用 FetchContent

```cmake
# 如果依赖已经下载到本地，可以指定源目录
# 设置环境变量或 CMake 变量覆盖下载位置
# FETCHCONTENT_SOURCE_DIR_<uppercase name> 优先于下载

# 示例：使用本地 fmt 源码
# cmake -DFETCHCONTENT_SOURCE_DIR_FMT=/path/to/fmt ..

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.2.1
)

# 如果设置了 FETCHCONTENT_SOURCE_DIR_FMT，则不会下载，直接使用本地源码
FetchContent_MakeAvailable(fmt)
```

---

## 7. 使用 ExternalProject

### 构建外部项目

> 💡 **比喻**：这就像你不仅装修自己的房子，还让装修队去隔壁小区帮邻居盖了一栋楼——完全独立施工，用他们自己的工具和材料，盖好了把钥匙（库文件）给你就行。

```cmake
include(ExternalProject)

ExternalProject_Add(external_lib
    GIT_REPOSITORY https://github.com/user/external_lib.git
    GIT_TAG v1.0.0
    
    # 配置步骤
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DBUILD_SHARED_LIBS=OFF
    
    # 构建步骤
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --config ${CMAKE_BUILD_TYPE}
    
    # 安装步骤
    INSTALL_COMMAND ${CMAKE_COMMAND} --install . --config ${CMAKE_BUILD_TYPE}
    
    # 日志
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_INSTALL ON
)
```

### 使用外部项目的产物

```cmake
ExternalProject_Add(external_lib
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/lib
    INSTALL_DIR ${CMAKE_BINARY_DIR}/external_install
    
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
)

# 导入外部库
add_library(lib_external STATIC IMPORTED)
set_target_properties(lib_external PROPERTIES
    IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/external_install/lib/libexternal.a
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_BINARY_DIR}/external_install/include
)

# 添加依赖关系
add_dependencies(lib_external external_lib)
```

### FetchContent vs ExternalProject 如何选择？

> 💡 **比喻**：FetchContent 就像"把供应商的工人请到你的工地上干活"——他们用你的工具、在你的管理下工作；ExternalProject 就像"让供应商在自己的工厂里生产，然后把成品运给你"——他们用自己的工具、按自己的方式工作。

| 对比项 | FetchContent | ExternalProject |
|--------|-------------|-----------------|
| **执行时机** | 配置阶段 | 构建阶段 |
| **依赖方式** | `add_subdirectory` | 独立构建安装 |
| **目标可见性** | 直接可用 | 需要手动导入 IMPORTED 目标 |
| **构建系统** | 必须是 CMake 项目 | 可以是任意构建系统 |
| **编译器/选项** | 与主项目一致 | 可以独立配置 |
| **适用场景** | CMake 项目的源码依赖 | 非 CMake 项目或需要隔离构建 |

**选择建议**：

```cmake
# ✅ 使用 FetchContent 的场景：
# 1. 依赖是 CMake 项目，且你想直接使用它的目标
# 2. 需要在配置阶段就知道依赖的详细信息
# 3. 希望依赖使用与主项目相同的编译器和选项

include(FetchContent)
FetchContent_Declare(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.2.1
)
FetchContent_MakeAvailable(fmt)
target_link_libraries(my_app PRIVATE fmt::fmt)  # 直接使用目标

# ✅ 使用 ExternalProject 的场景：
# 1. 依赖不是 CMake 项目（如 Makefile、Autotools 项目）
# 2. 需要与主项目使用不同的编译器或构建选项
# 3. 需要完全隔离的构建过程
# 4. 需要自定义构建步骤（如打补丁）

include(ExternalProject)
ExternalProject_Add(protobuf
    GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
    GIT_TAG v25.0
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
)
```

---

## 8. 高级测试配置

### 使用 CTest 高级功能

```cmake
enable_testing()

# 添加测试
add_executable(test_mylib tests/test_mylib.cpp)
target_link_libraries(test_mylib PRIVATE my_lib GTest::gtest_main)

add_test(NAME test_mylib COMMAND test_mylib)

# 设置测试属性
set_tests_properties(test_mylib PROPERTIES
    TIMEOUT 10
    LABELS "unit"
    WILL_FAIL FALSE
)

# 添加测试夹具
add_test(NAME setup COMMAND setup_script)
set_tests_properties(setup PROPERTIES FIXTURES_SETUP my_fixture)

add_test(NAME cleanup COMMAND cleanup_script)
set_tests_properties(cleanup PROPERTIES FIXTURES_CLEANUP my_fixture)

add_test(NAME test_with_fixture COMMAND test_exe)
set_tests_properties(test_with_fixture PROPERTIES FIXTURES_REQUIRED my_fixture)

# 并行测试
set_tests_properties(test_mylib PROPERTIES
    PROCESSORS 4
    RUN_SERIAL FALSE
)

# 环境变量
set_tests_properties(test_mylib PROPERTIES
    ENVIRONMENT "MY_VAR=value;PATH=/custom/path"
)

# 条件测试
if(ENABLE_HEAVY_TESTS)
    add_test(NAME heavy_test COMMAND heavy_test_exe)
    set_tests_properties(heavy_test PROPERTIES
        LABELS "heavy"
        TIMEOUT 300
    )
endif()
```

### 使用测试标签

```cmake
# 添加带标签的测试
add_test(NAME unit_test1 COMMAND test1)
set_tests_properties(unit_test1 PROPERTIES LABELS "unit;quick")

add_test(NAME unit_test2 COMMAND test2)
set_tests_properties(unit_test2 PROPERTIES LABELS "unit;slow")

add_test(NAME integration_test COMMAND test_integration)
set_tests_properties(integration_test PROPERTIES LABELS "integration;slow")

# 运行特定标签的测试
# ctest -L unit       # 运行所有 unit 标签的测试
# ctest -L quick      # 运行所有 quick 标签的测试
# ctest -LE slow      # 排除 slow 标签的测试
```

---

## 9. 性能优化技巧

### 使用预编译头

```cmake
# 创建预编译头
target_precompile_headers(my_target
    PRIVATE
        <vector>
        <string>
        <map>
        <memory>
        <iostream>
)

# 重用预编译头
target_precompile_headers(another_target
    REUSE_FROM my_target
)
```

### 使用 Unity 构建

```cmake
# 启用 Unity 构建
set_target_properties(my_target PROPERTIES
    UNITY_BUILD ON
    UNITY_BUILD_BATCH_SIZE 8
)

# 排除特定文件
set_source_files_properties(special.cpp PROPERTIES
    SKIP_UNITY_BUILD_INCLUSION ON
)
```

### 使用 ccache

> 💡 **比喻**：ccache 就像一个"过目不忘的厨师"——上次做过的菜，他记住了所有步骤和火候，下次同样的菜直接出盘，不用重新看菜谱。编译过的代码再次编译？秒出！

```cmake
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()
```

---

## 10. 调试与分析

### 导出编译数据库

```cmake
# 生成 compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 或使用命令行
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
```

### 使用 CMake 分析工具

```bash
# 分析构建时间
cmake --time-trace ..

# 输出详细的配置过程
cmake --debug-output ..

# 跟踪执行过程
cmake --trace ..
cmake --trace-source=CMakeLists.txt ..
```

### 自定义构建信息

```cmake
# 生成构建信息头文件
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/build_info.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/build_info.h
)

# build_info.h.in
#cmakedefine BUILD_TYPE "@CMAKE_BUILD_TYPE@"
#cmakedefine BUILD_TIME "@CMAKE_CURRENT_TIMESTAMP@"
#cmakedefine CMAKE_VERSION "@CMAKE_VERSION@"
#cmakedefine COMPILER_ID "@CMAKE_CXX_COMPILER_ID@"
#cmakedefine COMPILER_VERSION "@CMAKE_CXX_COMPILER_VERSION@"
```

---

## 小结

CMake 进阶技巧包括：
- **自定义模块** - 创建可重用的 CMake 函数和宏
- **代码生成** - 使用 configure_file 和自定义命令
- **跨平台配置** - 工具链文件和预设
- **高级目标属性** - 自定义属性和高级配置
- **安装配置** - 导出目标和安装脚本
- **依赖管理** - FetchContent（推荐）、ExternalProject 及其选型
- **测试配置** - CTest 高级功能
- **性能优化** - 预编译头、Unity 构建、ccache
- **调试分析** - 编译数据库和构建分析

掌握这些进阶技巧，可以构建出更加专业、高效的 CMake 项目！

---

> 📖 **相关阅读**：
> - [FAQ 常见问题](../faq/README.md) - 常见问题排查
> - [08-CMake与CI-CD集成.md](../tools/08-CMake与CI-CD集成.md) - CI/CD 中的 CMake 配置
> - [09-CMake与IDE集成.md](../tools/09-CMake与IDE集成.md) - IDE 中的 CMake 配置
