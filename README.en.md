# Arknights: Endfield Gacha Planner

[Chinese Version](README.md)

This is a high-performance Command Line Interface (CLI) tool built with C++20, designed to help players optimize and plan their gacha pulling strategies in *Arknights: Endfield*.

## Core Features

- **Dual-Engine Calculation Architecture**:
  - **Monte Carlo Simulation Engine**: Handles complex edge cases (such as the 30/60 free pull recruitment rewards) through massive simulations.
  - **Dynamic Programming (DP) Exact Engine**: Computes mathematically perfect expected utility using Bellman equations and Markov Renewal Processes.
- **Infinite Horizon Residual Value Evaluation**: Quantifies the carry-over "residual value" of your current soft pity counter across infinitely long future banner timelines.
- **Customizable Character Weights**: Flexibly assign utility weights of 0.0–1.0 to standard and limited characters using a simple TOML configuration file.

## Environment Requirements

- A C++20 compatible compiler (supports both Clang and GCC).
- CMake (version 3.14 or above).

*Note: Project dependencies (`toml++` and `CLI11`) will be automatically downloaded and linked via `FetchContent` during the CMake configuration phase. No manual package manager installation is required.*

## Build Guide

# Quick Build

This project supports CMake Presets for quick building.

```bash
# Configure and build (using Clang as an example)
cmake --preset clang
cmake --build --preset clang

# Run CLI to verify the engine
./build/clang/EndfieldGacha.exe --help
```

Please refer to the `docs/` directory for detailed mathematical design documents.
