# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.20)
# 参考文档：docs/01-CMake基础概念.md（编译器部分）

# 定义项目名称、版本和使用的语言
project(CompilerOptionsExample VERSION 1.0.0 LANGUAGES CXX)

# ============================================================================
# 方法一：使用 CMAKE_CXX_COMPILER_ID 判断编译器类型
# ============================================================================

# 输出当前编译器信息
message(STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Compiler Path: ${CMAKE_CXX_COMPILER}")

# 创建可执行目标
add_executable(compiler_options_example main.cpp)

# 设置 C++ 标准为 C++20
target_compile_features(compiler_options_example PRIVATE cxx_std_20)

# ============================================================================
# 方法二：使用生成器表达式（推荐方式）
# 生成器表达式在构建时求值，更加灵活
# ============================================================================

# 通用编译选项（所有编译器都适用）
target_compile_options(compiler_options_example PRIVATE
    # 使用生成器表达式根据编译器类型选择不同的选项

    # MSVC (Visual Studio) 特定选项
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4           # 警告级别 4
        /WX           # 将警告视为错误
        /utf-8        # 设置源文件和执行文件编码为 UTF-8
        /permissive-  # 严格标准一致性模式
        /Zc:__cplusplus  # 正确报告 __cplusplus 宏的值
    >

    # GCC 特定选项
    $<$<CXX_COMPILER_ID:GNU>:
        -Wall         # 启用常见警告
        -Wextra       # 额外警告
        -Werror       # 将警告视为错误
        -Wpedantic    # 严格遵循 ISO C++ 标准
        -Wconversion  # 隐式类型转换警告
        -Wshadow      # 变量遮蔽警告
    >

    # Clang 特定选项
    # 注意：不要使用 -Weverything，它会启用大量不实用的警告
    $<$<CXX_COMPILER_ID:Clang>:
        -Wall         # 启用常见警告
        -Wextra       # 额外警告
        -Werror       # 将警告视为错误
        -Wpedantic    # 严格遵循 ISO C++ 标准
        -Wconversion  # 隐式类型转换警告
        -Wshadow      # 变量遮蔽警告
        -Wno-c++98-compat  # 禁用 C++98 兼容性警告（使用现代 C++ 时不需要）
    >
)

# ============================================================================
# 方法三：使用 if-else 条件判断（配置时求值）
# ============================================================================

# 也可以使用传统的 if-else 方式
if(MSVC)
    # MSVC 特定定义
    target_compile_definitions(compiler_options_example PRIVATE
        COMPILER_MSVC
        _CRT_SECURE_NO_WARNINGS  # 禁用 CRT 安全警告
    )

    # MSVC 特定的运行时库选项
    # /MD - 多线程 DLL（Release 默认）
    # /MDd - 多线程 DLL 调试版（Debug 默认）
    # /MT - 多线程静态库
    # /MTd - 多线程静态库调试版
    set_property(TARGET compiler_options_example PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # GCC 特定定义
    target_compile_definitions(compiler_options_example PRIVATE
        COMPILER_GCC
    )

    # GCC 特定的链接选项
    target_link_options(compiler_options_example PRIVATE
        -Wl,--as-needed  # 只链接实际需要的库
    )

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Clang 特定定义
    target_compile_definitions(compiler_options_example PRIVATE
        COMPILER_CLANG
    )
endif()

# ============================================================================
# 方法四：针对不同构建类型设置不同选项
# ============================================================================

# 检测编译器是否支持 AddressSanitizer（MinGW 等环境可能不提供 ASan 运行时库）
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-fsanitize=address" HAS_ASAN_SUPPORT)

# Debug 模式特定选项
target_compile_options(compiler_options_example PRIVATE
    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:
        /Od           # 禁用优化
        /RTC1         # 运行时错误检查
        /Zi           # 调试信息
    >

    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:
        -O0           # 禁用优化
        -g3           # 最大调试信息
    >

    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>>:
        -O0           # 禁用优化
        -g3           # 最大调试信息
    >
)

# AddressSanitizer 需要同时在编译和链接阶段启用
# 注意：MinGW 等环境可能不支持 ASan，因此先检测编译器支持情况
if(HAS_ASAN_SUPPORT)
    target_compile_options(compiler_options_example PRIVATE
        $<$<AND:$<CONFIG:Debug>,$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>>:
            -fsanitize=address  # 地址消毒器
        >
    )
    target_link_options(compiler_options_example PRIVATE
        $<$<AND:$<CONFIG:Debug>,$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>>:
            -fsanitize=address  # 链接阶段也必须启用
        >
    )
    message(STATUS "  AddressSanitizer: enabled for Debug builds")
else()
    message(STATUS "  AddressSanitizer: not supported by this compiler/platform, skipped")
endif()

# Release 模式特定选项
# 注意：-flto 在某些 MinGW 版本上可能需要额外配置
# 推荐使用 CMAKE_INTERPROCEDURAL_OPTIMIZATION 代替手动 -flto
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_output)

target_compile_options(compiler_options_example PRIVATE
    $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:
        /O2           # 最大速度优化
        /GL           # 全程序优化
        /Oi           # 启用内联函数
    >

    $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU>>:
        -O3           # 最高优化级别
        -march=native # 针对当前 CPU 优化
    >

    $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>>:
        -O3           # 最高优化级别
        -march=native # 针对当前 CPU 优化
    >
)

# 使用 CMake 内置的 IPO/LTO 支持（比手动 -flto 更可移植）
if(ipo_supported)
    set_target_properties(compiler_options_example PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_Release ON
    )
    message(STATUS "  IPO/LTO: supported and enabled for Release")
else()
    message(STATUS "  IPO/LTO: not supported (${ipo_output})")
endif()

# ============================================================================
# 方法五：使用 CMAKE_CXX_FLAGS 变量（不推荐，但有时需要）
# ============================================================================

# 这种方式会影响项目中所有目标，不推荐在现代 CMake 中使用
# 但有时需要与旧代码兼容

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
# set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
# set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# ============================================================================
# 输出编译器信息供 C++ 代码使用
# ============================================================================

target_compile_definitions(compiler_options_example PRIVATE
    COMPILER_NAME="${CMAKE_CXX_COMPILER_ID}"
    COMPILER_VERSION="${CMAKE_CXX_COMPILER_VERSION}"
)

# 输出配置状态信息
message(STATUS "06-compiler-options: Compiler options example configured")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
