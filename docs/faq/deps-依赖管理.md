# 依赖管理问题

> 💡 找不到依赖？就像在宜家迷路——别慌，有办法。

---

## Q1: find\_package 找不到包怎么办？

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

---

## Q2: 如何链接系统库？

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

---

## Q3: 如何处理可选依赖？

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

---

> 📖 **相关阅读**：
> - [../basics/02-CMake常用命令参考.md](../basics/02-CMake常用命令参考.md) - find_package 命令详解
> - [../practices/05-CMake进阶技巧.md](../practices/05-CMake进阶技巧.md) - FetchContent 与 ExternalProject
> - [compile-编译与链接.md](./compile-编译与链接.md) - 链接错误排查
