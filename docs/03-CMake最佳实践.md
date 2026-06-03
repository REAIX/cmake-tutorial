# CMake 最佳实践

> 📂 **对应示例**：本节内容对应示例代码 [examples/modules/](../examples/modules/) 和 [examples/targets/](../examples/targets/)，建议结合示例代码一起学习。

## 1. 使用现代 CMake

### ✅ 推荐：目标导向的 CMake

> 💡 **比喻**：老式 CMake 就像村长拿大喇叭喊"全村人注意了！都给我装防盗门！"——不管你家需不需要，都得装。现代 CMake 就像快递小哥，精确投递："张三家，防盗门；李四家，监控摄像头。"谁需要什么，一清二楚。

现代 CMake（3.x）采用目标导向的设计，所有属性都通过目标设置：

```cmake
# 定义目标
add_library(my_lib lib.cpp)

# 设置目标属性
target_include_directories(my_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(my_lib PUBLIC cxx_std_20)

target_compile_options(my_lib
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
```

### ❌ 避免：全局命令

> 💡 **比喻**：这就像公司老板群发邮件"全体员工必须穿西装！"——连保洁阿姨和程序员都得穿，荒谬且浪费。全局命令就是这种"一刀切"的霸道做法。

避免使用影响全局状态的命令：

```cmake
# ❌ 不推荐：全局包含路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# ❌ 不推荐：全局链接目录
link_directories(${PROJECT_SOURCE_DIR}/lib)

# ❌ 不推荐：全局编译定义
add_definitions(-DDEBUG)

# ❌ 不推荐：全局编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
```

**原因**：全局命令会影响所有目标，难以追踪依赖关系，容易导致冲突。

---

## 2. 正确使用可见性

### PUBLIC - 对外接口

> 💡 **比喻**：就像产品的"使用说明书"——你卖冰箱，说明书得给客户看，不然人家不会用。

```cmake
# 库的头文件路径应该是 PUBLIC
add_library(my_lib lib.cpp)
target_include_directories(my_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include  # 用户需要包含这个路径
)
```

### PRIVATE - 内部实现

> 💡 **比喻**：就像公司的"内部机密"——配方是商业秘密，客户不需要知道可乐到底加了什么，只要好喝就行。

```cmake
# 内部依赖应该是 PRIVATE
target_include_directories(my_lib
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src      # 仅内部使用
)

target_link_libraries(my_lib
    PRIVATE
        internal_lib                         # 仅内部使用
)
```

### INTERFACE - 仅对消费者

> 💡 **比喻**：就像"美食评论家"——自己不下厨（没有源文件），但知道哪家餐厅好吃（头文件路径），还能给你推荐（传播给消费者）。

```cmake
# 头文件库使用 INTERFACE
add_library(header_only_lib INTERFACE)
target_include_directories(header_only_lib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
```

### 可见性传播示例

```cmake
# 库 A
add_library(lib_a a.cpp)
target_include_directories(lib_a
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include_a  # 用户需要
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src_a     # 内部使用
)

# 库 B 依赖 A
add_library(lib_b b.cpp)
target_include_directories(lib_b
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include_b
)
target_link_libraries(lib_b
    PUBLIC lib_a  # PUBLIC: lib_b 的用户也能访问 lib_a
)

# 可执行文件
add_executable(my_app main.cpp)
target_link_libraries(my_app
    PRIVATE lib_b  # my_app 自动获得：
                   # - lib_b 的 PUBLIC 包含路径
                   # - lib_a 的 PUBLIC 包含路径
)
```

---

## 3. 项目结构最佳实践

### 推荐的项目结构

```
my_project/
├── CMakeLists.txt              # 根配置文件
├── cmake/                      # CMake 模块
│   ├── FindMyLib.cmake
│   └── MyFunctions.cmake
├── include/                    # 公共头文件
│   └── mylib/
│       └── mylib.h
├── src/                        # 源文件
│   ├── CMakeLists.txt
│   ├── mylib.cpp
│   └── internal/
│       └── helper.h
├── tests/                      # 测试代码
│   ├── CMakeLists.txt
│   └── test_mylib.cpp
├── examples/                   # 示例代码
│   ├── CMakeLists.txt
│   └── example.cpp
└── docs/                       # 文档
    └── README.md
```

### 根 CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.20)

project(MyProject
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "My awesome project"
)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 选项
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_SHARED_LIBS "Build shared library" OFF)

# 添加子目录
add_subdirectory(src)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# 安装配置
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# 打包
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
include(CPack)
```

---

## 4. 源文件管理

### ✅ 推荐：显式列出源文件

> 💡 **比喻**：这就像列一份详细的购物清单——去超市前写好"鸡蛋、牛奶、面包"，缺啥一目了然。虽然写清单麻烦，但不会买错东西回家。

```cmake
add_library(my_lib
    src/lib.cpp
    src/helper.cpp
    src/utils.cpp
)
```

**优点**：
- 构建系统可以正确追踪依赖
- IDE 可以正确显示项目结构
- 添加/删除文件需要明确操作

### ⚠️ 谨慎使用：GLOB 自动获取

> 💡 **比喻**：这就像让扫地机器人自动打扫——方便是方便，但它不会告诉你桌底下多了只袜子（新文件），也不会告诉你沙发下面少了只拖鞋（删了文件），直到你踩到才知道出事了。

```cmake
file(GLOB SOURCES "src/*.cpp")
add_library(my_lib ${SOURCES})
```

**缺点**：
- 添加新文件后需要重新运行 CMake
- 删除文件可能导致构建错误
- IDE 可能无法正确显示项目结构

**如果使用 GLOB，建议添加提示**：

```cmake
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(my_lib ${SOURCES})

# 提示用户
message(STATUS "Using GLOB for sources. Remember to re-run cmake if files change.")
```

---

## 5. 编译选项管理

### 平台特定的编译选项

```cmake
target_compile_options(my_target
    PRIVATE
        # GCC/Clang 选项
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:
            -Wall -Wextra -Wpedantic -Werror
            -Wno-unused-parameter
        >
        
        # MSVC 选项
        $<$<CXX_COMPILER_ID:MSVC>:
            /W4
            /utf-8
            /WX
            /wd4100  # 禁用特定警告
        >
)
```

### 构建类型特定的选项

```cmake
target_compile_options(my_target
    PRIVATE
        # Debug 模式
        $<$<CONFIG:Debug>:
            -g
            -O0
            -fsanitize=address,undefined
        >
        
        # Release 模式
        $<$<CONFIG:Release>:
            -O3
            -DNDEBUG
        >
)
```

### 链接选项

```cmake
target_link_options(my_target
    PRIVATE
        # Debug 模式链接 sanitizer
        $<$<CONFIG:Debug>:-fsanitize=address,undefined>
        
        # Windows 特定
        $<$<PLATFORM_ID:Windows>:/SUBSYSTEM:CONSOLE>
)
```

---

## 6. 依赖管理

### 查找和使用外部库

```cmake
# 查找包
find_package(Boost 1.70 REQUIRED COMPONENTS filesystem system)

# 使用现代 CMake 目标
target_link_libraries(my_app
    PRIVATE
        Boost::filesystem
        Boost::system
)

# 查找可选包
find_package(Qt6 COMPONENTS Core Widgets)

if(Qt6_FOUND)
    target_link_libraries(my_app PRIVATE Qt6::Core Qt6::Widgets)
    target_compile_definitions(my_app PRIVATE USE_QT)
endif()
```

### 导出库供他人使用

```cmake
# 定义库
add_library(my_lib src/lib.cpp)

target_include_directories(my_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 安装目标
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

# 导出配置
install(EXPORT my_lib-targets
    FILE my_lib-targets.cmake
    NAMESPACE MyLib::
    DESTINATION lib/cmake/my_lib
)

# 生成配置文件
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/my_lib-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/my_lib-config.cmake
    INSTALL_DESTINATION lib/cmake/my_lib
)

# 生成版本文件
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

# 包含导出的目标
include("${CMAKE_CURRENT_LIST_DIR}/my_lib-targets.cmake")

# 检查请求的组件是否都找到了
# 如果用户写 find_package(MyLib REQUIRED COMPONENTS xxx)，
# check_required_components 会验证 xxx 组件是否存在
check_required_components(my_lib)

# 可选：检查依赖是否可用
# include(CMakeFindDependencyMacro)
# find_dependency(Boost REQUIRED COMPONENTS filesystem)
```

> 💡 **说明**：
> - `@PACKAGE_INIT@` 是 CMake 提供的特殊占位符，会被替换为初始化代码，包括设置 `PACKAGE_PREFIX_DIR` 等变量
> - `check_required_components()` 用于验证 `find_package` 中 `COMPONENTS` 列出的组件是否都可用
> - 如果你的库依赖其他库，使用 `find_dependency()` 而非 `find_package()`，这样找不到依赖时会正确传播错误

---

## 7. 测试集成

### 使用 CTest

```cmake
enable_testing()

add_executable(test_mylib tests/test_mylib.cpp)
target_link_libraries(test_mylib PRIVATE my_lib)

add_test(NAME test_mylib COMMAND test_mylib)

# 添加测试属性
set_tests_properties(test_mylib PROPERTIES
    TIMEOUT 10
    LABELS "unit"
)

# 条件测试
if(ENABLE_HEAVY_TESTS)
    add_test(NAME heavy_test COMMAND heavy_test_exe)
endif()
```

### 使用 Google Test

```cmake
find_package(GTest REQUIRED)

add_executable(test_mylib tests/test_mylib.cpp)
target_link_libraries(test_mylib
    PRIVATE
        my_lib
        GTest::gtest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_mylib)
```

---

## 8. 安装与打包

### 标准安装路径

```cmake
include(GNUInstallDirs)

install(TARGETS my_lib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES include/mylib.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
```

### 使用 CPack 打包

```cmake
set(CPACK_PACKAGE_NAME "MyLib")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "My awesome library")
set(CPACK_PACKAGE_VENDOR "My Company")
set(CPACK_PACKAGE_CONTACT "support@example.com")

# 生成器
set(CPACK_GENERATOR "ZIP;TGZ")

# DEB 包（Linux）
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++6")

# RPM 包（Linux）
set(CPACK_RPM_PACKAGE_REQUIRES "libstdc++")

include(CPack)
```

---

## 9. 避免常见错误

### 错误 1：滥用全局变量

```cmake
# ❌ 错误
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# ✅ 正确
target_compile_options(my_target PRIVATE -Wall)
```

### 错误 2：错误使用 include_directories

```cmake
# ❌ 错误
include_directories(${PROJECT_SOURCE_DIR}/include)

# ✅ 正确
target_include_directories(my_target
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
```

### 错误 3：硬编码路径

```cmake
# ❌ 错误
target_include_directories(my_target PRIVATE /home/user/project/include)

# ✅ 正确
target_include_directories(my_target
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)
```

### 错误 4：忽略可见性

```cmake
# ❌ 错误：所有依赖都是 PUBLIC
target_link_libraries(my_lib PUBLIC dep1 dep2 dep3)

# ✅ 正确：区分 PUBLIC 和 PRIVATE
target_link_libraries(my_lib
    PUBLIC dep1      # 用户需要
    PRIVATE dep2 dep3  # 仅内部使用
)
```

### 错误 5：不设置 C++ 标准

```cmake
# ❌ 错误：依赖编译器默认
add_executable(my_app main.cpp)

# ✅ 正确：明确设置标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 或使用 target_compile_features
target_compile_features(my_app PRIVATE cxx_std_20)
```

---

## 10. 性能优化

> 💡 **提示**：本节介绍最佳实践层面的性能优化。更详细的配置选项和高级用法（如预编译头重用、Unity 构建排除文件、ccache 集成等）请参考 [05-CMake进阶技巧.md](./05-CMake进阶技巧.md) 的"性能优化技巧"章节。

### 使用预编译头

> 💡 **比喻**：这就像厨师提前把葱姜蒜切好放碗里——每次炒菜直接抓一把，不用临时再切。预编译头就是把最常用的"配料"提前准备好，编译时直接下锅。

```cmake
target_precompile_headers(my_target
    PRIVATE
        <vector>
        <string>
        <iostream>
)
```

### 使用 Unity 构建

> 💡 **比喻**：这就像搬家时把零碎东西全塞进一个大箱子——虽然找东西不方便，但搬运效率翻倍。Unity Build 就是把一堆小源文件合并成大文件编译，速度嗖嗖的。

```cmake
set_target_properties(my_target PROPERTIES
    UNITY_BUILD ON
    UNITY_BUILD_BATCH_SIZE 8
)
```

### 并行编译

> 💡 **比喻**：这就像请了一队装修工人——一个人刷墙要一周，八个人一起干一天搞定。`--parallel 8` 就是雇了 8 个工人同时开工。

```bash
cmake --build . --parallel 8
# 或
cmake --build . -- -j8
```

---

## 小结

现代 CMake 最佳实践的核心原则：

1. **目标导向** - 使用 `target_*` 命令，避免全局命令
2. **明确可见性** - 正确使用 PUBLIC/PRIVATE/INTERFACE
3. **显式依赖** - 显式列出源文件和依赖
4. **平台无关** - 使用生成器表达式处理平台差异
5. **模块化设计** - 合理组织项目结构
6. **可移植性** - 避免硬编码路径
7. **可维护性** - 清晰的注释和文档

遵循这些实践，可以构建出高质量、可维护的 CMake 项目！
