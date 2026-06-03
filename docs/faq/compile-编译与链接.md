# 编译与链接问题

> 💡 链接错误就像拼图缺了块——别慌，逐个排查。

---

## Q1: 如何设置 C++ 标准？

**问题**：需要使用特定版本的 C++ 标准。

**解决方案**：

```cmake
# 方法 1：全局设置
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 方法 2：目标特定设置
target_compile_features(my_target PRIVATE cxx_std_20)

# 方法 3：编译选项（不推荐）
target_compile_options(my_target PRIVATE -std=c++20)
```

---

## Q2: 如何添加编译定义？

**问题**：需要在代码中使用宏定义。

**解决方案**：

```cmake
# 方法 1：target_compile_definitions（推荐）
target_compile_definitions(my_target
    PRIVATE
        DEBUG_MODE
        VERSION="1.0.0"
        MAX_SIZE=100
)

# 方法 2：target_compile_options（不推荐）
target_compile_options(my_target PRIVATE -DDEBUG_MODE)

# 方法 3：configure_file（复杂配置）
configure_file(
    config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
)
```

---

## Q3: 如何解决链接错误？

> 💡 **比喻**：链接错误就像拼图拼到最后发现缺了几块——你得排查：1) 这块拼图买了吗（库链接了没）？2) 拼图顺序对吗（静态库有依赖顺序）？3) 拼图是不是被狗啃了（符号没正确导出）？

**问题**：出现 "undefined reference" 错误。

**解决方案**：

```cmake
# 1. 确保正确链接库
target_link_libraries(my_app
    PRIVATE
        my_lib
        external_lib
)

# 2. 检查库的顺序（静态库）
target_link_libraries(my_app
    PRIVATE
        my_lib
        dep1  # dep1 被 my_lib 使用
        dep2  # dep2 被 dep1 使用
)

# 3. 使用 --start-group 和 --end-group（GCC）
target_link_libraries(my_app
    PRIVATE
        -Wl,--start-group
        lib1
        lib2
        lib3
        -Wl,--end-group
)

# 4. 检查符号可见性
# 在库中确保符号导出
# Windows: __declspec(dllexport)
# Linux: __attribute__((visibility("default")))
```

---

## Q4: 如何处理循环依赖？

> 💡 **比喻**：这就像鸡和蛋的问题——A 公司需要 B 公司的产品，B 公司又需要 A 公司的产品，谁也离不开谁。最好的办法是"分家"：把共同依赖的部分抽出来成立 C 公司，让 A 和 B 都依赖 C。

**问题**：库之间存在循环依赖。

**解决方案**：

```cmake
# 方法 1：重构代码，消除循环依赖（推荐）

# 方法 2：使用链接组
target_link_libraries(my_app
    PRIVATE
        -Wl,--start-group
        lib_a
        lib_b
        -Wl,--end-group
)

# 方法 3：多次链接
target_link_libraries(my_app
    PRIVATE
        lib_a
        lib_b
        lib_a  # 再次链接
)
```

---

> 📖 **相关阅读**：
> - [deps-依赖管理.md](./deps-依赖管理.md) - 依赖查找与管理
> - [platform-跨平台.md](./platform-跨平台.md) - DLL 导出与符号可见性
> - [../basics/01-CMake基础概念.md](../basics/01-CMake基础概念.md) - 可见性详解
