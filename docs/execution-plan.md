# 抽卡规划工具执行计划 (Execution Plan / Action Items)

本文档用于拆解并跟踪 CLI 工具的开发进度，按模块逐步推进。

## Phase 1: 基础数据结构与配置层 (Foundation & Configuration)

- [x] **初始化项目与构建系统**：创建 `CMakeLists.txt`，设置 C++20 标准，并利用 `FetchContent` 引入 `toml++`, `CLI11`。
- [x] **实现核心数据结构 (`Character`, `Banner`, `GachaConfig`)**：完成头文件定义 `GachaTypes.h`。
- [x] **实现 `PlayerStatus` 类**：定义了 `soft_pity`，`banner_pulls` 等核心状态变量。
- [x] **实现 TOML 配置加载器**：完成 `ConfigLoader.h`，支持从外部 TOML 文件反序列化。

## Phase 2: 蒙特卡罗引擎实现 (Monte Carlo Engine)

- [x] **实现随机抽卡单步逻辑 (`GachaSystem::pull_once`)**：包含 0.8% 基础概率及 66 抽后的递增逻辑。
- [x] **实现出金判定逻辑**：处理 50% UP 和 50% 歪卡的分配。
- [x] **实现保底与奖励状态机**：处理 120 抽大保底清零、30/60抽奖励触发逻辑。
- [x] **编写 MC 模拟器外壳**：针对给定预算，执行 $N$ 次“抽到 UP 停手或预算耗尽”的循环。

## Phase 3: 动态规划引擎实现 (DP Engine)

- [x] **实现基础概率分布计算器**：实现并抽离出 `SteadyStateMath.h` 模块。
- [x] **实现跨池稳态残值计算器 (Steady-state Residual Generator)**：结合 `future.banner` 完美闭环。
- [x] **编写核心状态转移机**：实现 $V(i, p, h, b)$ 的递推，将 `has_60_reward` 状态精妙地利用转池时刻判定。
- [x] **实现向后归纳算法 (Backward Induction)**：利用 `std::unordered_map` 及哈希函数处理状态存储。
- `[x]` **引擎结果对齐测试 (Engine Alignment Test)**：编写单元测试，断言 MC 引擎的输出在误差允许范围内与 DP 引擎的绝对精确值对齐。

## Phase 4: CLI 交互层 (CLI Wrapper)

- `[x]` **引入参数解析库**：引入类似 `cxxopts` 或 `CLI11` 的库，处理命令行传参。
- `[x]` **开发命令行接口**：支持 `--pity` (当前小保底), `--budget` (预算抽数), `--engine` (mc/dp) 等参数。
- `[x]` **格式化结果输出**：美化终端输出结果，清晰展示“期望效用”、“抽卡效率”、“残余价值”等关键指标。

## Phase 5: 优化与打包 (Optimization & Release)

- [x] **内存/性能优化**：完全去除了不需要的抽象，采用简单直接的 `GachaMechanics` 静态类，并清理了所有隐式的内联函数（100% 声明与实现分离）。
- [x] **发布与打包**：提供 CMake Presets (`CMakePresets.json`)，零配置支持 Clang 和 GCC 的一键构建验证。
- [x] **代码规范 (SOLID)**：经过多轮重构，代码严格遵循 OOP 规范，杜绝魔法数字 (Magic Numbers)，头文件极度纯净。
