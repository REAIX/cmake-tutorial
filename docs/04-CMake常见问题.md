# CMake 常见问题与解决方案

## 构建配置问题

### Q1: 如何查看 CMake 的详细输出？

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

***

### Q2: 如何清理构建？

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

***

### Q3: 如何指定构建类型？

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

***

### Q3-1: 多配置生成器下 CMAKE\_BUILD\_TYPE 无效

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

***

### Q4: 如何指定编译器？

> 💡 **比喻**：这就像选装修队——GCC 是"老字号施工队"，干了三十年，稳重但话多（警告多）；Clang 是"海归施工队"，报错信息像写诗一样优雅；MSVC 是"微软御用施工队"，只在 Windows 这片地盘干活；MinGW 是"GCC 的海外分部"，在 Windows 上干着 Linux 的活。

**问题**：想要使用特定的编译器。

**解决方案**：

```bash
# 方法 1：命令行指定
cmake -DCMAKE_CXX_COMPILER=clang++ ..
cmake -DCMAKE_CXX_COMPILER=/usr/bin/g++-11 ..

# 方法 2：环境变量
export CXX=clang++
cmake ..

# 方法 3：在 CMakeLists.txt 中设置（不推荐）
set(CMAKE_CXX_COMPILER "clang++")
```

### Q4-1: MinGW 和 MSVC 之间的选择

> 💡 **比喻**：在 Windows 上盖房子，你面临灵魂拷问——选本地帮派（MSVC）还是外来和尚（MinGW）？本地帮派地头蛇，和 Windows 水土相服；外来和尚念的经和 Linux 一样，跨平台更方便。选谁？看你信哪个教。

**问题**：在Windows上应该选择MinGW还是MSVC？

**解决方案**：

| 特性    | MinGW          | MSVC             |
| ----- | -------------- | ---------------- |
| 代码兼容性 | 与Linux/GCC高度兼容 | Windows特定        |
| 调试    | GDB/CDB        | Visual Studio调试器 |
| 库格式   | `.a` / `.lib`  | `.lib`           |
| 发布    | 需要附带GCC运行时     | 需要Visual C++运行时  |
| 构建速度  | 较快             | 较快（并行编译）         |
| IDE集成 | VS Code/CLion  | Visual Studio    |

```cmake
# 检测编译器类型
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(WIN32)
        message(STATUS "Using MinGW GCC")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "Using MSVC")
endif()
```

### Q4-2: 如何在 MinGW 和 MSVC 之间切换

> 💡 **比喻**：这就像换装修队——新来的队伍不认旧图纸（CMakeCache.txt），得先把工地清空，让他们从头画。不清场就换人？两个队伍的图纸混在一起，必出乱子。

**问题**：如何在MinGW和MSVC之间切换？

**解决方案**：

```bash
# 切换到 MinGW（需要清理build目录）
rm -rf build
cmake -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++ ..
cmake --build .

# 切换到 MSVC（需要清理build目录）
rm -rf build
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
```

***

### Q5: 如何查看所有可用的目标？

**问题**：想知道项目中定义了哪些构建目标。

**解决方案**：

```bash
# 查看所有目标
cmake --build . --target help

# 或使用原生构建工具
make help          # Unix Makefiles
ninja -t targets   # Ninja
```

***

## 依赖管理问题

### Q6: find\_package 找不到包怎么办？

> 💡 **比喻**：这就像在宜家找商品——你知道它有，但死活找不到货架号。解决方案：要么问客服（设 CMAKE_PREFIX_PATH），要么直接给货架号（设 XXX_DIR），要么提前查好地图（设环境变量）。

**问题**：`find_package` 无法找到已安装的库。

**解决方案**：

```bash
# 方法 1：指定路径前缀
cmake -DCMAKE_PREFIX_PATH=/path/to/lib ..

# 方法 2：指定具体路径
cmake -DMyLib_DIR=/path/to/lib/cmake/MyLib ..

# 方法 3：设置环境变量
export CMAKE_PREFIX_PATH=/path/to/lib
cmake ..
```

在 CMakeLists.txt 中添加搜索路径：

```cmake
list(APPEND CMAKE_PREFIX_PATH "/usr/local/lib")
list(APPEND CMAKE_PREFIX_PATH "/opt/mylib")

find_package(MyLib REQUIRED)
```

***

### Q7: 如何链接系统库？

**问题**：需要链接系统库，如 pthread、dl、m 等。

**解决方案**：

```cmake
# 方法 1：直接链接
target_link_libraries(my_app PRIVATE pthread)

# 方法 2：使用 find_package（推荐）
find_package(Threads REQUIRED)
target_link_libraries(my_app PRIVATE Threads::Threads)

# 方法 3：平台特定
if(UNIX AND NOT APPLE)
    target_link_libraries(my_app PRIVATE dl rt)
endif()
```

***

### Q8: 如何处理可选依赖？

**问题**：某些库是可选的，找不到时禁用相关功能。

**解决方案**：

```cmake
# 查找可选包
find_package(OpenCV QUIET)

if(OpenCV_FOUND)
    message(STATUS "OpenCV found, enabling image processing")
    target_link_libraries(my_app PRIVATE opencv_core opencv_imgproc)
    target_compile_definitions(my_app PRIVATE USE_OPENCV)
else()
    message(STATUS "OpenCV not found, image processing disabled")
endif()
```

使用现代 CMake 目标：

```cmake
find_package(Boost OPTIONAL_COMPONENTS filesystem)

if(TARGET Boost::filesystem)
    target_link_libraries(my_app PRIVATE Boost::filesystem)
endif()
```

***

## 路径与文件问题

### Q9: 如何正确处理路径？

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

***

### Q10: 如何复制文件到构建目录？

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

***

### Q11: 如何获取所有源文件？

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

***

## 编译与链接问题

### Q12: 如何设置 C++ 标准？

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

***

### Q13: 如何添加编译定义？

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

***

### Q14: 如何解决链接错误？

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

***

### Q15: 如何处理循环依赖？

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

***

## 平台相关问题

### Q16: 如何处理平台差异？

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

### Q16-1: 如何同时处理平台和编译器的差异？

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

### Q16-2: 如何处理 DLL 导出的编译器差异？

> 💡 **比喻**：你要对外开放功能，就像小区要设门禁——MSVC 小区要求刷门禁卡（__declspec(dllexport)），GCC 小区要求人脸识别（__attribute__((visibility)))。跨平台导出宏就是一张"万能门禁卡"，到哪个小区都能刷。

**问题**：不同编译器导出DLL符号的方式不同。

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

### Q16-3: 如何处理运行时库的差异？

> 💡 **比喻**：MSVC 的运行时库就像充电电池——选充电套装（动态 /MD）还是一次性电池（静态 /MT）。如果你用了充电电池但链接的库用了一次性电池，两种电池混用，设备可能短路（链接冲突）。

**问题**：MSVC和MinGG的运行时库选择可能导致链接问题。

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

# 方法 3：传统方法（不推荐，容易出错）
# if(MSVC)
#     foreach(flag_var
#         CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
#         CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE)
#         if(${flag_var} MATCHES "/MD")
#             string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
#         endif()
#     endforeach()
# endif()
```

***

### Q17: 如何在 Windows 上处理 Unicode？

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

***

### Q18: 如何处理 Windows 和 Linux 的路径分隔符？

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

***

## 安装与打包问题

### Q19: 如何安装项目？

**问题**：想要安装构建的文件。

**解决方案**：

```cmake
# 安装目标
install(TARGETS my_lib my_app
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# 安装头文件
install(FILES include/mylib.h
    DESTINATION include
)

# 安装目录
install(DIRECTORY include/
    DESTINATION include
)

# 执行安装
cmake --install . --prefix /usr/local
```

***

### Q20: 如何创建可分发的包？

**问题**：想要创建安装包。

**解决方案**：

```cmake
# 配置 CPack
set(CPACK_PACKAGE_NAME "MyProject")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "My awesome project")

# 设置生成器
set(CPACK_GENERATOR "ZIP;TGZ")  # 跨平台

# DEB 包（Debian/Ubuntu）
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++6")

# RPM 包（RedHat/Fedora）
set(CPACK_RPM_PACKAGE_REQUIRES "libstdc++")

# NSIS（Windows 安装程序）
set(CPACK_NSIS_INSTALL_ROOT "C:\\Program Files")

include(CPack)

# 生成包
cpack -G ZIP
cpack -G DEB
cpack -G RPM
```

***

## 调试技巧

### Q21: 如何调试 CMake 脚本？

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

***

### Q22: 如何查看目标的完整属性？

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

***

## 性能优化问题

### Q23: 如何加速构建？

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

***

### Q24: 如何减少重编译？

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

***

## 小结

CMake 常见问题主要集中在：

- **构建配置**：构建类型、编译器选择、清理构建
- **依赖管理**：查找包、链接库、可选依赖
- **路径处理**：跨平台路径、文件复制
- **编译链接**：C++ 标准、编译定义、链接错误
- **平台差异**：Windows/Linux/macOS 特定问题
- **安装打包**：安装规则、创建分发包
- **调试技巧**：变量输出、属性查看
- **性能优化**：加速构建、减少重编译

遇到问题时，善用 `message()` 命令和 `--trace` 选项可以帮助快速定位问题！
