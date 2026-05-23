# LLM Context

This document contains additional background context and implicit rules for the Arknights: Endfield Gacha Planner project. It is intended to be provided to LLM assistants in future coding sessions to maintain project context.

## Gacha System Nuances

### Spook Pool Composition (6-Star Characters)

The design document states that the "Spook" (non-rate-up) 6-star pool consists of:

- 5 Standard 6-star characters.
- 2 additional limited 6-star characters chosen from the **previous two** banners.

**Implementation Note:**
The code handles the previous limited characters automatically. The `Banner::get_dynamic_spooks()` function dynamically extracts the previous two characters from the `rate_up_sequence` array provided in the `config.toml`, merging them with the `standard_spooks` to form the full spook pool. This entirely automates the edge cases of early banners.

## Architecture & System Design Context (For LLM Resumption)

If you are an LLM starting a new session with this project, here is the critical context you need to pick up where we left off:

### 1. Data Input Strategy (CLI vs TOML)

- **CLI (Command Line Arguments)**: Used exclusively for runtime variables that change frequently per query. It supports `--soft-pity` for soft pity (0-79), `--hard-pity` for hard pity (0-119), `--budget` for budget, and `--engine` (mc/dp). Inputs are validated to prevent out-of-bounds errors.
- **TOML Config**: Used for static data (character weights, banner configuration). We parse a `[banner]` section to evaluate the exact current banner sequence and its parameters.

### 2. The Dual-Engine Philosophy

- **Monte Carlo (MC) Engine**: Built first. Runs $N$ simulations to find the Expected Utility. Extremely useful for verifying complex logic (like 30/60 pull rewards) before writing the complex math for DP.
- **Dynamic Programming (DP) Engine**: Built second. Uses Backward Induction and Bellman Equations to find the absolute exact mathematical expected utility.

### 3. The "Infinite Horizon" Problem (Crucial Math Detail)

A naive DP simulation of banners would recurse infinitely because pity always carries over. To prevent infinite recursion, we use **Markov Renewal Process** and **Wald's Equation**.
When the user's budget runs out ($b=0$) in the DP model, we *do not* simulate further. Instead, we map their remaining soft pity $p$ to a pre-calculated absolute utility constant $V(p)$. This $V(p)$ is calculated using the infinite steady-state baseline based on the current `future_up_weight` and `future_avg_spook_weight` constant values. (See `design.md` for the exact derivation).

### 4. Development Status

- Design phase is **COMPLETE**.
- Both `design.md` and `design.en.md` are completely synced and updated with the Bellman state transition equations.
- **Phase 1, 2, 3, and 4** are **COMPLETE**. Engines are fully implemented, and the CLI wrapper provides formatted output including Pull Efficiency.
- **Engine Alignment** has been mathematically verified. A bug in the 80-pull hard pity probability (and a resulting array out-of-bounds error) was fixed. Both Monte Carlo and Dynamic Programming engines yield virtually identical expected utilities (error < 0.003%).

### 5. C++ Build Environment

- The project enforces **C++20** and uses **Clang** by default (though GCC compatible).
- **Dependency Management**: We use CMake's `FetchContent` to download dependencies (`tomlplusplus`, `CLI11`) on the fly during the configure stage. **DO NOT** suggest manual package manager installations (like `vcpkg` or `apt-get`) in future instructions, as the build is designed to be fully self-contained.
- **Directory Structure**: Headers are properly located in `include/` and source files in `src/`. `CMakeLists.txt` correctly targets these directories.

### 6. Immediate Next Steps (For the New Agent)

- The project is now fully functional. Future work may involve optimizing the Dynamic Programming engine further, adding new CLI features (like exporting data to JSON/CSV), or supporting UI wrappers. Review user requests to determine the next phase of development.
