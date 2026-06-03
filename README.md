# cmake-tutorial - CMake 学习教程项目

## 项目简介

cmake-tutorial 是一个渐进式的 CMake 学习项目，通过 9 个精心设计的示例模块，帮助您从零开始掌握现代 CMake 的核心概念和最佳实践。

## 项目结构

```
cmake-tutorial/
├── CMakeLists.txt              # 主 CMake 配置文件
├── README.md                   # 项目说明文档
│
├── examples/                   # 示例代码
│   ├── basic/                  # 模块1：CMake 基础
│   │   ├── CMakeLists.txt
│   │   └── main.cpp
│   ├── variables/              # 模块2：变量与条件判断
│   │   ├── CMakeLists.txt
│   │   └── main.cpp
│   ├── targets/                # 模块3：目标管理（静态库 + 动态库）
│   │   ├── CMakeLists.txt
│   │   ├── src/
│   │   │   └── main.cpp
│   │   └── lib/
│   │       ├── math.h / math.cpp
│   │       └── string_utils.h / string_utils.cpp
│   ├── modules/                # 模块4：多模块项目
│   │   ├── CMakeLists.txt
│   │   ├── app/
│   │   │   └── main.cpp
│   │   ├── math/
│   │   │   ├── CMakeLists.txt
│   │   │   ├── math.h
│   │   │   └── math.cpp
│   │   └── utils/
│   │       ├── CMakeLists.txt
│   │       ├── utils.h
│   │       └── utils.cpp
│   ├── advanced/               # 模块5：高级特性（代码生成 + Git 集成 + 打包）
│   │   ├── CMakeLists.txt
│   │   ├── generate_build_info.cmake
│   │   └── main.cpp
│   ├── compiler-options/       # 模块6：编译器选项
│   │   ├── CMakeLists.txt
│   │   └── main.cpp
│   ├── testing/                # 模块7：测试（CTest）
│   │   ├── CMakeLists.txt
│   │   ├── main.cpp
│   │   ├── math.h / math.cpp
│   │   ├── test_add.cpp
│   │   ├── test_multiply.cpp
│   │   └── test_factorial.cpp
│   └── fetchcontent/           # 模块8：依赖管理（FetchContent）
│       ├── CMakeLists.txt
│       └── main.cpp
│   └── header-only/           # 模块9：Header-Only 库 & configure_file
│       ├── CMakeLists.txt
│       ├── config.h.in
│       ├── include/
│       │   └── format_lib.h
│       └── main.cpp
│
└── docs/                       # 文档
    ├── basics/                 # 基础知识
    │   ├── 01-CMake基础概念.md
    │   └── 02-CMake常用命令参考.md
    ├── practices/              # 实践指南
    │   ├── 03-CMake最佳实践.md
    │   └── 05-CMake进阶技巧.md
    ├── tools/                  # 工具与IDE
    │   ├── 06-CMake与VSCode搭配使用.md
    │   ├── 07-CMake开发Qt6指南.md
    │   ├── 08-CMake与CI-CD集成.md
    │   └── 09-CMake与IDE集成.md
    └── faq/                    # 常见问题（按主题拆分）
        ├── README.md           # FAQ 索引
        ├── beginner-零基础入门.md  # 🌱 新手必看！
        ├── build-配置与构建.md
        ├── build-编译器选择.md
        ├── deps-依赖管理.md
        ├── files-路径与文件.md
        ├── compile-编译与链接.md
        ├── platform-跨平台.md
        ├── install-安装与打包.md
        ├── debug-调试技巧.md
        └── perf-性能优化.md
```

## 学习路径

### 模块 01：CMake 基础
**学习目标**：理解 CMake 的基本工作流程

**核心概念**：
- `cmake_minimum_required()` - 设置 CMake 最低版本
- `project()` - 定义项目名称和版本
- `add_executable()` - 创建可执行目标

**构建命令**：
```bash
cd examples/basic
mkdir build && cd build
cmake ..
cmake --build .
./bin/basic_example
```

---

### 模块 02：变量与条件判断
**学习目标**：掌握 CMake 变量系统和条件控制

**核心概念**：
- `set()` - 定义变量
- Cache 变量 - 持久化配置
- `if()/elseif()/else()` - 条件判断
- `target_compile_definitions()` - 传递变量到 C++ 代码

**构建命令**：
```bash
cd examples/variables
mkdir build && cd build
cmake -DENABLE_FEATURE_A=ON -DENABLE_FEATURE_B=OFF ..
cmake --build .
./bin/variables_example
```

---

### 模块 03：目标管理
**学习目标**：理解现代 CMake 的目标导向设计，掌握静态库与动态库

**核心概念**：
- `add_library(STATIC)` - 创建静态库
- `add_library(SHARED)` - 创建动态库
- `target_include_directories()` - 设置包含路径
  - `PUBLIC` - 对当前目标和消费者可见
  - `PRIVATE` - 仅对当前目标可见
  - `INTERFACE` - 仅对消费者可见
- `target_link_libraries()` - 链接库
- `CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS` - Windows DLL 导出

**构建命令**：
```bash
cd examples/targets
mkdir build && cd build
cmake ..
cmake --build .
./bin/targets_example
```

---

### 模块 04：多模块项目
**学习目标**：掌握大型项目的模块化组织

**核心概念**：
- `add_subdirectory()` - 添加子目录
- 每个子目录独立的 CMakeLists.txt
- 模块间的依赖管理
- 属性的自动传播

**构建命令**：
```bash
cd examples/modules
mkdir build && cd build
cmake ..
cmake --build .
./bin/modules_example
```

---

### 模块 05：高级特性
**学习目标**：掌握 CMake 的高级功能

**核心概念**：
- `add_custom_command()` - 自定义构建命令（代码生成）
- `add_custom_target()` - 自定义目标
- `find_package()` - 查找外部包
- `install()` - 安装规则
- CPack - 打包分发

**构建命令**：
```bash
cd examples/advanced
mkdir build && cd build
cmake ..
cmake --build .
./bin/advanced_example

# 安装
cmake --install . --prefix ./install

# 打包
cpack -G ZIP
```

---

### 模块 06：编译器选项
**学习目标**：掌握跨编译器的编译选项配置

**核心概念**：
- `target_compile_options()` - 设置编译选项
- 生成器表达式 - `$<$<CXX_COMPILER_ID:MSVC>:...>`
- 编译器检测 - `CMAKE_CXX_COMPILER_ID`
- 构建类型特定选项 - Debug/Release

**构建命令**：
```bash
cd examples/compiler-options
mkdir build && cd build
cmake ..
cmake --build .
./bin/compiler_options_example
```

---

### 模块 07：测试（CTest）
**学习目标**：掌握 CMake/CTest 测试框架

**核心概念**：
- `enable_testing()` - 启用 CTest 支持
- `add_test()` - 注册测试用例
- `ctest` 命令 - 运行测试
- 测试过滤 - `-R` 正则匹配
- Google Test 集成 - `USE_GTEST` 选项启用 GTest

**构建命令**：
```bash
cd examples/testing
mkdir build && cd build
cmake ..
cmake --build .

# 运行所有测试
ctest --output-on-failure

# 并行运行测试
ctest -j4

# 运行特定测试
ctest -R test_add

# 启用 Google Test（需要网络下载 GTest）
cmake -DUSE_GTEST=ON ..
cmake --build .
ctest --output-on-failure
```

---

### 模块 08：依赖管理（FetchContent）
**学习目标**：掌握现代 CMake 的依赖管理方式

**核心概念**：
- `FetchContent_Declare()` - 声明外部依赖
- `FetchContent_MakeAvailable()` - 获取并构建依赖
- `GIT_REPOSITORY` / `GIT_TAG` - 指定仓库和版本
- FetchContent vs find_package 的区别

**构建命令**：
```bash
cd examples/fetchcontent
mkdir build && cd build
cmake -DBUILD_FETCHCONTENT=ON ..
cmake --build .
./bin/fetchcontent_example
```

---

### 模块 09：Header-Only 库 & configure_file
**学习目标**：掌握 Header-Only 库的创建方式与 configure_file 配置生成

**核心概念**：
- `add_library(... INTERFACE)` - 创建 Header-Only 库目标
- `target_include_directories(... INTERFACE)` - 仅对消费者可见的包含路径
- `target_compile_features(... INTERFACE)` - 传播编译特性给消费者
- `configure_file()` - 从模板生成配置头文件
  - `@VAR@` - 替换为 CMake 变量的值
  - `#cmakedefine` - 根据变量是否定义生成 `#define` 或 `/* #undef */`
- INTERFACE 可见性：Header-Only 库没有自身编译，所有属性必须使用 INTERFACE

**构建命令**：
```bash
cd examples/header-only
mkdir build && cd build
cmake ..
cmake --build .
./bin/header_only_example

# 启用调试输出
cmake -DENABLE_DEBUG_OUTPUT=ON ..
cmake --build .
./bin/header_only_example
```

---

## 快速开始

### 环境要求
- **CMake**: >= 3.20（推荐 4.0+）
- **C++ 编译器**: 支持 C++20 标准
  - GCC >= 10
  - Clang >= 10
  - MSVC >= 19.28

### 构建整个项目

```bash
# 1. 进入项目根目录
cd cmake-tutorial

# 2. 创建构建目录
mkdir build && cd build

# 3. 配置项目
cmake ..

# 4. 构建项目
cmake --build .

# 5. 运行示例
./bin/basic_example
./bin/variables_example
./bin/targets_example
./bin/modules_example
./bin/advanced_example
./bin/compiler_options_example
./bin/testing_example
./bin/header_only_example

# 6. 运行测试
ctest --output-on-failure
```

### 构建特定模块

```bash
# 只构建某个模块
cd examples/basic
mkdir build && cd build
cmake ..
cmake --build .
```

### 常用 CMake 选项

```bash
# 指定构建类型
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake -DCMAKE_BUILD_TYPE=Release ..

# 指定编译器
cmake -DCMAKE_CXX_COMPILER=g++ ..
cmake -DCMAKE_CXX_COMPILER=clang++ ..

# 指定安装路径
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..

# 禁用示例构建
cmake -DBUILD_EXAMPLES=OFF ..

# 启用 FetchContent 示例（需要网络）
cmake -DBUILD_FETCHCONTENT=ON ..
```

---

## CMake 最佳实践

### 1. 使用现代 CMake（3.20+，推荐 4.0+）
- 避免使用过时的命令（如 `link_directories`、`include_directories`）
- 使用目标导向的命令（`target_*` 系列）

### 2. 目标属性优于全局变量
```cmake
# ❌ 不推荐
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

# ✅ 推荐
target_include_directories(my_target PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_directories(my_target PRIVATE ${PROJECT_SOURCE_DIR}/lib)
```

### 3. 明确指定属性可见性
```cmake
# PUBLIC: 当前目标和消费者都需要
target_include_directories(my_lib PUBLIC include/)

# PRIVATE: 仅当前目标需要
target_include_directories(my_exe PRIVATE src/)

# INTERFACE: 仅消费者需要
target_include_directories(my_header_only INTERFACE include/)
```

### 4. 使用生成器表达式
```cmake
target_compile_definitions(my_target PRIVATE
    $<$<CONFIG:Debug>:DEBUG_MODE>
    $<$<CONFIG:Release>:NDEBUG>
)
```

### 5. 避免全局修改编译标志
```cmake
# ❌ 不推荐
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# ✅ 推荐
target_compile_options(my_target PRIVATE -Wall -Wextra -Wpedantic)
```

### 6. 利用属性自动传播
```cmake
# 库设置 PUBLIC cxx_std_20，链接它的目标自动继承
target_compile_features(my_lib PUBLIC cxx_std_20)
# 下游目标无需再重复设置
```

---

## 常见问题

> 📖 完整 FAQ 请查看 [docs/faq/README.md](docs/faq/README.md)

### Q1: 如何查看 CMake 的详细输出？
```bash
cmake -DCMAKE_VERBOSE_MAKEFILE=ON ..
# 或
cmake --build . -- VERBOSE=1
```

### Q2: 如何清理构建？
```bash
# 删除构建目录
rm -rf build
mkdir build && cd build
cmake ..
```

### Q3: 如何查看所有可用的目标？
```bash
cmake --build . --target help
```

### Q4: 如何并行构建？
```bash
cmake --build . -- -j4  # 使用 4 个并行任务
# 或
cmake --build . --parallel 4
```

### Q5: 如何导出编译命令（用于 IDE/工具）？
```bash
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
# 生成 compile_commands.json 文件
```

---

## 学习资源

### 官方文档
- [CMake 官方文档](https://cmake.org/documentation/)
- [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)

### 推荐书籍
- 《Professional CMake: A Practical Guide》 - Craig Scott
- 《CMake Cookbook》 - Radovan Bast

### 在线资源
- [Modern CMake](https://cliutils.gitlab.io/modern-cmake/)
- [Effective CMake](https://www.youtube.com/watch?v=bsXLMQ6Wg-I) (视频)

---

## 项目特性

✅ **C++20 标准** - 使用最新的 C++ 特性  
✅ **现代 CMake** - 遵循最佳实践  
✅ **渐进式学习** - 从简单到复杂  
✅ **完整注释** - 每个示例都有详细说明  
✅ **跨平台** - 支持 Windows/Linux/macOS  
✅ **模块化设计** - 清晰的项目结构  
✅ **测试支持** - CTest 集成  
✅ **依赖管理** - FetchContent 示例  

---

## 许可证

本项目仅供学习使用。

---

## 作者

CMakeLearn 项目 - 帮助开发者快速掌握 CMake 构建系统
