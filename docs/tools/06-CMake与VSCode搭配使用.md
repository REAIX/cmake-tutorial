# CMake 与 VS Code 搭配使用指南

> 💡 **相关阅读**：本文聚焦 VSCode 配置。如需了解其他 IDE（CLion、Visual Studio、Qt Creator），请阅读 [09-CMake与IDE集成.md](./09-CMake与IDE集成.md)。

> 💡 **前置阅读**：建议先阅读 [01-CMake基础概念.md](../basics/01-CMake基础概念.md) 了解 CMake 的基本概念。

## 为什么要用 VS Code 配合 CMake？

**比喻**：VS Code 就像一个全能工地办公室，而 CMake 是负责制定施工计划的工程师。VS Code 需要通过 CMake 来了解：
- 你的项目有哪些"房间"（源文件）
- 每个"房间"需要什么材料（头文件路径）
- 用什么工具来建造（编译器设置）
- 怎样建造才能通过验收（编译选项）

---

## 1. 必要的插件安装

VS Code 需要安装以下插件才能愉快地使用 CMake：

| 插件名称 | 作用 | 推荐程度 |
|---------|------|---------|
| **C/C++** (Microsoft) | 代码补全、跳转、调试 | ⭐⭐⭐ 必装 |
| **CMake** (twxs) | CMake 语法高亮、CMakeLists.txt 补全 | ⭐⭐⭐ 必装 |
| **CMake Tools** (Microsoft) | 一键构建、配置、调试 | ⭐⭐⭐ 必装 |

安装方法：VS Code 左侧扩展图标 → 搜索上述插件名称 → 点击安装

---

## 2. 核心配置文件通俗解释

> 💡 **比喻**：VS Code 要正常工作，需要几个"小助手"来帮忙。这些助手各有分工：

### 2.1 c_cpp_properties.json - 代码的"翻译员"

> 💡 **比喻**：这个文件告诉 VS Code："当你帮用户翻译（补全）代码时，去哪些仓库找原材料（头文件）？"

```json
{
    // ======== 配置列表，可以有多个配置 ========
    "configurations": [
        {
            // ----- 1. 配置名称 -----
            // 作用：给这个配置起个名字，方便在UI中切换
            // 示例值："Win32", "Linux", "Debug", "Release"
            "name": "Win32",

            // ----- 2. includePath（头文件搜索路径）------
            // 作用：告诉VS Code去哪里找.h头文件，就像告诉翻译员去哪些仓库找资料
            // "${workspaceFolder}" 是VS Code的特殊变量，代表当前打开的文件夹路径
            // "**" 表示搜索所有子目录
            "includePath": [
                "${workspaceFolder}/**",                    // 项目根目录下的所有文件
                "C:/boost_1_80_0",                       // Boost库的头文件路径（示例）
                "C:/Qt/6.5.0/mingw64_64/include",       // Qt库的头文件路径（示例）
                "C:/mingw64/include",                     // MinGW的头文件目录
                "C:/mingw64/lib/gcc/x86_64-w64-mingw32/12.2.0/include"  // GCC内部头文件
            ],

            // ----- 3. defines（预定义的宏）------
            // 作用：告诉VS Code哪些宏已经被定义了，这样它就能正确理解代码
            // 例如：如果定义了_DEBUG，下面代码中的 #ifdef _DEBUG 才会生效
            "defines": [
                "_DEBUG",           // 调试模式的标志
                "UNICODE",          // 宽字符支持（Windows）
                "_UNICODE",         // 宽字符支持（Windows）
                "_CRT_SECURE_NO_WARNINGS",  // 禁用MSVC安全警告
                "_UNICODE"          // Unicode字符集
            ],

            // ----- 4. compilerPath（编译器路径）------
            // 作用：告诉VS Code你的编译器在哪里
            // 这个路径用来获取编译器的信息，让智能提示更准确
            "compilerPath": "C:/mingw64/bin/g++.exe",

            // ----- 5. cStandard（C语言标准）------
            // 作用：告诉VS Code你用的是哪个版本的C语言
            // 可选值：c11, c17, c99, c89等
            "cStandard": "c17",

            // ----- 6. cppStandard（C++语言标准）------
            // 作用：告诉VS Code你用的是哪个版本的C++语言
            // 可选值：c++98, c++11, c++14, c++17, c++20, c++23
            "cppStandard": "c++20",

            // ----- 7. intelliSenseMode（智能提示模式）------
            // 作用：告诉VS Code用什么编译器的规则来提示代码
            // 不同编译器有些语法细节不同，这个让提示更准确
            // 可选值：
            //   - gcc-x64      （GCC/MinGW）
            //   - clang-x64    （Clang）
            //   - msvc-x64     （MSVC）
            //   - gcc-arm      （ARM架构的GCC）
            "intelliSenseMode": "gcc-x64"
        },

        // ======== 如果你用MSVC，可以再加一个配置 ========
        {
            "name": "MSVC",
            "includePath": [
                "${workspaceFolder}/**",
                "C:/Qt/6.5.0/msvc2022_64/include"  // MSVC版的Qt
            ],
            "defines": [
                "_DEBUG",
                "WIN32",
                "_UNICODE"
            ],
            "compilerPath": "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.36.32532/bin/Hostx64/x64/cl.exe",
            "cStandard": "c17",
            "cppStandard": "c++20",
            "intelliSenseMode": "msvc-x64"
        }
    ],

    // ----- version（配置文件版本）------
    // 作用：告诉VS Code这个配置文件用的是什么格式
    // 一般固定写4
    "version": 4
}
```

**参数速查表**：

| 参数名 | 必须 | 作用 | 举例 |
|--------|------|------|------|
| `name` | ✅ | 配置名称，用于切换 | `"Win32"`, `"Linux"` |
| `includePath` | ✅ | 头文件搜索路径 | `"C:/mingw64/include"` |
| `defines` | ❌ | 预定义的宏 | `"_DEBUG"` |
| `compilerPath` | ✅ | 编译器可执行文件路径 | `"C:/mingw64/bin/g++.exe"` |
| `cStandard` | ✅ | C语言标准版本 | `"c17"` |
| `cppStandard` | ✅ | C++语言标准版本 | `"c++20"` |
| `intelliSenseMode` | ✅ | 智能提示的编译器模式 | `"gcc-x64"`, `"msvc-x64"` |

### 2.2 tasks.json - 预设的"任务清单"

> 💡 **比喻**：这个文件定义了一系列"常用任务"，比如"如何清理工地"、"如何重新规划"。

```json
{
    // ----- version（任务文件版本）------
    // 作用：标明任务配置文件的版本
    // 一般固定写 2.0.0
    "version": "2.0.0",

    // ======== 任务列表 ========
    "tasks": [
        {
            // ----- 1. label（任务名称）------
            // 作用：这个任务的名字，会显示在VS Code的任务列表中
            // 示例值："CMake: 配置项目", "编译", "清理"
            "label": "CMake: 配置项目",

            // ----- 2. type（任务类型）------
            // 作用：指定这个任务怎么运行
            // 可选值：
            //   - "shell"    : 在shell/终端中执行命令
            //   - "process"  : 直接运行一个进程
            "type": "shell",

            // ----- 3. command（要执行的命令）------
            // 作用：告诉VS Code要运行什么程序
            // 可以是完整路径，也可以是PATH中的命令名
            "command": "cmake",

            // ----- 4. args（命令参数）------
            // 作用：传给上面那个命令的参数
            // cmake常用参数：
            //   -S : 指定源码目录（source directory）
            //   -B : 指定构建目录（build directory）
            //   --build : 指定构建
            //   --config : 指定构建配置（Debug/Release）
            //   --parallel : 并行构建
            "args": [
                // "-S" 后面跟源码目录
                "-S",
                "${workspaceFolder}",

                // "-B" 后面跟构建目录
                "-B",
                "${workspaceFolder}/build"
            ],

            // ----- 5. options（运行选项）------
            "options": {
                // ----- cwd（工作目录）------
                // 作用：在哪个目录执行这个命令
                "cwd": "${workspaceFolder}"
            },

            // ----- 6. problemMatcher（问题匹配器）------
            // 作用：告诉VS Code如何解析命令输出中的错误信息
            // 这样VS Code才能在代码中显示错误位置
            // "$gcc" 是GCC/Clang的错误格式
            // "$msvc" 是MSVC的错误格式
            // "[]" 表示不使用问题匹配
            "problemMatcher": [],

            // ----- 7. group（任务分组）------
            "group": {
                // ----- kind（分组类型）------
                // 作用：这个任务属于哪一组
                // 可选值：
                //   - "build" : 构建任务
                //   - "test"  : 测试任务
                //   - "none"  : 普通任务
                "kind": "build",

                // ----- isDefault（是否默认）------
                // 作用：这个分组中的默认任务是什么
                // 按Ctrl+Shift+B时会运行这个任务
                "isDefault": true
            },

            // ----- 8. dependsOn（依赖任务）------
            // 作用：执行这个任务之前，需要先完成哪些任务
            // 这是一个数组，可以依赖多个任务
            "dependsOn": []
        },

        // ======== 第二个任务：构建项目 ========
        {
            "label": "CMake: 构建项目",
            "type": "shell",
            "command": "cmake",

            "args": [
                // --build 指定要构建
                "--build",
                "${workspaceFolder}/build",

                // --config 指定构建类型
                "--config",
                "Debug",

                // --parallel 指定并行job数量
                "--parallel",
                "8"
            ],

            "dependsOn": [
                // 这个任务依赖"CMake: 配置项目"
                // 所以运行时会先执行配置，再执行构建
                "CMake: 配置项目"
            ],

            "group": {
                "kind": "build",
                "isDefault": true
            },

            "problemMatcher": [
                // 使用GCC的问题匹配器
                "$gcc"
            ]
        },

        // ======== 第三个任务：清理构建目录 ========
        {
            "label": "清理构建目录",

            // Windows用PowerShell的删除命令
            "type": "shell",
            "command": "Remove-Item",

            "args": [
                // -Recurse: 递归删除（删除文件夹和里面所有内容）
                "-Recurse",
                // -Force: 强制删除（忽略确认提示）
                "-Force",
                // 要删除的目录
                "${workspaceFolder}/build"
            ],

            "group": {
                "kind": "none"
            }
        },

        // ======== 第四个任务：运行程序 ========
        {
            "label": "运行程序",
            "type": "shell",
            // Windows上直接运行exe文件
            "command": "${workspaceFolder}/build/my_app.exe",

            "args": [
                // 传给程序的参数（如果需要）
                "arg1",
                "arg2"
            ],

            // 运行完成后不等待，直接返回
            "dependsOn": [
                "CMake: 构建项目"
            ]
        }
    ]
}
```

**参数速查表**：

| 参数名 | 必须 | 作用 | 举例 |
|--------|------|------|------|
| `label` | ✅ | 任务名称 | `"CMake: 配置项目"` |
| `type` | ✅ | 任务类型 | `"shell"`, `"process"` |
| `command` | ✅ | 要执行的命令 | `"cmake"`, `"Remove-Item"` |
| `args` | ❌ | 命令参数 | `["-S", "${workspaceFolder}"]` |
| `options.cwd` | ❌ | 工作目录 | `"${workspaceFolder}"` |
| `problemMatcher` | ❌ | 错误解析器 | `"$gcc"`, `"$msvc"` |
| `group.kind` | ❌ | 分组类型 | `"build"`, `"test"` |
| `group.isDefault` | ❌ | 是否默认 | `true`, `false` |
| `dependsOn` | ❌ | 依赖任务 | `["CMake: 配置项目"]` |

### 2.3 launch.json - 调试器的"启动说明书"

> 💡 **比喻**：这个文件告诉 VS Code 的调试器："你想怎么启动这个程序？用什么方式进入？入口在哪里？"

```json
{
    // ----- version（配置文件版本）------
    // 作用：标明调试配置文件的版本
    // 一般固定写 0.2.0
    "version": "0.2.0",

    // ======== 调试配置列表 ========
    "configurations": [
        {
            // ----- 1. name（配置名称）------
            // 作用：这个调试配置的名称，会显示在调试下拉菜单中
            // 应该选择一个能区分不同编译器的名字
            "name": "C++ Launch (GDB/MinGW)",

            // ----- 2. type（调试器类型）------
            // 作用：告诉VS Code使用什么调试器
            // 可选值：
            //   - "cppdbg"  : 通用C/C++调试器（GDB/LLDB/CDB）
            //   - "cppvsdbg": Visual Studio调试器（仅MSVC）
            //   - "lldb"    : LLDB调试器（macOS/Linux）
            //   - "pwa-msvc": Web Ass调试器
            "type": "cppdbg",

            // ----- 3. request（请求类型）------
            // 作用：调试的方式
            // 可选值：
            //   - "launch"  : 启动一个程序进行调试（最常用）
            //   - "attach" : 附加到正在运行的进程
            "request": "launch",

            // ----- 4. program（要调试的程序）------
            // 作用：告诉调试器要调试哪个可执行文件
            // 必须是完整路径或相对于workspaceFolder的路径
            "program": "${workspaceFolder}/build/my_app.exe",

            // ----- 5. args（程序参数）------
            // 作用：程序启动时传入的参数
            // 就像在命令行运行：my_app.exe arg1 arg2
            "args": [
                "arg1",
                "arg2",
                "--option"
            ],

            // ----- 6. stopAtEntry（是否在入口暂停）------
            // 作用：程序启动后是否自动暂停在main()函数
            // 可选值：
            //   - true  : 是，暂停等待调试
            //   - false : 否，让程序正常运行（通常用这个）
            "stopAtEntry": false,

            // ----- 7. cwd（工作目录）------
            // 作用：程序运行的工作目录
            // 程序中的相对路径都是相对于这个目录的
            "cwd": "${workspaceFolder}",

            // ----- 8. environment（环境变量）------
            // 作用：设置程序运行时的环境变量
            // 格式：[{ "name": "变量名", "value": "变量值" }]
            "environment": [
                {
                    "name": "MY_VAR",
                    "value": "hello"
                }
            ],

            // ----- 9. externalConsole（外部终端）------
            // 作用：程序输出显示在哪里
            // 可选值：
            //   - true  : 使用系统终端（Windows上会弹出cmd窗口）
            //   - false : 使用VS Code内置调试控制台
            // Windows GUI程序建议用true
            "externalConsole": false,

            // ----- 10. MIMode（调试器）------
            // 作用：使用什么调试器
            // 可选值（cppdbg类型时）：
            //   - "gdb"  : GNU调试器，用于GCC/Clang/MinGW
            //   - "lldb" : LLVM调试器，用于Clang
            //   - "cdb"  : Windows调试器，用于MSVC
            "MIMode": "gdb",

            // ----- 11. miDebuggerPath（调试器路径）------
            // 作用：调试器可执行文件的完整路径
            "miDebuggerPath": "C:/mingw64/bin/gdb.exe",

            // ----- 12. setupCommands（初始化命令）------
            // 作用：调试开始前执行的命令
            // 常用于设置调试器的一些选项
            "setupCommands": [
                {
                    // ----- description -----
                    // 作用的说明（可选）
                    "description": "启用美观打印",

                    // ----- text -----
                    // 要执行的调试器命令
                    "text": "-enable-pretty-printing",

                    // ----- ignoreFailures -----
                    // 是否忽略命令执行失败
                    "ignoreFailures": true
                }
            ],

            // ----- 13. preLaunchTask（调试前任务）------
            // 作用：调试开始前要执行的任务
            // 通常设置为构建任务，确保调试的是最新代码
            "preLaunchTask": "CMake: 构建项目"
        },

        // ======== MSVC调试配置 ========
        {
            "name": "C++ Launch (MSVC)",

            // MSVC专用调试器类型
            "type": "cppvsdbg",

            "request": "launch",

            "program": "${workspaceFolder}/build/my_app.exe",

            "args": [],

            "stopAtEntry": false,

            "cwd": "${workspaceFolder}",

            // MSVC不需要设置MIMode，因为type已经指定了

            "preLaunchTask": "CMake: 构建项目"
        },

        // ======== 附加到进程配置 ========
        {
            "name": "C++ Attach to Process",
            "type": "cppdbg",
            "request": "attach",

            // 附加模式不需要program，改为用pipeProgram或miDebuggerServerAddress

            // ----- processId -----
            // 作用：要附加的进程ID
            // 可以用${command.pickProcess}让用户选择
            "processId": "${command.pickProcess}",

            "MIMode": "gdb",
            "miDebuggerPath": "C:/mingw64/bin/gdb.exe"
        }
    ]
}
```

**参数速查表**：

| 参数名 | 必须 | 作用 | 举例 |
|--------|------|------|------|
| `name` | ✅ | 配置名称 | `"C++ Launch (GDB)"` |
| `type` | ✅ | 调试器类型 | `"cppdbg"`, `"cppvsdbg"` |
| `request` | ✅ | 请求类型 | `"launch"`, `"attach"` |
| `program` | ✅ | 可执行文件路径 | `"${workspaceFolder}/build/my_app.exe"` |
| `args` | ❌ | 程序参数 | `["arg1", "arg2"]` |
| `stopAtEntry` | ❌ | 入口暂停 | `true`, `false` |
| `cwd` | ❌ | 工作目录 | `"${workspaceFolder}"` |
| `environment` | ❌ | 环境变量 | `[{"name":"VAR","value":"val"}]` |
| `externalConsole` | ❌ | 外部终端 | `true`, `false` |
| `MIMode` | ❌ | 调试器 | `"gdb"`, `"lldb"`, `"cdb"` |
| `miDebuggerPath` | ❌ | 调试器路径 | `"C:/mingw64/bin/gdb.exe"` |
| `setupCommands` | ❌ | 初始化命令 | `-enable-pretty-printing` |
| `preLaunchTask` | ❌ | 调试前任务 | `"CMake: 构建项目"` |
| `processId` | ✅(attach模式) | 进程ID | `${command.pickProcess}` |

### 2.4 settings.json - VS Code 的"个人偏好"

> 💡 **比喻**：这是你给 VS Code 的"个人偏好设置"，告诉它你喜欢什么样的工作方式。

```json
{
    // ========================================
    // ======== CMake Tools 插件设置 ========
    // ========================================

    // ----- cmake.configureOnOpen -----
    // 作用：打开项目时是否自动运行CMake配置
    // true  : 自动配置（推荐）
    // false : 手动配置
    "cmake.configureOnOpen": true,

    // ----- cmake.buildDirectory -----
    // 作用：构建目录的路径（相对或绝对路径）
    // 推荐使用build子目录，与源码分开
    "cmake.buildDirectory": "${workspaceFolder}/build",

    // ----- cmake.configureSettings -----
    // 作用：CMake配置时的额外参数
    // 这些会在运行cmake命令时自动添加
    "cmake.configureSettings": {
        // CMAKE_BUILD_TYPE 设置构建类型
        // 可选值：Debug, Release, RelWithDebInfo, MinSizeRel
        "CMAKE_BUILD_TYPE": "Debug",

        // 可以添加其他CMake变量
        // "CMAKE_CXX_COMPILER": "/path/to/g++",
        // "CMAKE_PREFIX_PATH": "C:/Qt/6.5.0/mingw64"
    },

    // ----- cmake.parallelJobs -----
    // 作用：并行构建的job数量
    // 设置为0表示使用CPU的所有核心
    // 推荐：CPU核心数 或 核心数-1
    "cmake.parallelJobs": 8,

    // ----- cmake.configureOnEdit -----
    // 作用：修改CMakeLists.txt后是否自动重新配置
    // true  : 自动重新配置
    // false : 手动配置
    "cmake.configureOnEdit": true,

    // ----- cmake.saveBeforeBuild -----
    // 作用：构建前是否自动保存所有文件
    "cmake.saveBeforeBuild": true,

    // ----- cmake.buildBeforeLaunch -----
    // 作用：调试前是否自动构建
    "cmake.buildBeforeLaunch": true,

    // ----- cmake.defaultConfigureKit -----
    // 作用：默认使用的编译器套件（Kit）
    // 通常让用户自己选择
    "cmake.defaultConfigureKit": null,

    // ========================================
    // ======== C/C++ 插件设置 ========
    // ========================================

    // ----- C_Cpp.default.configurationProvider -----
    // 作用：指定代码补全的配置提供者
    // ms-vscode.cmake-tools 表示让CMake Tools提供配置
    // 这样C/C++插件就能自动读取CMake的配置
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",

    // ----- C_Cpp.default.cppStandard -----
    // 作用：默认的C++语言标准
    // 用于代码补全和语法检查
    "C_Cpp.default.cppStandard": "c++20",

    // ----- C_Cpp.default.cStandard -----
    // 作用：默认的C语言标准
    "C_Cpp.default.cStandard": "c17",

    // ----- C_Cpp.default.includePath -----
    // 作用：默认的头文件搜索路径
    // 如果CMake配置正常，这里可以留空
    "C_Cpp.default.includePath": [],

    // ----- C_Cpp.default.defines -----
    // 作用：默认的预定义宏
    "C_Cpp.default.defines": [],

    // ----- C_Cpp.default.compilerPath -----
    // 作用：默认的编译器路径
    "C_Cpp.default.compilerPath": "",

    // ----- C_Cpp.default.intelliSenseMode -----
    // 作用：默认的智能提示模式
    "C_Cpp.default.intelliSenseMode": "gcc-x64",

    // ========================================
    // ======== 编辑器设置 ========
    // ========================================

    // ----- editor.formatOnSave -----
    // 作用：保存文件时是否自动格式化代码
    "editor.formatOnSave": false,

    // ----- editor.formatOnType -----
    // 作用：输入时是否自动格式化（慎用，会影响输入速度）
    "editor.formatOnType": false,

    // ----- editor.tabSize -----
    // 作用：Tab键缩进的空格数
    "editor.tabSize": 4,

    // ----- editor.insertSpaces -----
    // 作用：是否用空格代替Tab
    "editor.insertSpaces": true,

    // ----- files.associations -----
    // 作用：文件关联，让特定后缀使用特定语法
    "files.associations": {
        // .h文件用C++语法高亮（因为很多项目把.h当C++用）
        "*.h": "cpp",
        // .hh文件也用C++
        "*.hh": "cpp",
        // .cmake文件用CMake语法
        "CMakeLists.txt": "cmake",
        "*.cmake": "cmake"
    },

    // ----- files.exclude -----
    // 作用：排除某些文件，不显示在资源管理器中
    "files.exclude": {
        "**/build": true,
        "**/.vs": true,
        "**/*.obj": true,
        "**/*.o": true
    }
}
```

**参数速查表**：

| 参数名 | 作用 | 举例 |
|--------|------|------|
| `cmake.configureOnOpen` | 打开时自动配置 | `true`, `false` |
| `cmake.buildDirectory` | 构建目录 | `"${workspaceFolder}/build"` |
| `cmake.configureSettings` | CMake配置参数 | `{"CMAKE_BUILD_TYPE": "Debug"}` |
| `cmake.parallelJobs` | 并行job数 | `8`, `0` |
| `cmake.configureOnEdit` | 编辑时自动配置 | `true`, `false` |
| `cmake.saveBeforeBuild` | 构建前保存 | `true`, `false` |
| `cmake.buildBeforeLaunch` | 调试前构建 | `true`, `false` |
| `C_Cpp.default.configurationProvider` | 配置提供者 | `"ms-vscode.cmake-tools"` |
| `C_Cpp.default.cppStandard` | C++标准 | `"c++20"` |
| `C_Cpp.default.cStandard` | C标准 | `"c17"` |
| `C_Cpp.default.includePath` | 头文件路径 | `[]` |
| `C_Cpp.default.defines` | 预定义宏 | `[]` |
| `C_Cpp.default.intelliSenseMode` | 智能提示模式 | `"gcc-x64"` |
| `editor.formatOnSave` | 保存时格式化 | `true`, `false` |
| `editor.tabSize` | Tab缩进大小 | `4` |
| `files.associations` | 文件关联 | `{"*.h": "cpp"}` |
| `files.exclude` | 排除文件 | `{"**/build": true}` |

---

## 3. 完整配置示例

假设你有一个项目结构如下：

```
my_project/
├── .vscode/
│   ├── c_cpp_properties.json
│   ├── tasks.json
│   └── launch.json
├── CMakeLists.txt
├── src/
│   └── main.cpp
└── build/
```

### 3.1 最简 CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyApp)

set(CMAKE_CXX_STANDARD 20)

# 源文件
add_executable(my_app
    src/main.cpp
)
```

### 3.2 一键生成配置（推荐）

> 💡 **比喻**：手动写这些配置文件太麻烦了，CMake Tools 插件可以帮你一键生成！

**操作步骤**：
1. 打开项目文件夹
2. 按 `F1` 或 `Ctrl+Shift+P`
3. 输入 `CMake: Quick Start` 或 `CMake: Configure`
4. 选择编译器（GCC、Clang、MSVC 等）
5. 选择构建类型（Debug、Release）
6. VS Code 会自动生成基本的配置文件！

### 3.3 手动配置流程

如果一键生成不满足需求，可以手动配置：

1. **配置 CMake**
   - 按 `F1` → `CMake: Configure`
   - 选择编译器
   - 选择构建类型

2. **配置智能提示**
   - 按 `F1` → `C/C++: Edit Configurations (JSON)`
   - 填写 includePath

3. **配置调试**
   - 点击左侧"运行和调试"图标
   - 点击"创建 launch.json 文件"
   - 选择对应调试器

---

## 4. 常见问题与解决方案

### Q1: VS Code 显示 "无法找到 include 文件"

**问题**：代码里有 `#include <iostream>` 下面有红色波浪线。

**解决方案**：
1. 打开命令面板：`Ctrl+Shift+P`
2. 输入 `C/C++: Edit Configurations (JSON)`
3. 在 `includePath` 中添加头文件路径：
   ```json
   "includePath": [
       "${workspaceFolder}/**",
       "C:/mingw64/include",
       "C:/mingw64/lib/gcc/x86_64-w64-mingw32/12.2.0/include"
   ]
   ```

### Q2: 调试按钮是灰色的

**问题**：点击"运行和调试"没有反应。

**解决方案**：
1. 先执行一次 CMake 配置：`F1` → `CMake: Configure`
2. 确保生成了可执行文件
3. 检查 `launch.json` 中的 `program` 路径是否正确

### Q3: MinGW 和 MSVC 之间切换

**问题**：之前用 MinGW，现在想用 MSVC。

**解决方案**：
1. 删除 build 目录（或新建一个）
2. `F1` → `CMake: Select Kit` → 选择 MSVC 编译器
3. `F1` → `CMake: Configure`

### Q4: 编译错误看不到详细输出

**问题**：底部终端只显示 "Build failed"，看不到具体错误。

**解决方案**：
1. 菜单：终端 → 新建终端
2. 手动执行构建：
   ```bash
   cmake --build build --verbose
   ```
或者在 `tasks.json` 中添加 `"verbose": true`

---

## 5. 快捷键速查

| 快捷键 | 作用 |
|-------|------|
| `F1` | 打开命令面板 |
| `Ctrl+Shift+P` | 打开命令面板（简写） |
| `F5` | 开始调试 |
| `Ctrl+F5` | 运行（不调试） |
| `Ctrl+Shift+B` | 执行默认构建任务 |
| `Ctrl+`` | 打开/关闭终端 |
| `F7` | CMake: Configure |
| `Ctrl+F7` | CMake: Build |

---

## 6. 推荐工作流

### 6.1 新手推荐流程

```
1. 打开项目文件夹
   ↓
2. F1 → CMake: Quick Start
   ↓
3. 选择编译器（推荐 MinGW）
   ↓
4. 选择构建类型（Debug）
   ↓
5. F1 → CMake: Configure
   ↓
6. Ctrl+Shift+B 构建
   ↓
7. F5 开始调试
```

### 6.2 日常开发流程

```
1. 打开 VS Code（自动配置）
   ↓
2. 修改代码
   ↓
3. Ctrl+Shift+B 或 F5（自动构建+调试）
   ↓
4. 调试成功后提交代码
```

---

## 小结

| 配置文件 | 作用 | 通俗理解 |
|---------|------|---------|
| `c_cpp_properties.json` | 代码补全和跳转 | 翻译员查词典的目录 |
| `tasks.json` | 预设任务 | 常用任务清单 |
| `launch.json` | 调试启动配置 | 调试器启动说明书 |
| `settings.json` | VS Code 偏好设置 | 个人工作习惯 |

VS Code + CMake 的组合非常强大，但配置确实有一定门槛。建议：
1. **先用插件的一键生成功能**，体验完整流程
2. **遇到问题再手动调整**，不要一次性写全部配置
3. **多用 F1 命令面板**，VS Code 会给你提示

祝你编码愉快！🚀
