# 性能优化问题

> 💡 构建太慢？多雇工人、换电动工具、雇过目不忘的工头、提前备好工具。

---

## Q1: 如何加速构建？

> 💡 **比喻**：让施工队加速的方法：1) 多雇几个工人同时干（并行编译）；2) 换效率更高的电动工具（Ninja 生成器）；3) 雇个过目不忘的工头，做过的活秒出（ccache）；4) 把常用工具提前摆工作台上（预编译头）。

**问题**：构建速度太慢。

**解决方案**：

```bash
# 1. 并行构建
cmake --build . --parallel 8

# 2. 使用 Ninja 生成器
cmake -G Ninja ..
ninja

# 3. 使用 ccache
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

# 4. 使用预编译头
target_precompile_headers(my_target PRIVATE <vector> <string>)
```

---

## Q2: 如何减少重编译？

> 💡 **比喻**：这就像改了一个公共文档，全公司 500 人都得重新阅读签字——改一行代码，1000 个文件重编译。解决方法：1) 把"公告栏"和"内部文件"分开（接口与实现分离）；2) 能不抄送就不抄送（减少头文件包含）；3) 常用文件提前打印好发给大家（预编译头）。

**问题**：修改一个文件导致大量文件重编译。

**解决方案**：

```cmake
# 1. 使用接口库分离接口和实现
add_library(my_lib_interface INTERFACE)
target_include_directories(my_lib_interface INTERFACE include/)

add_library(my_lib_impl src/impl.cpp)
target_link_libraries(my_lib_impl PUBLIC my_lib_interface)

# 2. 减少头文件依赖
# 使用前向声明
# 使用 Pimpl 模式

# 3. 使用预编译头
target_precompile_headers(my_target PRIVATE
    <vector>
    <string>
    <memory>
)
```

---

> 📖 **相关阅读**：
> - [debug-调试技巧.md](./debug-调试技巧.md) - 调试 CMake 脚本
> - [../practices/03-CMake最佳实践.md](../practices/03-CMake最佳实践.md) - 性能优化最佳实践
> - [../practices/05-CMake进阶技巧.md](../practices/05-CMake进阶技巧.md) - 预编译头重用与 ccache
