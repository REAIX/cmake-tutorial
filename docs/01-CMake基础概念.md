# CMake 基础概念

> 📂 **对应示例**：本节内容对应示例代码 [examples/basic/](../examples/basic/)，建议结合示例代码一起学习。

## 什么是 CMake？

CMake 是一个跨平台的构建系统生成工具，它不直接构建项目，而是生成平台特定的构建文件（如 Makefile、Visual Studio 项目文件等）。

### CMake 的工作流程

```
CMakeLists.txt → CMake → 构建文件 → 本地构建工具 → 可执行文件/库
     (配置)              (生成)       (编译链接)
```

### 核心优势

1. **跨平台** - 支持 Windows、Linux、macOS
2. **编译器无关** - 支持 GCC、Clang、MSVC 等多种编译器
3. **IDE 集成** - 支持 Visual Studio、CLion、VSCode 等
4. **依赖管理** - 方便查找和管理外部库
5. **现代 C++ 支持** - 完整支持 C++11/14/17/20/23

---

## CMake 的基本概念

### 1. 目标（Target）

目标是 CMake 构建系统的核心概念，主要有两种类型：

> 💡 **比喻**：想象你在盖一座房子，"目标"就是你要建造的东西——可以是一栋完整的房子（可执行文件），也可以是预制好的房间模块（库）。房子需要由这些组件组合而成，CMake 正是管理这些组件的工具。

#### 可执行目标（Executable）
```cmake
add_executable(my_app main.cpp)
```

#### 库目标（Library）
```cmake
# 静态库 - 就像一次性浇筑好的混凝土块，直接嵌入房子里
add_library(my_lib STATIC lib.cpp)

# 动态库 - 就像可以拆卸的预制房间，建好后可以单独替换
add_library(my_lib SHARED lib.cpp)

# 头文件库（header-only）- 就像装修指南，只有说明书，不需要实际材料
add_library(my_lib INTERFACE)
```

### 2. 属性（Properties）

每个目标都有自己的属性，控制编译、链接等行为：

> 💡 **比喻**：如果说目标是"房子"，那么"属性"就是房子的各种配置——门的大小、窗户的位置、墙的颜色等。不同房子有不同的配置，CMake允许你为每个目标单独设置这些属性。

```cmake
# 包含目录 - 就像告诉建筑工人："需要的材料在哪个仓库"
target_include_directories(my_target 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 编译定义 - 就像给工人发的任务清单："这次要开启节能模式"
target_compile_definitions(my_target
    PRIVATE DEBUG_MODE
)

# 编译选项 - 就像建筑规范："必须用什么样的砖头"
target_compile_options(my_target
    PRIVATE -Wall -Wextra
)

# 链接库 - 就像把预制房间组装到房子里
target_link_libraries(my_target
    PRIVATE other_lib
)
```

### 3. 可见性（Visibility）

CMake 有三种属性可见性：

> 💡 **比喻**：这就像装修公司与业主的关系。PRIVATE（隐私）= 这是装修公司自己的内部资料，不给业主看；PUBLIC（公开）= 这是给业主看的使用说明书；INTERFACE（接口）= 这只是给业主看的安装指南，但装修公司自己不用。

| 可见性 | 含义 | 使用场景 |
|--------|------|----------|
| **PUBLIC** | 当前目标和消费者都可见 | 库的头文件路径 |
| **PRIVATE** | 仅当前目标可见 | 实现细节、内部依赖 |
| **INTERFACE** | 仅消费者可见 | 头文件库的属性 |

#### 示例
```cmake
# 库目标
add_library(my_lib lib.cpp)

# PUBLIC: my_lib 和链接 my_lib 的目标都需要这个路径
target_include_directories(my_lib PUBLIC include/)

# PRIVATE: 只有 my_lib 需要
target_compile_definitions(my_lib PRIVATE INTERNAL_FEATURE)

# 可执行目标
add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE my_lib)
# my_app 自动获得 my_lib 的 PUBLIC 属性
```

---

## CMake 变量

### 1. 普通变量

> 💡 **比喻**：普通变量就像记在便签纸上的临时メモ，关机就没了。下次运行CMake时，这些值不会保留。

```cmake
# 设置变量
set(MY_VAR "Hello")
set(MY_LIST item1 item2 item3)

# 使用变量
message(STATUS "MY_VAR = ${MY_VAR}")
```

### 2. 缓存变量（Cache Variables）

> 💡 **比喻**：缓存变量就像刻在石头上的记录，会永久保存下来。即使关闭电脑，下次打开CMake时还能看到这些值。这就是为什么CMakeCache.txt文件很重要的原因。

缓存变量会持久化到 CMakeCache.txt 文件中：

```cmake
# 设置缓存变量
set(MY_CACHE_VAR "default_value" CACHE STRING "Description")

# 用户可以通过命令行覆盖
# cmake -DMY_CACHE_VAR="custom_value" ..
```

### 3. 环境变量

> 💡 **比喻**：环境变量就像你电脑的系统设置——PATH、HOME等。它们影响全局，CMake可以读取和修改。

```cmake
# 访问环境变量
set(ENV_VAR $ENV{PATH})

# 设置环境变量
set(ENV{MY_ENV} "value")
```

### 4. 内置变量

CMake 提供了许多内置变量：

| 变量 | 说明 |
|------|------|
| `CMAKE_SOURCE_DIR` | 项目根目录 |
| `CMAKE_BINARY_DIR` | 构建目录 |
| `CMAKE_CURRENT_SOURCE_DIR` | 当前 CMakeLists.txt 所在目录 |
| `CMAKE_CURRENT_BINARY_DIR` | 当前构建目录 |
| `CMAKE_INSTALL_PREFIX` | 安装路径前缀 |
| `CMAKE_BUILD_TYPE` | 构建类型（Debug/Release） |
| `CMAKE_CXX_STANDARD` | C++ 标准版本 |
| `PROJECT_NAME` | 项目名称 |
| `PROJECT_VERSION` | 项目版本 |

---

## 构建类型

CMake 支持多种构建类型：

> 💡 **比喻**：这就像装修的不同级别。Debug = 毛坯房+所有监控摄像头（方便调试）；Release = 精装修（追求完美效果）；RelWithDebInfo = 精装修但保留了监控录像（出了问题还能回看）；MinSizeRel = 简约装修（节省空间为主）。

```cmake
# 设置构建类型
set(CMAKE_BUILD_TYPE Release)

# 可选值：
# - Debug: 调试版本，包含调试信息，无优化
# - Release: 发布版本，完全优化
# - RelWithDebInfo: 发布版本 + 调试信息
# - MinSizeRel: 最小尺寸发布版本
```

### 不同构建类型的编译选项

| 构建类型 | GCC/Clang | MSVC |
|---------|-----------|------|
| Debug | `-O0 -g` | `/Od /Zi` |
| Release | `-O3 -DNDEBUG` | `/O2 /DNDEBUG` |
| RelWithDebInfo | `-O2 -g -DNDEBUG` | `/O2 /Zi /DNDEBUG` |
| MinSizeRel | `-Os -DNDEBUG` | `/O1 /DNDEBUG` |

---

## 生成器表达式

生成器表达式允许在构建时（而非配置时）求值：

> 💡 **比喻**：想象你在写一份"未来指令"。配置时CMake只是读懂了你的指令，但具体执行要等到真正盖房子的时候。比如"如果业主是中国人，就用中文说明书；如果是美国人，就用英文说明书"——这就是生成器表达式在做的事情：条件判断被推迟到了真正构建的那一刻。

```cmake
# 根据构建类型设置不同的定义
target_compile_definitions(my_target PRIVATE
    $<$<CONFIG:Debug>:DEBUG_MODE>
    $<$<CONFIG:Release>:NDEBUG>
)

# 根据平台设置不同的选项
target_compile_options(my_target PRIVATE
    $<$<PLATFORM_ID:Windows>:/W4>
    $<$<PLATFORM_ID:Linux>:-Wall -Wextra>
)

# 根据编译器设置不同的选项
target_compile_options(my_target PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-fdiagnostics-color=always>
    $<$<CXX_COMPILER_ID:MSVC>:/utf-8>
)
```

### 常用生成器表达式

| 表达式 | 说明 |
|--------|------|
| `$<CONFIG:cfg>` | 当前构建类型是否为 cfg |
| `$<PLATFORM_ID:id>` | 当前平台是否为 id |
| `$<CXX_COMPILER_ID:id>` | 当前 C++ 编译器是否为 id |
| `$<BOOL:value>` | 值是否为真 |
| `$<IF:cond,true,false>` | 条件表达式 |
| `$<TARGET_PROPERTY:prop>` | 目标属性值 |

---

## 主流编译器介绍

> 💡 **比喻**：编译器就像"翻译员"，把你的C++代码翻译成机器能读懂的语言。不同的翻译员有不同的方言和习惯，你需要用不同的方式与他们沟通。

### GCC (GNU Compiler Collection)

- **平台**：Linux、macOS、Windows (MinGW)
- **特点**：开源免费，Linux默认编译器
- **编译器ID**：`GNU`
- **典型选项**：
  - `-Wall -Wextra`：开启详细警告
  - `-O2`：优化级别
  - `-g`：调试信息
  - `-std=c++20`：C++标准

### Clang

- **平台**：Linux、macOS、Windows
- **特点**：LLVM项目的一部分，诊断信息清晰，与GCC兼容性好
- **编译器ID**：`Clang`（在Apple平台上可能是`AppleClang`）
- **典型选项**：与GCC基本兼容

### MSVC (Microsoft Visual C++)

- **平台**：仅Windows
- **特点**：Visual Studio自带，Windows平台原生支持
- **编译器ID**：`MSVC`
- **典型选项**：
  - `/W4`：警告级别4
  - `/O2`：优化
  - `/Zi`：调试信息
  - `/utf-8`：UTF-8编码

### MinGW (Minimalist GNU for Windows)

- **平台**：Windows
- **特点**：Windows上的GCC移植，与Linux代码兼容性最好
- **编译器ID**：`GNU`（与GCC相同）
- **生成的库后缀**：`.a`（静态）、`.dll`（动态）

### 编译器检测

CMake 可以自动检测当前使用的编译器：

```cmake
# 检测编译器ID
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C Compiler ID: ${CMAKE_C_COMPILER_ID}")

# 常见编译器ID值
# - GNU: GCC 或 MinGW
# - Clang: Clang
# - AppleClang: macOS上的Clang
# - MSVC: Visual Studio
```

### 在 CMake 中指定编译器

```bash
# 指定 GCC
cmake -DCMAKE_CXX_COMPILER=g++ ..
cmake -DCMAKE_C_COMPILER=gcc ..

# 指定 Clang
cmake -DCMAKE_CXX_COMPILER=clang++ ..
cmake -DCMAKE_C_COMPILER=clang ..

# 指定 MSVC（通过Visual Studio生成器）
cmake -G "Visual Studio 17 2022" ..

# 指定 MinGW
cmake -G "MinGW Makefiles" ..
cmake -DCMAKE_CXX_COMPILER=g++ ..
```

### 编译器特定的编译选项对比

| 功能 | GCC/Clang | MSVC | 说明 |
|------|-----------|------|------|
| 警告级别 | `-Wall -Wextra` | `/W4` | 开启更多警告 |
| 优化 | `-O2` 或 `-O3` | `/O2` | 代码优化 |
| 调试信息 | `-g` | `/Zi` | 生成调试符号 |
| C++标准 | `-std=c++20` | `/std:c++20` | C++语言版本 |
| UTF-8编码 | `-finput-charset=UTF-8` | `/utf-8` | 源代码编码 |
| 链接动态库 | `-lxxx` 或 `-L xxx` | `xxx.lib` | 链接库 |
| 导出符号 | `__attribute__((visibility("default")))` | `__declspec(dllexport)` | 导出DLL符号 |
| 预处理器 | `-DDEFINE` | `/DDEFINE` | 定义宏 |

### 使用生成器表达式处理编译器差异

```cmake
target_compile_options(my_target PRIVATE
    # GCC/Clang 选项
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
        -Wall -Wextra -Wpedantic
        -fPIC
    >
    # MSVC 选项
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4
        /utf-8
        /MP
        /permissive-
    >
)

# 链接库时的编译器差异
target_link_libraries(my_target PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:pthread>
    $<$<CXX_COMPILER_ID:MSVC>:>
)
```

### 常用内置变量

| 变量 | 说明 |
|------|------|
| `CMAKE_CXX_COMPILER_ID` | C++编译器标识（GNU/Clang/MSVC） |
| `CMAKE_CXX_COMPILER_VERSION` | C++编译器版本 |
| `CMAKE_C_COMPILER_ID` | C编译器标识 |
| `CMAKE_C_COMPILER_VERSION` | C编译器版本 |
| `CMAKE_GENERATOR` | 使用的生成器 |
| `CMAKE_GENERATOR_TOOLSET` | 工具集（如VS的x64/x86） |

---

## 项目结构

### 单目录项目

```
project/
├── CMakeLists.txt
├── main.cpp
└── lib.cpp
```

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyProject)

add_executable(my_app main.cpp lib.cpp)
```

### 多目录项目

```
project/
├── CMakeLists.txt
├── src/
│   ├── CMakeLists.txt
│   └── main.cpp
└── lib/
    ├── CMakeLists.txt
    ├── mylib.h
    └── mylib.cpp
```

根目录 CMakeLists.txt：
```cmake
cmake_minimum_required(VERSION 3.20)
project(MyProject)

add_subdirectory(lib)
add_subdirectory(src)
```

---

## 构建流程

### 1. 配置阶段（Configure）

> 💡 **比喻**：这个阶段就像建筑师拿到你的需求后，画设计图、选材料、算成本的阶段。他会检查你有什么工具（编译器）、需要什么材料（库），然后制定一份详细的建造计划。

```bash
# 在项目根目录
mkdir build && cd build
cmake ..
```

这个阶段：
- 解析 CMakeLists.txt
- 检查编译器和工具链
- 查找依赖库
- 生成 CMakeCache.txt

### 2. 生成阶段（Generate）

CMake 根据生成器创建构建文件：
- Unix Makefiles
- Ninja
- Visual Studio
- Xcode

> 💡 **比喻**：这个阶段就像建筑师把设计图交给施工队。不同施工队需要不同的图纸格式——有的是蓝图（Makefiles），有的是施工单（Ninja），有的是3D模型（Visual Studio）。

### 3. 构建阶段（Build）

> 💡 **比喻**：这就是真正动手盖房子的阶段。工人根据图纸开始施工，最终产出可住的房子（可执行文件）或预制房间（库文件）。

```bash
cmake --build .

# 或使用本地构建工具
make        # Unix Makefiles
ninja       # Ninja
msbuild     # Visual Studio
```

### 4. 安装阶段（Install）

> 💡 **比喻**：房子盖好后，需要搬到指定的地方。有的要搬到城市的商品房区（/usr/local），有的要搬到你的项目目录。这就是安装阶段做的事情。

```bash
cmake --install . --prefix /usr/local
```

---

## 小结

CMake 的核心概念：
- **目标**：构建的基本单元（可执行文件、库）
- **属性**：控制目标的行为（包含路径、编译选项等）
- **可见性**：控制属性的传播（PUBLIC/PRIVATE/INTERFACE）
- **变量**：存储和传递配置信息
- **生成器表达式**：构建时求值的表达式

理解这些概念是掌握 CMake 的基础！
