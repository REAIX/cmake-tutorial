# CMake 开发 Qt6 指南

> 💡 **前置阅读**：建议先阅读以下文档了解基础概念：
> - [01-CMake基础概念.md](./01-CMake基础概念.md) - CMake 基本概念
> - [02-CMake常用命令参考.md](./02-CMake常用命令参考.md) - 常用命令

---

## 1. Qt6 与 CMake 的关系

> 💡 **比喻**：Qt6 就像一套完整的"精装房家具套件"，而 CMake 是帮你把这套家具组装起来的工具。Qt6 官方推荐使用 CMake 作为构建系统，就像家具厂只提供了 CMake 这一种组装说明书。

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

> 💡 **比喻**：首先你得先买一套家具（Qt6），才能谈组装。

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

---

## 4. 常用 Qt6 组件查找

> 💡 **比喻**：Qt6 就像一个大型家具商城，有各种展厅（组件）。你需要告诉 CMake 你要去哪个展厅买东西。

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

# 如果使用 Qt Quick，还需要
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)
# set(CMAKE_AUTOUIC ON)

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

> 💡 **比喻**：Qt 在不同平台上可能需要不同的配置，就像不同地区供电标准不同一样。

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

---

## 9. 完整配置文件模板

### 9.1 基础模板

```cmake
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
cmake_minimum_required(VERSION 3.16)
project(MyComplexQtApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt 自动处理
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 根据平台设置
if(WIN32)
    set(CMAKE_AUTOMOC ON)
elseif(UNIX)
    set(CMAKE_AUTOMOC ON)
endif()

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

记住这个口诀：**"自动化三件套，链接用现代目标"**

- 自动化三件套： AUTOMOC + AUTOUIC + AUTORCC
- 现代目标：Qt6::Widgets 而不是 ${QT_LIBRARIES}

祝你 Qt 开发愉快！🚀
