# CMake 常见问题（FAQ）

> 💡 **比喻**：FAQ 就像"看病指南"——哪里不舒服，翻到对应章节，照方抓药。

## 按主题查找

### 🌱 零基础入门（新手必看！）
| 问题 | 文件 |
|------|------|
| CMake 是什么？我为什么需要它？ | [beginner-零基础入门.md](./beginner-零基础入门.md#q1-cmake-是什么我为什么需要它) |
| CMake 和 Make 有什么区别？ | [beginner-零基础入门.md](./beginner-零基础入门.md#q2-cmake-和-make-有什么区别) |
| 如何安装 CMake？ | [beginner-零基础入门.md](./beginner-零基础入门.md#q3-如何安装-cmake) |
| 我的第一个 CMake 项目怎么建？ | [beginner-零基础入门.md](./beginner-零基础入门.md#q4-我的第一个-cmake-项目怎么建) |
| "配置"、"构建"、"安装"是什么意思？ | [beginner-零基础入门.md](./beginner-零基础入门.md#q5-配置构建安装到底是什么意思) |
| 为什么要在 build 目录里构建？ | [beginner-零基础入门.md](./beginner-零基础入门.md#q6-为什么要在-build-目录里构建) |
| 什么是"生成器"？ | [beginner-零基础入门.md](./beginner-零基础入门.md#q7-什么是生成器generator) |
| 加了新 .cpp 文件为什么编译报错？ | [beginner-零基础入门.md](./beginner-零基础入门.md#q8-我加了一个新的-cpp-文件为什么编译报错) |
| CMakeCache.txt 是什么？能删吗？ | [beginner-零基础入门.md](./beginner-零基础入门.md#q9-cmakecachetxt-是什么能删吗) |
| 改了 CMakeLists.txt 但没生效？ | [beginner-零基础入门.md](./beginner-零基础入门.md#q10-我改了-cmakeliststxt-但好像没生效) |
| 报错了怎么看错误信息？ | [beginner-零基础入门.md](./beginner-零基础入门.md#q11-报错了怎么看错误信息) |
| Debug 和 Release 有什么区别？ | [beginner-零基础入门.md](./beginner-零基础入门.md#q12-debug-和-release-有什么区别) |
| 我该用什么编辑器/IDE？ | [beginner-零基础入门.md](./beginner-零基础入门.md#q13-我该用什么编辑器ide) |
| CMakeLists.txt 的基本结构是什么？ | [beginner-零基础入门.md](./beginner-零基础入门.md#q14-cmakeliststxt-的基本结构是什么) |

### 🔧 配置与构建
| 问题 | 文件 |
|------|------|
| 如何查看 CMake 的详细输出？ | [build-配置与构建.md](./build-配置与构建.md#q1-如何查看-cmake-的详细输出) |
| 如何清理构建？ | [build-配置与构建.md](./build-配置与构建.md#q2-如何清理构建) |
| 如何指定构建类型？ | [build-配置与构建.md](./build-配置与构建.md#q3-如何指定构建类型) |
| 多配置生成器下 CMAKE_BUILD_TYPE 无效？ | [build-配置与构建.md](./build-配置与构建.md#q4-多配置生成器下-cmake_build_type-无效) |
| 如何查看所有可用的目标？ | [build-配置与构建.md](./build-配置与构建.md#q5-如何查看所有可用的目标) |

### 🔨 编译器选择
| 问题 | 文件 |
|------|------|
| 如何指定编译器？ | [build-编译器选择.md](./build-编译器选择.md#q1-如何指定编译器) |
| MinGW 和 MSVC 怎么选？ | [build-编译器选择.md](./build-编译器选择.md#q2-mingw-和-msvc-怎么选) |
| 如何在 MinGW 和 MSVC 之间切换？ | [build-编译器选择.md](./build-编译器选择.md#q3-如何在-mingw-和-msvc-之间切换) |

### 📦 依赖管理
| 问题 | 文件 |
|------|------|
| find_package 找不到包怎么办？ | [deps-依赖管理.md](./deps-依赖管理.md#q1-find_package-找不到包怎么办) |
| 如何链接系统库？ | [deps-依赖管理.md](./deps-依赖管理.md#q2-如何链接系统库) |
| 如何处理可选依赖？ | [deps-依赖管理.md](./deps-依赖管理.md#q3-如何处理可选依赖) |

### 📁 路径与文件
| 问题 | 文件 |
|------|------|
| 如何正确处理路径？ | [files-路径与文件.md](./files-路径与文件.md#q1-如何正确处理路径) |
| 如何复制文件到构建目录？ | [files-路径与文件.md](./files-路径与文件.md#q2-如何复制文件到构建目录) |
| 如何获取所有源文件？ | [files-路径与文件.md](./files-路径与文件.md#q3-如何获取所有源文件) |

### ⚙️ 编译与链接
| 问题 | 文件 |
|------|------|
| 如何设置 C++ 标准？ | [compile-编译与链接.md](./compile-编译与链接.md#q1-如何设置-c-标准) |
| 如何添加编译定义？ | [compile-编译与链接.md](./compile-编译与链接.md#q2-如何添加编译定义) |
| 如何解决链接错误？ | [compile-编译与链接.md](./compile-编译与链接.md#q3-如何解决链接错误) |
| 如何处理循环依赖？ | [compile-编译与链接.md](./compile-编译与链接.md#q4-如何处理循环依赖) |

### 🌍 跨平台
| 问题 | 文件 |
|------|------|
| 如何处理平台差异？ | [platform-跨平台.md](./platform-跨平台.md#q1-如何处理平台差异) |
| 如何同时处理平台和编译器的差异？ | [platform-跨平台.md](./platform-跨平台.md#q2-如何同时处理平台和编译器的差异) |
| 如何处理 DLL 导出的编译器差异？ | [platform-跨平台.md](./platform-跨平台.md#q3-如何处理-dll-导出的编译器差异) |
| 如何处理运行时库的差异？ | [platform-跨平台.md](./platform-跨平台.md#q4-如何处理运行时库的差异) |
| 如何在 Windows 上处理 Unicode？ | [platform-跨平台.md](./platform-跨平台.md#q5-如何在-windows-上处理-unicode) |
| 如何处理路径分隔符差异？ | [platform-跨平台.md](./platform-跨平台.md#q6-如何处理路径分隔符差异) |

### 📥 安装与打包
| 问题 | 文件 |
|------|------|
| 如何安装项目？ | [install-安装与打包.md](./install-安装与打包.md#q1-如何安装项目) |
| 如何创建可分发的包？ | [install-安装与打包.md](./install-安装与打包.md#q2-如何创建可分发的包) |

### 🔍 调试技巧
| 问题 | 文件 |
|------|------|
| 如何调试 CMake 脚本？ | [debug-调试技巧.md](./debug-调试技巧.md#q1-如何调试-cmake-脚本) |
| 如何查看目标的完整属性？ | [debug-调试技巧.md](./debug-调试技巧.md#q2-如何查看目标的完整属性) |

### 🚀 性能优化
| 问题 | 文件 |
|------|------|
| 如何加速构建？ | [perf-性能优化.md](./perf-性能优化.md#q1-如何加速构建) |
| 如何减少重编译？ | [perf-性能优化.md](./perf-性能优化.md#q2-如何减少重编译) |

---

> 📖 **相关阅读**：
> - [../basics/02-CMake常用命令参考.md](../basics/02-CMake常用命令参考.md) - 命令详细用法
> - [../practices/05-CMake进阶技巧.md](../practices/05-CMake进阶技巧.md) - 进阶技巧和调试方法
> - [../tools/09-CMake与IDE集成.md](../tools/09-CMake与IDE集成.md) - IDE 配置和调试
