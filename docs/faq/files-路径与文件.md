# 路径与文件问题

> 💡 路径问题？记住 CMake 内部统一用 `/`，不用担心反斜杠。

---

## Q1: 如何正确处理路径？

**问题**：路径在不同平台上表现不一致。

**解决方案**：

```cmake
# 使用 CMake 内置变量
target_include_directories(my_target
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}  # 生成文件目录
)

# 使用绝对路径
get_filename_component(ABS_PATH "${REL_PATH}" ABSOLUTE)

# 路径拼接
set(FULL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include")

# 跨平台路径
file(TO_CMAKE_PATH "/usr/local/bin" CMAKE_PATH)
file(TO_NATIVE_PATH "/usr/local/bin" NATIVE_PATH)
```

---

## Q2: 如何复制文件到构建目录？

**问题**：需要在构建时复制配置文件、资源文件等。

**解决方案**：

```cmake
# 方法 1：configure_file（可以替换变量）
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
    @ONLY
)

# 方法 2：file(COPY)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data
)

# 方法 3：add_custom_command（构建时复制）
add_custom_command(TARGET my_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources
)
```

---

## Q3: 如何获取所有源文件？

**问题**：想要自动获取目录下的所有源文件。

**解决方案**：

```cmake
# 方法 1：GLOB（简单但有缺点）
file(GLOB SOURCES "src/*.cpp")
add_executable(my_app ${SOURCES})

# 方法 2：GLOB_RECURSE（递归）
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 方法 3：分组不同类型
file(GLOB_RECURSE 
    SOURCES "src/*.cpp"
    HEADERS "include/*.h"
)

# 方法 4：显式列出（推荐）
set(SOURCES
    src/main.cpp
    src/utils.cpp
    src/helper.cpp
)
```

**注意**：使用 GLOB 时，添加或删除文件后需要重新运行 CMake。

---

> 📖 **相关阅读**：
> - [../basics/02-CMake常用命令参考.md](../basics/02-CMake常用命令参考.md) - file 命令详解
> - [platform-跨平台.md](./platform-跨平台.md) - 跨平台路径处理
> - [../practices/03-CMake最佳实践.md](../practices/03-CMake最佳实践.md) - 源文件管理最佳实践
