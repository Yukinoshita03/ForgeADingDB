# ForgeADingDB

一个使用现代 C++ 编写的小型关系型数据库，用于学习数据库内核的基本组成和工程化实现。

[![CI](https://github.com/Yukinoshita03/ForgeADingDB/actions/workflows/ci.yml/badge.svg)](https://github.com/Yukinoshita03/ForgeADingDB/actions/workflows/ci.yml)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)

> 项目处于早期开发阶段。当前已完成页面存储和磁盘页读写，SQL 执行层及索引等模块仍在开发中。

## 项目定位

ForgeADingDB 的目标不是实现完整的商业数据库，而是用一个规模可控的代码库，完整走通关系型数据库从磁盘页面到 SQL 执行器的核心路径。

项目以 RedBase 的模块化思路为基础，参考现代数据库教学项目的工程组织方式，所有核心模块保持独立、可测试，并尽量让每个设计决策都能通过实验验证。

## 当前进度

已实现：

- C++20 + CMake 工程骨架
- 固定大小页面 `Page`
- `PageId` 和页面字节存储
- 页面重置、读写访问和常量访问接口
- `DiskManager` 的数据库文件打开
- 页面按固定偏移写入和读取
- 非法页面 ID 检查
- CTest 单元测试
- Ubuntu 和 Windows GitHub Actions CI

开发中：

- Buffer Pool
- LRU 页面替换
- Heap Page 和 Heap File
- Tuple、RID 和 Catalog
- B+ Tree 索引
- 简化 SQL Parser
- Volcano 执行器

## 技术路线

```text
Page
  |
  v
DiskManager
  |
  v
Buffer Pool -> LRU / 页面替换
  |
  v
Heap File -> Tuple / RID
  |
  v
Catalog / B+ Tree
  |
  v
Parser -> Planner -> Volcano Executor
```

计划支持的基础算子包括：

- `SeqScan`
- `IndexScan`
- `Filter`
- `Projection`
- `NestedLoopJoin`

最终计划支持基础的 `CREATE`、`INSERT`、`SELECT`、`UPDATE` 和 `DELETE` 操作，并在核心功能完成后选择 WAL 崩溃恢复或 LRU-K + benchmark 作为扩展方向。

## 快速开始

### 环境要求

- C++20 编译器
- CMake 3.20 或更高版本
- Ninja 或 Make 等构建工具

### 配置、构建和测试

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

运行版本信息：

```bash
./build/forgeadingdb --version
```

在 Windows 多配置生成器下，执行文件通常位于：

```powershell
.\build\Release\forgeadingdb.exe --version
```

## 项目结构

```text
ForgeADingDB/
├── include/forgeadingdb/      # 对外头文件
│   ├── common/                # 公共类型
│   └── storage/               # Page、DiskManager 等存储组件
├── src/                       # 核心实现
├── tests/                     # 单元测试
├── docs/                      # 设计和学习文档
├── .github/workflows/         # GitHub Actions CI
├── CMakeLists.txt
└── README.md
```

## 设计原则

- 先建立清晰的数据和所有权边界，再逐步增加功能。
- 每个核心模块都配套最小可运行测试。
- 区分已经验证的能力和规划中的能力。
- 优先使用标准库和简单的数据结构，控制核心代码规模。
- 让构建、测试和实验可以被其他开发者重新运行。

## 开发路线

1. 完善 `Page` 和 `DiskManager` 的边界与错误处理。
2. 实现 Buffer Pool 和 LRU 页面替换。
3. 实现 Heap File、Tuple 和 RID。
4. 实现 B+ Tree 和 Catalog。
5. 接入简化 SQL Parser 和执行计划。
6. 实现 Volcano 执行器及基础算子。
7. 增加端到端 SQL 测试和性能基准。
8. 选择 WAL 崩溃恢复或 LRU-K 作为扩展实验。

## 参与开发

欢迎通过 Issue 讨论设计，通过 Pull Request 提交改进。提交代码前请确保：

```bash
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

新功能应尽量包含对应测试，并在说明中注明设计取舍和已知限制。

## License

ForgeADingDB 使用 [MIT License](LICENSE) 发布。
