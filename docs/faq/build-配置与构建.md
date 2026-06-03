# 配置与构建问题

> 💡 遇到构建问题？先打开"执法记录仪"（详细输出），再看具体症状。

---

## Q1: 如何查看 CMake 的详细输出？

> 💡 **比喻**：这就像给每个工人戴了执法记录仪——谁偷懒了、谁用错工具了、谁把墙砌歪了，一目了然。排查问题的第一招：打开记录仪！

**问题**：想要查看 CMake 构建过程中的详细命令。

**解决方案**：

```bash
# 方法 1：设置变量
cmake -DCMAKE_VERBOSE_MAKEFILE=ON ..

# 方法 2：构建时指定
cmake --build . -- VERBOSE=1

# 方法 3：使用环境变量
export CMAKE_VERBOSE_MAKEFILE=ON
cmake ..
```

---

## Q2: 如何清理构建？

**问题**：想要完全清理构建目录，重新开始。

**解决方案**：

```bash
# 方法 1：删除构建目录（推荐）
rm -rf build
mkdir build && cd build
cmake ..

# 方法 2：使用 cmake --build 清理
cmake --build . --target clean

# 方法 3：清理特定目标
cmake --build . --target my_target --clean-first
```

---

## Q3: 如何指定构建类型？

**问题**：想要构建 Debug 或 Release 版本。

**解决方案**：

```bash
# 单配置生成器（Unix Makefiles、Ninja）
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake -DCMAKE_BUILD_TYPE=Release ..

# 多配置生成器（Visual Studio、Xcode）
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Debug
cmake --build . --config Release
```

在 CMakeLists.txt 中设置默认值：

```cmake
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
```

---

## Q4: 多配置生成器下 CMAKE\_BUILD\_TYPE 无效

> ⚠️ **常见陷阱**：这是 CMake 新手最容易踩的坑之一！

**问题**：设置了 `CMAKE_BUILD_TYPE` 但在 Visual Studio / Xcode 下不生效。

**原因**：多配置生成器（Visual Studio、Xcode、Ninja Multi-Config）在构建时才选择配置，`CMAKE_BUILD_TYPE` 在配置阶段无效。

**解决方案**：

```cmake
# ❌ 错误：对多配置生成器无效
set(CMAKE_BUILD_TYPE Release)

# ✅ 正确：使用生成器表达式
target_compile_options(my_target PRIVATE
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:Debug>:-g -O0>
)

# ✅ 正确：为多配置生成器设置默认配置
# 在 CMakePresets.json 中配置
```

```bash
# 多配置生成器在构建时指定配置
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release

# 单配置生成器在配置时指定
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

| 生成器类型 | 配置时机 | CMAKE_BUILD_TYPE | 示例 |
|-----------|---------|-----------------|------|
| 单配置 | 配置时 | ✅ 有效 | Ninja, Unix Makefiles |
| 多配置 | 构建时 | ❌ 无效 | Visual Studio, Xcode |

---

## Q5: 如何查看所有可用的目标？

**问题**：想知道项目中定义了哪些构建目标。

**解决方案**：

```bash
# 查看所有目标
cmake --build . --target help

# 或使用原生构建工具
make help          # Unix Makefiles
ninja -t targets   # Ninja
```

---

> 📖 **相关阅读**：
> - [build-编译器选择.md](./build-编译器选择.md) - 编译器相关问题
> - [debug-调试技巧.md](./debug-调试技巧.md) - 调试 CMake 脚本
> - [../basics/01-CMake基础概念.md](../basics/01-CMake基础概念.md) - 构建流程详解
