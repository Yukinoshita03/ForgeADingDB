# ForgeADingDB

ForgeADingDB 是一个使用现代 C++ 编写的小型教学关系型数据库。

> 当前状态：仓库骨架已初始化，数据库功能尚未实现。

## 项目目标

- 使用 C++20 和 CMake，保持核心代码清晰、可测试。
- 实现 Page、Buffer Pool、LRU、Heap File、Tuple 和 RID。
- 实现 B+ Tree、Catalog 和简化 SQL Parser。
- 使用 Volcano 模型实现查询执行器。
- 支持 SeqScan、IndexScan、Filter、Projection 和 NestedLoopJoin。
- 支持基础的 `CREATE`、`INSERT`、`SELECT`、`UPDATE` 和 `DELETE`。
- 在 WAL 崩溃恢复和 LRU-K 性能实验中选择一个作为扩展方向。

项目架构参考 Stanford RedBase，工程组织参考 OceanBase MiniOB，但核心模块将独立设计和实现。

## 范围约束

- 核心实现目标为 10,000 至 15,000 行。
- 行数统计不包含第三方代码、自动生成的 Parser、测试和构建产物。
- 第一阶段不实现分布式执行、完整 SQL 标准、MVCC 或成本优化器。

## 构建

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

运行：

```powershell
.\build\forgeadingdb.exe --version
```

在多配置生成器下，可执行文件可能位于 `build\Debug` 或 `build\Release`。

## 开发路线

1. 存储基础：Page、Disk Manager、Buffer Pool、LRU。
2. 表存储：Tuple、RID、Heap Page、Heap File。
3. 索引与元数据：B+ Tree、Catalog、Schema。
4. 查询链路：Parser、Binder、Plan、Volcano Executor。
5. 数据操作：增删改查、索引维护和测试。
6. 扩展实验：WAL 崩溃恢复，或 LRU-K 与基准测试。

## License

[MIT](LICENSE)

