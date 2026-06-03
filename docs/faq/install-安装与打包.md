# 安装与打包问题

> 💡 安装就像搬家后的"物品归位"，打包就像"打包快递"发出去。

---

## Q1: 如何安装项目？

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

---

## Q2: 如何创建可分发的包？

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

---

> 📖 **相关阅读**：
> - [../practices/03-CMake最佳实践.md](../practices/03-CMake最佳实践.md) - 安装与打包最佳实践
> - [../practices/05-CMake进阶技巧.md](../practices/05-CMake进阶技巧.md) - 高级安装配置
> - [../tools/08-CMake与CI-CD集成.md](../tools/08-CMake与CI-CD集成.md) - CI/CD 中的打包发布
