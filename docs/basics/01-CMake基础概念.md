# CMake 基础概念

> 📂 **对应示例**：本节内容对应示例代码 [examples/basic/](../../examples/basic/)，建议结合示例代码一起学习。

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

> 💡 **比喻**：想象你在玩乐高——"目标"就是你要拼的东西。一栋完整的城堡（可执行文件），或者一包预制好的城墙模块（库）。城堡需要由这些模块拼装而成，CMake 就是那个"乐高说明书"，告诉你哪块拼哪里。

#### 可执行目标（Executable）
```cmake
add_executable(my_app main.cpp)
```

#### 库目标（Library）
```cmake
# 以下三种库类型选择其一（不能同时使用同名目标）

# 静态库 - 就像一次性浇筑好的混凝土块，直接嵌入房子里
add_library(my_lib STATIC lib.cpp)

# 动态库 - 就像可以拆卸的预制房间，建好后可以单独替换
add_library(my_lib SHARED lib.cpp)

# 头文件库（header-only）- 就像装修指南，只有说明书，不需要实际材料
add_library(my_lib INTERFACE)
```

### 2. 属性（Properties）

每个目标都有自己的属性，控制编译、链接等行为：

> 💡 **比喻**：如果说目标是"房子"，那"属性"就是装修方案——地板用木的还是瓷砖的、墙刷什么颜色、空调装几匹的。每栋房子可以有不同的装修方案，CMake 允许你给每栋房子单独出一份装修清单。

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

> 💡 **比喻**：这就像餐厅的厨房和菜单——PRIVATE 是厨师的秘方，客人看不到也吃不到；PUBLIC 是招牌菜，厨师做、客人吃；INTERFACE 是外卖菜单，厨师自己不吃，但客人可以点。

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

> 💡 **比喻**：普通变量就像写在草稿纸上的便签——用完就扔，下次开会（运行 CMake）还得重新写。

```cmake
# 设置变量
set(MY_VAR "Hello")
set(MY_LIST item1 item2 item3)

# 使用变量
message(STATUS "MY_VAR = ${MY_VAR}")
```

### 2. 缓存变量（Cache Variables）

> 💡 **比喻**：缓存变量就像刻在石碑上的规定——一旦立了，风吹雨打都不变。下次开村民大会（运行 CMake），大家还是按石碑上的办。除非你抡起锤子重新刻（修改缓存）。

缓存变量会持久化到 CMakeCache.txt 文件中：

```cmake
# 设置缓存变量
set(MY_CACHE_VAR "default_value" CACHE STRING "Description")

# 用户可以通过命令行覆盖
# cmake -DMY_CACHE_VAR="custom_value" ..
```

### 3. 环境变量

> 💡 **比喻**：环境变量就像城市的"基础设施"——自来水管、电网、天然气。你家里（CMake 项目）可以接通使用，但通常不会自己去修管道。

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

> 💡 **比喻**：这就像装修的不同级别。Debug = 毛坯房+满屋摄像头+每面墙都贴满标签（方便找问题，但住着不舒服）；Release = 豪华精装修，标签全撕了，住着爽但出了问题找不到原因；RelWithDebInfo = 精装修但偷偷在天花板藏了几个摄像头（出了事还能调监控）；MinSizeRel = 极简主义装修——能省则省，能小则小，断舍离爱好者首选。

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

## 函数与宏

> 💡 **比喻**：函数就像"外卖套餐"——你点了一份宫保鸡丁套餐，厨房按标准流程做出来，你吃你的，不影响隔壁桌。宏就像"抄作业"——把别人的答案直接抄到你自己的卷子上，改了就是改了，没有"隔壁桌"这回事。

### function（函数）

函数有自己的变量作用域，函数内修改的变量不会影响外部：

```cmake
# 定义函数
function(add_my_library name)
    # 函数参数
    # ARGC - 参数个数
    # ARGV - 所有参数列表
    # ARGV0, ARGV1, ... - 按位置访问参数
    # ARGN - 多余的参数

    add_library(${name} STATIC ${ARGN})
    target_compile_features(${name} PUBLIC cxx_std_20)

    # 使用 PARENT_SCOPE 修改外部变量
    set(LAST_LIB_NAME ${name} PARENT_SCOPE)
endfunction()

# 调用函数
add_my_library(math_lib math.cpp add.cpp)
# 创建了静态库 math_lib，源文件为 math.cpp add.cpp
```

### macro（宏）

宏没有自己的作用域，直接在调用处展开：

```cmake
# 定义宏
macro(print_version)
    message(STATUS "Project: ${PROJECT_NAME}")
    message(STATUS "Version: ${PROJECT_VERSION}")
endmacro()

# 调用宏
print_version()
```

### function vs macro 选择

| 对比项 | function | macro |
|--------|----------|-------|
| 变量作用域 | 独立作用域 | 共享调用者作用域 |
| 修改外部变量 | 需要 `PARENT_SCOPE` | 直接修改 |
| 推荐程度 | ✅ 推荐 | ⚠️ 仅用于简单文本替换 |

> 💡 **建议**：优先使用 `function`，避免 `macro` 的作用域污染问题。只有在需要修改调用者变量或做简单文本替换时才使用 `macro`。

---

## CMake 策略（Policy）

> 💡 **比喻**：CMake 策略就像"交通规则改版"——以前红灯可以右转，现在不行了。策略机制就是给你一个选择：按新规矩来（NEW），还是按老规矩来（OLD）。`cmake_minimum_required(VERSION 3.20)` 就相当于说"我自愿遵守 3.20 版本的所有新交规"。

CMake 随版本更新会引入行为变化，策略机制控制如何处理这些变化：

```cmake
# 查看当前策略设置
cmake_policy(GET CMP0054 policy_status)
message(STATUS "CMP0054: ${policy_status}")

# 设置策略为 NEW（推荐，使用新行为）
cmake_policy(SET CMP0054 NEW)

# 设置策略为 OLD（使用旧行为，不推荐）
cmake_policy(SET CMP0054 OLD)

# 设置策略范围（推荐方式）
cmake_policy(PUSH)
cmake_policy(SET CMP0054 NEW)
# ... 受影响的代码 ...
cmake_policy(POP)
```

### 常见策略

| 策略 | 版本 | 说明 |
|------|------|------|
| `CMP0048` | 3.0 | `project()` 命令管理 VERSION |
| `CMP0054` | 3.1 | `if()` 中仅对未引号参数解引用变量 |
| `CMP0063` | 3.3 | 对可见性隐藏的库目标也启用符号可见性 |
| `CMP0074` | 3.12 | `find_package()` 搜索 `<Package>_ROOT` 变量 |
| `CMP0077` | 3.13 | `option()` 命令在已定义变量时不再覆盖 |
| `CMP0092` | 3.15 | MSVC 警告标志 `/W3` 不再默认添加 |
| `CMP0100` | 3.17 | `file(STRINGS)` 中 `REGEX` 选项的行为 |
| `CMP0169` | 4.0 | `FetchContent_Populate` 已移除（使用 `FetchContent_MakeAvailable`） |

> 💡 **最佳实践**：使用 `cmake_minimum_required(VERSION 3.20)` 会自动将所有在该版本之前引入的策略设为 NEW 行为，通常不需要手动设置策略。只有在遇到策略警告时才需要处理。

> ⚠️ **CMake 4.0 重大变更**：CMake 4.0（2025年3月发布）不再兼容 3.5 之前的版本。如果你的 `cmake_minimum_required(VERSION)` 设置低于 3.5，CMake 4.0+ 会直接报错。建议将最低版本设为 3.5 以上（推荐 3.20+）。如果必须编译旧项目，可设置环境变量 `CMAKE_POLICY_VERSION_MINIMUM=3.5` 临时绕过。

---

## 生成器表达式

生成器表达式允许在构建时（而非配置时）求值：

> 💡 **比喻**：生成器表达式就像"密函"——你写的时候只封好信封，不到拆信的那一刻谁也不知道里面写了啥。比如"如果业主是中国人，就送茅台；如果是法国人，就送红酒"——礼物是什么，要等真正上门那一刻才揭晓。这就是生成器表达式的魔法：条件判断被推迟到了构建时才求值。

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

### 更多实用生成器表达式

| 表达式 | 说明 |
|--------|------|
| `$<TARGET_FILE:target>` | 目标的输出文件路径 |
| `$<TARGET_FILE_NAME:target>` | 目标的输出文件名 |
| `$<TARGET_OBJECTS:target>` | 对象库的目标文件 |
| `$<BUILD_INTERFACE:...>` | 构建时使用的路径 |
| `$<INSTALL_INTERFACE:...>` | 安装后使用的路径 |
| `$<COMPILE_LANGUAGE:lang>` | 当前编译的语言 |
| `$<STREQUAL:a,b>` | 字符串相等比较 |
| `$<VERSION_GREATER:a,b>` | 版本号比较 |
| `$<ANGLE-R>` | 右尖括号 `>`（用于嵌套） |

#### 嵌套生成器表达式示例

```cmake
# 根据编译器和构建类型组合设置选项
target_compile_options(my_target PRIVATE
    $<$<AND:$<CXX_COMPILER_ID:GNU>,$<CONFIG:Debug>>:-g -O0>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/O2 /DNDEBUG>
)

# 使用 BUILD_INTERFACE 和 INSTALL_INTERFACE（库导出时必需）
target_include_directories(my_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 获取目标文件路径（用于自定义命令）
add_custom_command(TARGET my_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:my_lib>
        ${CMAKE_CURRENT_BINARY_DIR}/
    COMMENT "Copying library to build directory"
)

# 根据语言类型设置选项（多语言项目）
target_compile_options(my_target PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-std=c++20>
    $<$<COMPILE_LANGUAGE:C>:-std=c11>
)
```

---

## 主流编译器介绍

> 💡 **比喻**：编译器就像"翻译官"——把你的 C++ 代码翻译成机器听得懂的 0 和 1。GCC 是"老牌翻译"，稳重可靠但话多；Clang 是"年轻翻译"，报错信息像写散文一样优美；MSVC 是"微软御用翻译"，只在 Windows 这个"紫禁城"里上班，规矩多但待遇好。

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

> 💡 **比喻**：这个阶段就像建筑师拿到你的需求后，画设计图、选材料、算成本。他会检查你有什么工具（编译器）、需要什么材料（库），然后制定一份详细的建造计划。如果发现你想要的材料缺货（找不到依赖库），他会当场罢工（报错）。

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

> 💡 **比喻**：这个阶段就像建筑师把设计图交给施工队。不同施工队要的图纸格式不一样——老派施工队要蓝图（Makefiles），效率狂要施工单（Ninja），微软施工队要 3D 模型（Visual Studio），苹果施工队要手绘效果图（Xcode）。

### 3. 构建阶段（Build）

> 💡 **比喻**：这就是真正动手盖房子的阶段。工人们根据图纸叮叮当当地施工，最终产出能住的房子（可执行文件）或预制房间（库文件）。如果代码有 bug，就像房子漏水——住进去才知道。

```bash
cmake --build .

# 或使用本地构建工具
make        # Unix Makefiles
ninja       # Ninja
msbuild     # Visual Studio
```

### 4. 安装阶段（Install）

> 💡 **比喻**：房子盖好了，得搬进小区。有的搬进"高档小区"（/usr/local），有的搬进"经济适用房"（用户目录），还有的搬进"临时板房"（构建目录）。`pip install` 的 C++ 版本就是这个阶段。

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
- **函数与宏**：封装可重用的构建逻辑
- **生成器表达式**：构建时求值的表达式
- **策略**：控制 CMake 版本间的行为兼容性

理解这些概念是掌握 CMake 的基础！

---

> 📖 **下一步阅读**：
> - [02-CMake常用命令参考.md](./02-CMake常用命令参考.md) - 学习 CMake 的常用命令
> - [03-CMake最佳实践.md](../practices/03-CMake最佳实践.md) - 掌握现代 CMake 最佳实践
