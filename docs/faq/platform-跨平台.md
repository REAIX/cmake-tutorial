# 跨平台问题

> 💡 跨平台就像跨国施工——入乡随俗，每个国家出一份专属图纸。

---

## Q1: 如何处理平台差异？

> 💡 **比喻**：这就像跨国施工——中国盖楼要抗震标准，日本要防震标准（不一样！），欧洲要节能标准。每个国家的"建筑规范"不同，你得入乡随俗，用生成器表达式给每个国家出一份专属图纸。

**问题**：不同平台需要不同的配置。

**解决方案**：

```cmake
# 检测平台
if(WIN32)
    # Windows 特定代码
    target_compile_definitions(my_target PRIVATE WINDOWS_BUILD)
elseif(UNIX AND NOT APPLE)
    # Linux 特定代码
    target_compile_definitions(my_target PRIVATE LINUX_BUILD)
elseif(APPLE)
    # macOS 特定代码
    target_compile_definitions(my_target PRIVATE MACOS_BUILD)
endif()

# 使用生成器表达式
target_compile_options(my_target
    PRIVATE
        $<$<PLATFORM_ID:Windows>:/W4>
        $<$<PLATFORM_ID:Linux>:-Wall -Wextra>
        $<$<PLATFORM_ID:Darwin>:-Wall -Wextra>
)
```

---

## Q2: 如何同时处理平台和编译器的差异？

> 💡 **比喻**：不仅国家不同规矩不同，同一个国家里不同施工队用的工具也不一样——GCC 队用扳手（-Wall），MSVC 队用螺丝刀（/W4）。你得给每支队伍配他们习惯的工具。

**问题**：需要同时考虑平台和编译器的差异。

**解决方案**：

```cmake
# 组合使用平台和编译器检测
target_compile_options(my_target PRIVATE
    # Linux + GCC/Clang
    $<$<AND:$<PLATFORM_ID:Linux>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>
    $<$<AND:$<PLATFORM_ID:Linux>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra>
    
    # Windows + MSVC
    $<$<AND:$<PLATFORM_ID:Windows>,$<CXX_COMPILER_ID:MSVC>>:/W4>
    
    # Windows + MinGW (GCC)
    $<$<AND:$<PLATFORM_ID:Windows>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>
    
    # macOS + AppleClang
    $<$<AND:$<PLATFORM_ID:Darwin>,$<CXX_COMPILER_ID:AppleClang>>:-Wall -Wextra>
)

# 使用 OR 组合多个编译器
target_compile_options(my_target PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
)
```

---

## Q3: 如何处理 DLL 导出的编译器差异？

> 💡 **比喻**：你要对外开放功能，就像小区要设门禁——MSVC 小区要求刷门禁卡（__declspec(dllexport)），GCC 小区要求人脸识别（__attribute__((visibility)))。跨平台导出宏就是一张"万能门禁卡"，到哪个小区都能刷。

**问题**：不同编译器导出 DLL 符号的方式不同。

**解决方案**：

创建跨平台导出宏头文件 `mylib_export.h`：

```cpp
// mylib_export.h
#ifndef MYLIB_EXPORT_H
#define MYLIB_EXPORT_H

#if defined(_WIN32)
    #if defined(mylib_EXPORTS)
        #define MYLIB_API __declspec(dllexport)
    #else
        #define MYLIB_API __declspec(dllimport)
    #endif
#else
    #if defined(mylib_EXPORTS)
        #define MYLIB_API __attribute__((visibility("default")))
    #else
        #define MYLIB_API
    #endif
#endif

#endif // MYLIB_EXPORT_H
```

在 CMake 中设置导出定义：

```cmake
add_library(mylib SHARED mylib.cpp)

# CMake 会自动定义 mylib_EXPORTS 宏（库名大写 + _EXPORTS）
# 无需手动 target_compile_definitions

# 或者使用 CMake 的生成器表达式（CMake 3.12+ 推荐）
set_target_properties(mylib PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)
include(GenerateExportHeader)
generate_export_header(mylib
    EXPORT_FILE_NAME mylib_export.h
)
```

> 💡 **说明**：当 CMake 构建共享库时，会自动定义 `<libraryname>_EXPORTS` 宏（如 `mylib_EXPORTS`）。导出宏根据此宏判断当前是"导出"还是"导入"，实现跨平台兼容。

---

## Q4: 如何处理运行时库的差异？

> 💡 **比喻**：MSVC 的运行时库就像充电电池——选充电套装（动态 /MD）还是一次性电池（静态 /MT）。如果你用了充电电池但链接的库用了一次性电池，两种电池混用，设备可能短路（链接冲突）。

**问题**：MSVC 和 MinGW 的运行时库选择可能导致链接问题。

**解决方案**：

```cmake
# 方法 1：使用现代 CMake 变量（推荐，CMake 3.15+）
if(MSVC)
    # 自动选择正确的运行时库变体（Debug/Release）
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# 方法 2：在目标级别设置（CMake 3.15+）
if(MSVC)
    set_property(TARGET my_target PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
elseif(MINGW)
    target_link_options(my_target PRIVATE -static-libgcc -static-libstdc++)
endif()
```

---

## Q5: 如何在 Windows 上处理 Unicode？

**问题**：Windows 上出现编码问题。

**解决方案**：

```cmake
# MSVC 使用 UTF-8
if(MSVC)
    target_compile_options(my_target PRIVATE /utf-8)
endif()

# 或使用生成器表达式
target_compile_options(my_target
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/utf-8>
)
```

---

## Q6: 如何处理路径分隔符差异？

**问题**：路径分隔符在不同平台上不一致。

**解决方案**：

```cmake
# CMake 自动处理路径分隔符
set(MY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include")

# 转换路径格式
file(TO_CMAKE_PATH "${MY_PATH}" CMAKE_PATH)
file(TO_NATIVE_PATH "${MY_PATH}" NATIVE_PATH)

# 在代码中使用跨平台路径
# C++17: std::filesystem::path
# C++11: 使用 "/" 或 Boost.Filesystem
```

---

> 📖 **相关阅读**：
> - [build-编译器选择.md](./build-编译器选择.md) - MinGW vs MSVC
> - [compile-编译与链接.md](./compile-编译与链接.md) - 链接错误排查
> - [../basics/01-CMake基础概念.md](../basics/01-CMake基础概念.md) - 生成器表达式详解
