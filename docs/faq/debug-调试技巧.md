# 调试技巧

> 💡 CMake 脚本不听话？先 `message()` 打印变量，再 `--trace` 全程跟踪。

---

## Q1: 如何调试 CMake 脚本？

**问题**：CMake 脚本执行不符合预期。

**解决方案**：

```cmake
# 1. 使用 message 输出变量
message(STATUS "MY_VAR = ${MY_VAR}")
message(STATUS "MY_LIST = ${MY_LIST}")

# 2. 输出所有变量
get_cmake_property(_variableNames VARIABLES)
foreach(_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

# 3. 使用 --trace 选项
# cmake --trace ..

# 4. 使用 --debug-output 选项
# cmake --debug-output ..

# 5. 输出目标属性
get_target_property(INCLUDES my_target INCLUDE_DIRECTORIES)
message(STATUS "Include dirs: ${INCLUDES}")
```

---

## Q2: 如何查看目标的完整属性？

**问题**：想知道目标的所有属性。

**解决方案**：

```cmake
# 获取特定属性
get_target_property(INCLUDES my_target INCLUDE_DIRECTORIES)
get_target_property(COMPILE_DEFS my_target COMPILE_DEFINITIONS)
get_target_property(LINK_LIBS my_target LINK_LIBRARIES)

# 输出属性
message(STATUS "Include directories: ${INCLUDES}")
message(STATUS "Compile definitions: ${COMPILE_DEFS}")
message(STATUS "Link libraries: ${LINK_LIBS}")
```

---

> 📖 **相关阅读**：
> - [build-配置与构建.md](./build-配置与构建.md) - 查看详细输出
> - [perf-性能优化.md](./perf-性能优化.md) - 构建性能分析
> - [../practices/05-CMake进阶技巧.md](../practices/05-CMake进阶技巧.md) - 调试与分析
