# CMake 与 CI/CD 集成

> 💡 **前置阅读**：建议先阅读以下文档了解基础概念：
> - [01-CMake基础概念.md](../basics/01-CMake基础概念.md) - CMake 基本概念
> - [03-CMake最佳实践.md](../practices/03-CMake最佳实践.md) - 最佳实践
> - [05-CMake进阶技巧.md](../practices/05-CMake进阶技巧.md) - CMakePresets 等进阶配置
> - [09-CMake与IDE集成.md](./09-CMake与IDE集成.md) - IDE 中的 CMake 配置

---

## 1. 为什么需要在 CI/CD 中使用 CMake？

> 💡 **比喻**：CI/CD 就像一个"自动质检流水线"——每次你提交代码，流水线自动帮你编译、测试、打包。CMake 是这条流水线上最关键的"编译工位"。

在持续集成/持续部署（CI/CD）中使用 CMake 的好处：

- **自动化构建**：每次提交自动编译，确保代码可构建
- **跨平台验证**：在 Windows、Linux、macOS 上同时测试
- **自动化测试**：构建后自动运行 CTest 测试
- **自动发布**：构建成功后自动打包发布

---

## 2. 通用 CI/CD 配置原则

### 2.1 构建目录策略

```bash
# 推荐使用 out-of-source 构建
mkdir build && cd build
cmake ..
cmake --build .
ctest --output-on-failure
```

### 2.2 常用 CMake 命令

```bash
# 配置
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 构建
cmake --build build --parallel

# 测试
cd build && ctest --output-on-failure

# 安装
cmake --install build --prefix install

# 打包
cd build && cpack
```

### 2.3 CMakePresets.json 与 CI/CD

> 💡 **推荐**：使用 CMakePresets.json 统一本地和 CI 的构建配置，避免在 CI 脚本中硬编码 CMake 参数。

```json
{
    "version": 6,
    "configurePresets": [
        {
            "name": "ci-linux",
            "displayName": "CI Linux",
            "generator": "Ninja",
            "binaryDir": "${sourceDir}/build",
            "cacheVariables": {
                "CMAKE_BUILD_TYPE": "Release",
                "BUILD_TESTS": "ON"
            }
        },
        {
            "name": "ci-windows",
            "displayName": "CI Windows",
            "inherits": "ci-linux",
            "generator": "Visual Studio 17 2022"
        },
        {
            "name": "ci-macos",
            "displayName": "CI macOS",
            "inherits": "ci-linux"
        }
    ],
    "buildPresets": [
        {
            "name": "ci-linux",
            "configurePreset": "ci-linux"
        },
        {
            "name": "ci-windows",
            "configurePreset": "ci-windows"
        }
    ],
    "testPresets": [
        {
            "name": "ci-linux",
            "configurePreset": "ci-linux",
            "output": {
                "outputOnFailure": true
            }
        }
    ]
}
```

CI 中使用预设：

```bash
cmake --preset ci-linux
cmake --build --preset ci-linux
ctest --preset ci-linux
```

---

## 3. GitHub Actions 配置

### 3.1 基础多平台构建

创建 `.github/workflows/cmake.yml`：

```yaml
name: CMake Build

on:
  push:
    branches: [main]
  pull_request:
    branches: [main]

jobs:
  build:
    name: ${{ matrix.config.name }}
    runs-on: ${{ matrix.config.os }}
    strategy:
      fail-fast: false
      matrix:
        config:
          - name: "Linux GCC"
            os: ubuntu-latest
            compiler: gcc
          - name: "Windows MSVC"
            os: windows-latest
            compiler: msvc
          - name: "macOS Clang"
            os: macos-latest
            compiler: clang

    steps:
      - uses: actions/checkout@v4

      - name: Configure
        run: cmake -B build -DCMAKE_BUILD_TYPE=Release

      - name: Build
        run: cmake --build build --parallel

      - name: Test
        working-directory: build
        run: ctest --output-on-failure
```

### 3.2 使用 CMakePresets 的 GitHub Actions

```yaml
name: CMake Presets

on: [push, pull_request]

jobs:
  build:
    strategy:
      matrix:
        preset: [ci-linux, ci-windows, ci-macos]
        include:
          - preset: ci-linux
            os: ubuntu-latest
          - preset: ci-windows
            os: windows-latest
          - preset: ci-macos
            os: macos-latest

    runs-on: ${{ matrix.os }}
    steps:
      - uses: actions/checkout@v4

      - name: Configure
        run: cmake --preset ${{ matrix.preset }}

      - name: Build
        run: cmake --build --preset ${{ matrix.preset }}

      - name: Test
        run: ctest --preset ${{ matrix.preset }}
```

### 3.3 带缓存和发布的工作流

```yaml
name: Build and Release

on:
  push:
    tags: ['v*']

jobs:
  release:
    strategy:
      matrix:
        include:
          - os: ubuntu-latest
            artifact: linux-x86_64
          - os: windows-latest
            artifact: windows-x86_64
          - os: macos-latest
            artifact: macos-arm64

    runs-on: ${{ matrix.os }}
    steps:
      - uses: actions/checkout@v4

      - name: Cache build
        uses: actions/cache@v4
        with:
          path: build
          key: ${{ runner.os }}-cmake-${{ hashFiles('CMakeLists.txt') }}
          restore-keys: ${{ runner.os }}-cmake-

      - name: Configure & Build
        run: |
          cmake -B build -DCMAKE_BUILD_TYPE=Release
          cmake --build build --parallel

      - name: Package
        working-directory: build
        run: cpack -G ZIP

      - name: Upload Artifact
        uses: actions/upload-artifact@v4
        with:
          name: ${{ matrix.artifact }}
          path: build/*.zip

      - name: Release
        uses: softprops/action-gh-release@v2
        if: startsWith(github.ref, 'refs/tags/')
        with:
          files: build/*.zip
```

---

## 4. GitLab CI 配置

创建 `.gitlab-ci.yml`：

```yaml
stages:
  - configure
  - build
  - test

variables:
  CMAKE_BUILD_TYPE: Release

# Linux 构建
build:linux:
  stage: build
  image: gcc:latest
  before_script:
    - apt-get update && apt-get install -y cmake ninja-build
  script:
    - cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    - cmake --build build --parallel
  artifacts:
    paths:
      - build/

test:linux:
  stage: test
  image: gcc:latest
  needs: [build:linux]
  script:
    - cd build && ctest --output-on-failure

# Windows 构建
build:windows:
  stage: build
  tags: [windows]
  script:
    - cmake -B build -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    - cmake --build build --parallel --config Release
  artifacts:
    paths:
      - build/

test:windows:
  stage: test
  tags: [windows]
  needs: [build:windows]
  script:
    - cd build && ctest -C Release --output-on-failure
```

---

## 5. CI/CD 中的常见问题

### Q1: 构建缓存如何配置？

> 💡 **比喻**：缓存就像"把上次施工的工具留在工地上"，下次施工时不用重新搬运。

```yaml
# GitHub Actions 缓存示例
- name: Cache CMake build
  uses: actions/cache@v4
  with:
    path: |
      build/
      !build/CMakeCache.txt
    key: cmake-${{ runner.os }}-${{ hashFiles('CMakeLists.txt', '**/CMakeLists.txt') }}
```

### Q2: 如何处理 FetchContent 依赖下载？

```yaml
# 方法 1：缓存 FetchContent 下载目录
- name: Cache dependencies
  uses: actions/cache@v4
  with:
    path: build/_deps
    key: deps-${{ runner.os }}-${{ hashFiles('CMakeLists.txt') }}

# 方法 2：使用 CMAKE_PREFIX_PATH 指向预安装的依赖
- name: Configure
  run: cmake -B build -DCMAKE_PREFIX_PATH="${{ github.workspace }}/deps"
```

### Q3: 如何在 CI 中处理多配置生成器？

```yaml
# Windows MSVC 需要在构建和测试时都指定 --config
- name: Build
  run: cmake --build build --config Release

- name: Test
  working-directory: build
  run: ctest -C Release --output-on-failure
```

### Q4: 如何生成测试报告？

```bash
# CTest 输出 JUnit 格式报告（CMake 3.21+）
ctest --output-junit report.xml --output-on-failure
```

```yaml
# GitHub Actions 发布测试结果
- name: Test
  working-directory: build
  run: ctest --output-junit report.xml --output-on-failure

- name: Publish Test Results
  uses: dorny/test-reporter@v1
  if: always()
  with:
    name: CTest Results
    path: build/report.xml
    reporter: java-junit
```

---

## 6. 推荐的 CI/CD 工作流

### 完整工作流示例

```
代码提交 → 多平台构建 → 自动测试 → 代码分析 → 打包发布
    ↓           ↓           ↓          ↓          ↓
  触发CI    GCC/MSVC    CTest     警告检查    CPack
           Clang编译    运行      编译数据库   上传Artifact
```

### 最佳实践清单

- ✅ 使用 `CMakePresets.json` 统一配置，避免硬编码
- ✅ 启用 `CMAKE_COMPILE_WARNING_AS_ERROR`（CMake 3.24+）将警告视为错误
- ✅ 使用 `--parallel` 加速构建
- ✅ 缓存构建目录和 FetchContent 依赖
- ✅ 使用 `ctest --output-on-failure` 确保测试失败时输出详情
- ✅ 多平台同时测试（Linux + Windows + macOS）
- ✅ 使用 `--output-junit` 生成测试报告
- ✅ 标签推送时自动发布

---

## 小结

| CI/CD 平台 | 配置文件 | 特点 |
|-----------|---------|------|
| GitHub Actions | `.github/workflows/*.yml` | 免费额度大，生态丰富 |
| GitLab CI | `.gitlab-ci.yml` | 自托管友好，内置制品管理 |

CMake 与 CI/CD 集成的关键点：

- **统一配置**：使用 CMakePresets.json 避免硬编码
- **缓存优化**：缓存构建目录和依赖
- **多平台**：在所有目标平台上构建和测试
- **自动化**：测试、打包、发布全自动
