# CMake 进阶技巧

## 1. 自定义 CMake 模块

### 创建可重用的 CMake 模块

> 💡 **比喻**：这就像把你的"装修秘籍"整理成一本手册，下次装修时可以直接翻看，不用每次都重新想一遍怎么做。

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

> 💡 **比喻**：这就像在印名片时，先做一个模板（.in文件），然后填入具体的人名、职位等信息，生成正式的名片。CMake可以帮你做这个"填表"的工作。

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

> 💡 **比喻**：这就像你要去外国盖房子，需要了解当地用什么工具、什么材料标准。工具链文件就是告诉CMake"在某某国家盖房子需要用这些特殊工具"的说明书。

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

> 💡 **比喻**：这就像保存不同的"施工方案"。你可以有"快速方案"（Debug模式）、"精装方案"（Release模式）、"特殊方案"（针对MSVC编译器）。每次施工时选一个方案就不用每次都手动配置了。

创建 `CMakePresets.json`：

```json
{
    "version": 3,
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
    ]
}
```

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

> 💡 **比喻**：这就像让装修公司直接去供应商的仓库提货，而不是你先自己去买回来。FetchContent可以自动帮你下载和管理第三方库。

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

### 自定义 FetchContent

```cmake
FetchContent_Declare(
    external_lib
    GIT_REPOSITORY https://github.com/user/external_lib.git
    GIT_TAG main
)

FetchContent_GetProperties(external_lib)
if(NOT external_lib_POPULATED)
    FetchContent_Populate(external_lib)
    
    # 自定义配置
    set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
    
    # 添加子目录
    add_subdirectory(${external_lib_SOURCE_DIR} ${external_lib_BINARY_DIR})
endif()
```

---

## 7. 使用 ExternalProject

### 构建外部项目

> 💡 **比喻**：这就像你不仅在装修自己的房子，还让装修队帮你把邻居家的车库也一起翻修了。ExternalProject可以让你在构建自己的项目时，顺便把依赖的外部项目也构建一遍。

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

> 💡 **比喻**：这就像你有一个记忆力超强的助手。你上次用砖头砌了一面墙，这次还要砌类似的墙，助手说"我记的上次你是怎么砌的，直接用那个方法吧"，不用从头再学一次。ccache就是这样的缓存工具。

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
- **依赖管理** - FetchContent 和 ExternalProject
- **测试配置** - CTest 高级功能
- **性能优化** - 预编译头、Unity 构建、ccache
- **调试分析** - 编译数据库和构建分析

掌握这些进阶技巧，可以构建出更加专业、高效的 CMake 项目！
