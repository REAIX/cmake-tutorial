# CMake 开发 Qt6 指南

> 💡 **前置阅读**：建议先阅读以下文档了解基础概念：
> - [01-CMake基础概念.md](../basics/01-CMake基础概念.md) - CMake 基本概念
> - [02-CMake常用命令参考.md](../basics/02-CMake常用命令参考.md) - 常用命令
> - [09-CMake与IDE集成.md](./09-CMake与IDE集成.md) - Qt Creator 等 IDE 配置

---

## 1. Qt6 与 CMake 的关系

> 💡 **比喻**：Qt6 就像宜家家具——零件齐全、设计精美，但得按说明书组装。以前宜家只提供 qmake 说明书（只有宜家员工看得懂），现在官方主推 CMake 说明书——通用标准，谁都能看懂。

### 为什么选择 CMake 开发 Qt6？

| 特点 | 说明 |
|------|------|
| **官方推荐** | Qt6 官方主推的构建系统 |
| **跨平台** | 一次配置，Windows/Linux/macOS 都能用 |
| **现代特性** | 支持目标导向、生成器表达式等现代 CMake 特性 |
| **集成方便** | Qt6 提供了完善的 CMake 模块 |

---

## 2. 环境准备

### 2.1 安装 Qt6

> 💡 **比喻**：首先你得先买一套家具（Qt6）——别想着白嫖，虽然开源，但安装包好几个 G，下载的时候可以先泡杯咖啡。

**Windows:**
1. 下载 Qt 官方安装器：https://www.qt.io/download-qt-installer
2. 选择 Qt 6.x 版本
3. 勾选 "Qt" → 选择对应编译器版本（MinGW 或 MSVC）
4. 等待下载安装

**Linux (Ubuntu/Debian):**
```bash
sudo apt install qt6-base-dev
sudo apt install qt6-tools-dev
```

**macOS:**
```bash
brew install qt6
```

### 2.2 安装 CMake 和编译器

- **Windows**: 安装 MinGW-w64 或使用 MSVC
- **Linux**: `sudo apt install cmake g++`
- **macOS**: `brew install cmake`

---

## 3. 最小 Qt6 CMake 项目

### 3.1 项目结构

```
qt_project/
├── CMakeLists.txt
├── main.cpp
└── mywidget.ui
```

### 3.2 最简 CMakeLists.txt

```cmake
# Qt6 最低支持 CMake 3.16，本教程其他部分推荐 3.20+
# 这里使用 3.16 是为了与 Qt6 官方最低要求保持一致
cmake_minimum_required(VERSION 3.16)
project(MyQtApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)        # 自动处理 Qt 元对象
set(CMAKE_AUTORCC ON)        # 自动处理资源文件
set(CMAKE_AUTOUIC ON)        # 自动处理 UI 文件

# 查找 Qt6（推荐方式）
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# 创建可执行文件
# ⚠️ 注意：Qt6 提供了专用的 qt_add_executable() 命令（见下方说明）
# 这里先使用通用 CMake 命令 add_executable，适用于简单项目
add_executable(my_app
    main.cpp
    mywidget.ui
)

# 链接 Qt6 库
target_link_libraries(my_app PRIVATE Qt6::Widgets)
```

### 3.3 main.cpp 示例

```cpp
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    QWidget window;
    window.setWindowTitle("我的第一个 Qt6 程序");
    window.resize(400, 300);
    
    QLabel *label = new QLabel("Hello Qt6!", &window);
    label->setAlignment(Qt::AlignCenter);
    label->setGeometry(100, 80, 200, 40);
    
    QPushButton *button = new QPushButton("点我", &window);
    button->setGeometry(150, 150, 100, 40);
    
    QObject::connect(button, &QPushButton::clicked, [](){
        QApplication::quit();
    });
    
    window.show();
    
    return app.exec();
}
```

> 💡 **重要概念**：上面的 `CMakeLists.txt` 中的三行自动处理设置非常关键：
> - `CMAKE_AUTOMOC ON`: 自动处理 Q_OBJECT、信号槽等元对象
> - `CMAKE_AUTORCC ON`: 自动处理 .qrc 资源文件
> - `CMAKE_AUTOUIC ON`: 自动处理 .ui 文件

### 3.4 Qt6 专用 CMake 命令

> ⚠️ **重要区分**：以下命令是 **Qt6 特有的**，不属于 CMake 通用语法。它们由 Qt6 的 CMake 模块提供，只有在 `find_package(Qt6)` 之后才能使用。本教程主要讲解 CMake 通用语法，这里仅作简要介绍，帮助你阅读 Qt6 项目的 CMakeLists.txt。

#### qt\_add\_executable（Qt 6.3+ 推荐）

Qt6 推荐使用 `qt_add_executable` 替代标准的 `add_executable`，它会自动处理平台特定的应用配置（如 macOS 的 Info.plist、Android 的部署等）：

```cmake
# Qt6 专用命令（替代 add_executable）
qt_add_executable(my_app
    main.cpp
    mywidget.ui
)

# 等价于使用通用命令 + 手动平台配置
add_executable(my_app main.cpp mywidget.ui)
if(APPLE)
    # qt_add_executable 会自动处理 macOS bundle 设置
endif()
```

#### qt\_add\_qml\_module（Qt Quick/QML 项目）

如果你使用 Qt Quick/QML，需要用此命令注册 QML 模块：

```cmake
# Qt6 专用命令：注册 QML 模块
qt_add_qml_module(my_app
    URI MyModule
    VERSION 1.0
    QML_FILES
        Main.qml
        Page1.qml
    RESOURCES
        images/logo.png
)
```

#### qt\_add\_resources（资源文件）

虽然 `CMAKE_AUTORCC` 可以自动处理 `.qrc` 文件，但 `qt_add_resources` 提供更精细的控制：

```cmake
# Qt6 专用命令：手动添加资源
qt_add_resources(my_app "resources"
    PREFIX "/"
    FILES
        images/icon.png
        data/config.json
)
```

#### qt\_add\_translations（国际化）

Qt6 提供了翻译集成的专用命令：

```cmake
# Qt6 专用命令：添加翻译
qt_add_translations(my_app
    TS_FILES
        translations/myapp_zh_CN.ts
        translations/myapp_en.ts
)
```

#### qt\_standard\_project\_setup（Qt 6.3+）

简化 Qt 项目的初始配置，自动设置 AUTOMOC 等：

```cmake
# Qt6 最低支持 CMake 3.16，本教程其他部分推荐 3.20+
# 这里使用 3.16 是为了与 Qt6 官方最低要求保持一致
cmake_minimum_required(VERSION 3.16)
project(MyQtApp LANGUAGES CXX)

# Qt 6.3+ 专用：一键设置 AUTOMOC/AUTORCC/AUTOUIC 等
qt_standard_project_setup()

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE Qt6::Widgets)
```

> 💡 **总结**：对于简单项目，使用通用 CMake 命令（`add_executable` + `CMAKE_AUTOMOC`）即可。对于复杂项目（特别是 Qt Quick/QML、跨平台部署），建议使用 Qt6 专用命令。更多详情请参考 [Qt6 CMake API 官方文档](https://doc.qt.io/qt-6/cmake-manual.html)。

---

## 4. 常用 Qt6 组件查找

> 💡 **比喻**：Qt6 就像一个超级家居市场——一楼卖基础建材（Core）、二楼卖家具（Widgets）、三楼卖灯饰（Quick/QML）、四楼卖智能家居（Network）。你不需要逛完整个市场，告诉 CMake 你要去哪层就行。

### 4.1 常用 Qt6 组件

| 组件名称 | 说明 | 典型用途 |
|---------|------|---------|
| `Qt6::Core` | 核心功能 | 所有 Qt 程序都需要 |
| `Qt6::Widgets` | 控件组件 | 传统 GUI 程序 |
| `Qt6::Gui` | 图形组件 | 绘图、游戏 |
| `Qt6::Quick` | QML 组件 | 现代 UI |
| `Qt6::Qml` | QML 核心 | QML 编程 |
| `Qt6::Network` | 网络功能 | HTTP、TCP、UDP |
| `Qt6::Sql` | 数据库 | SQLite 等 |
| `Qt6::Multimedia` | 多媒体 | 音频、视频 |
| `Qt6::WebEngine` | 浏览器引擎 | 显示网页 |

### 4.2 查找组件的方式

```cmake
# 方式 1：查找多个组件（推荐）
find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Core
)

# 方式 2：查找单个组件
find_package(Qt6 REQUIRED Widgets)

# 方式 3：可选组件（找不到也不报错）
find_package(Qt6 OPTIONAL_COMPONENTS Sql Network)

# 方式 4：指定最低版本
find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets)
```

---

## 5. 完整项目配置示例

### 5.1 完整项目结构

```
my_qt_app/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── mainwindow.cpp
│   ├── mainwindow.h
│   ├── mainwindow.ui
│   └── resources/
│       └── icons.qrc
├── include/              # 公共头文件
└── build/               # 构建目录
```

### 5.2 完整 CMakeLists.txt

```cmake
# Qt6 最低支持 CMake 3.16，本教程其他部分推荐 3.20+
# 这里使用 3.16 是为了与 Qt6 官方最低要求保持一致
cmake_minimum_required(VERSION 3.16)
project(MyQtApp VERSION 1.0.0 LANGUAGES CXX)

# ============ 基本设置 ============
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 自动处理 Qt 文件（MOC/UIC/RCC）
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ============ 查找 Qt6 ============
find_package(Qt6 REQUIRED COMPONENTS
    Widgets      # GUI 控件
    Core         # 核心功能
    # Network    # 网络（可选）
    # Sql        # 数据库（可选）
)

# ============ 源文件列表 ============
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
)

set(HEADERS
    src/mainwindow.h
)

set(UI_FILES
    src/mainwindow.ui
)

set(RESOURCES
    src/resources/icons.qrc
)

# ============ 创建目标 ============
add_executable(my_app
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ${RESOURCES}
)

# ============ 头文件路径 ============
target_include_directories(my_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ============ 链接库 ============
target_link_libraries(my_app PRIVATE
    Qt6::Widgets
    Qt6::Core
)

# ============ 平台特定设置 ============
if(WIN32)
    # Windows: 设置图标
    set_target_properties(my_app PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# ============ 安装设置 ============
install(TARGETS my_app
    RUNTIME DESTINATION bin
)
```

---

## 6. 常见 Qt 模块详解

### 6.1 Widgets 模块（传统 GUI）

```cmake
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# 链接
target_link_libraries(my_app PRIVATE Qt6::Widgets)
```

适用场景：桌面应用程序、传统界面

### 6.2 Quick 模块（现代 UI）

```cmake
find_package(Qt6 REQUIRED COMPONENTS Quick Qml)

# 链接
target_link_libraries(my_app PRIVATE Qt6::Quick Qt6::Qml)
```

适用场景：现代 UI、触摸界面、手机应用风格

### 6.3 Network 模块

```cmake
find_package(Qt6 REQUIRED COMPONENTS Network)

target_link_libraries(my_app PRIVATE Qt6::Network)
```

使用示例：
```cpp
#include <QNetworkAccessManager>
#include <QNetworkReply>
```

### 6.4 Sql 模块

```cmake
find_package(Qt6 REQUIRED COMPONENTS Sql)

target_link_libraries(my_app PRIVATE Qt6::Sql)
```

使用示例：
```cpp
#include <QSqlDatabase>
#include <QSqlQuery>
```

---

## 7. 生成器表达式处理 Qt 平台差异

> 💡 **比喻**：Qt 在不同平台上就像电器插头——中国用扁头、欧洲用圆头、英国用方头。同一个吹风机，到了不同国家得加转换头（平台特定配置），不然插不上。

```cmake
target_compile_options(my_app PRIVATE
    # Qt 在不同平台的特殊处理
    $<$<PLATFORM_ID:Windows>:
        /utf-8
    >
    $<$<PLATFORM_ID:Linux>:
        -fPIC
    >
)

# Qt 平台特定库
target_link_libraries(my_app PRIVATE
    Qt6::Widgets
    $<$<PLATFORM_ID:Linux>:X11>
)
```

---

## 8. 常见问题与解决方案

### Q1: 找不到 Qt6

**问题**：`Could not find Qt6`

**解决方案**：
```bash
# Windows: 设置 Qt 环境变量
# 方法 1: 在 CMakeLists.txt 中指定路径
set(Qt6_DIR "C:/Qt/6.5.0/mingw64/lib/cmake/Qt6")

# 方法 2: 命令行指定
cmake -DCMAKE_PREFIX_PATH=C:/Qt/6.5.0/mingw64 ..

# 方法 3: 添加环境变量
# 系统环境变量 CMAKE_PREFIX_PATH = C:/Qt/6.5.0/mingw64
```

### Q2: MOC 报错

**问题**：`moc: Cannot process Qt signal/slot`

**解决方案**：
1. 确保类继承自 QObject：
   ```cpp
   #include <QObject>
   class MyClass : public QObject {
       Q_OBJECT  // 必须放在类的 private 区
   public:
       MyClass(QObject *parent = nullptr) : QObject(parent) {}
   signals:
       void mySignal();  // 信号
   public slots:
       void mySlot();    // 槽
   };
   ```

2. 确保 CMAKE_AUTOMOC 设置为 ON：
   ```cmake
   set(CMAKE_AUTOMOC ON)
   ```

### Q3: UI 文件没有生成

**问题**：`.ui` 文件没有被处理

**解决方案**：
1. 确保 CMAKE_AUTOUIC 设置为 ON：
   ```cmake
   set(CMAKE_AUTOUIC ON)
   ```

2. 确保 UI 文件在 add_executable 中：
   ```cmake
   add_executable(my_app
       main.cpp
       mainwindow.cpp
       mainwindow.ui  # 必须包含
   )
   ```

### Q4: 链接错误

**问题**：链接时找不到 Qt 库

**解决方案**：
```cmake
# 使用现代 CMake 目标链接
target_link_libraries(my_app PRIVATE Qt6::Widgets)

# 不要使用旧式方法
# target_link_libraries(my_app PRIVATE ${QT_LIBRARIES})  # 错误！
```

### Q5: 中文显示乱码

**解决方案**：
1. 确保源文件保存为 UTF-8 编码
2. 在 CMake 中添加编码设置：
   ```cmake
   if(MSVC)
       target_compile_options(my_app PRIVATE /utf-8)
   endif()
   ```

### Q6: 如何部署 Qt 应用？

**问题**：构建完成后，在其他电脑上运行时缺少 Qt DLL/so 文件。

**解决方案**：

```bash
# Windows: 使用 windeployqt 自动收集依赖
windeployqt my_app.exe

# macOS: 使用 macdeployqt
macdeployqt my_app.app

# Linux: 使用 linuxdeployqt（第三方工具）
linuxdeployqt my_app -appimage
```

在 CMake 中集成部署步骤：

```cmake
# 构建后自动部署（Windows 示例）
if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${Qt6_DIR}/../../../bin")
    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET my_app POST_BUILD
            COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:my_app>
            COMMENT "Deploying Qt dependencies"
        )
    endif()
endif()
```

---

## 9. 完整配置文件模板

### 9.1 基础模板

```cmake
# Qt6 最低支持 CMake 3.16，本教程其他部分推荐 3.20+
# 这里使用 3.16 是为了与 Qt6 官方最低要求保持一致
cmake_minimum_required(VERSION 3.16)
project(MyQtProject VERSION 1.0 LANGUAGES CXX)

# C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt 自动处理
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 查找 Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets Core)

# 添加可执行文件
add_executable(${PROJECT_NAME}
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# 链接 Qt 库
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::Core
)
```

### 9.2 多模块模板

```cmake
# Qt6 最低支持 CMake 3.16，本教程其他部分推荐 3.20+
# 这里使用 3.16 是为了与 Qt6 官方最低要求保持一致
cmake_minimum_required(VERSION 3.16)
project(MyComplexQtApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt 自动处理
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 查找所有需要的 Qt 组件
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Gui
    Network
    Sql
)

# 创建可执行文件
add_executable(${PROJECT_NAME} main.cpp)

# 平台特定设置（必须在 add_executable 之后）
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# 链接所有 Qt 组件
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Network
    Qt6::Sql
)

# 安装
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
```

---

## 10. 构建与运行

### 10.1 命令行构建

```bash
# 创建构建目录
mkdir build
cd build

# 配置（Windows MinGW 示例）
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=C:/Qt/6.5.0/mingw64

# 构建
cmake --build . --parallel

# 运行
./my_app.exe
```

### 10.2 Visual Studio Code 中构建

使用 CMake Tools 插件：
1. `F1` → `CMake: Configure`
2. 选择 Kit（编译器）
3. `F1` → `CMake: Build`
4. `F5` 调试运行

---

## 小结

| 关键设置 | 说明 |
|---------|------|
| `CMAKE_AUTOMOC ON` | 自动处理 MOC（Qt 元对象） |
| `CMAKE_AUTOUIC ON` | 自动处理 UI 文件 |
| `CMAKE_AUTORCC ON` | 自动处理资源文件 |
| `find_package(Qt6 REQUIRED)` | 查找 Qt6 |
| `Qt6::Widgets` | 现代 CMake 方式链接 |
| `qt_add_executable()` | Qt6 专用创建可执行文件（Qt 6.3+） |
| `qt_standard_project_setup()` | Qt6 专用一键配置（Qt 6.3+） |

记住这个口诀：**"自动化三件套，链接用现代目标"**

- 自动化三件套： AUTOMOC + AUTOUIC + AUTORCC
- 现代目标：Qt6::Widgets 而不是 ${QT_LIBRARIES}

祝你 Qt 开发愉快！🚀
