# Arknights: Endfield Gacha Planner Design Document

## Overview
This project is a Command Line Interface (CLI) tool built with pure Modern C++, designed to assist players in planning their gacha pulls for "Arknights: Endfield".

## Gacha System Rules Summary

### Core Objective
The ultimate goal of the gacha system is to obtain 6-star characters.

### Base Probability & Soft Pity
- **Base Probability**: The base chance to obtain a 6-star character is **0.8%** per pull.
- **Probability Scaling**: Starting from the **66th pull** up to the 79th pull, the probability increases by **5%** for each subsequent pull (e.g., 66th pull is 5.8%, 67th pull is 10.8%, etc.).
- **Guaranteed Pull**: The **80th pull** has a **100%** chance to yield a 6-star character.

### 6-Star Character Distribution
Upon obtaining a 6-star character, the outcome is determined as follows:
1. **50% Chance (Rate-UP)**: Obtain the featured limited 6-star character of the current banner.
2. **50% Chance (Spook)**: Obtain a non-featured 6-star character. The "spook" pool consists of:
   - **5 Standard 6-star characters** (remains the same across all banners).
   - **2 Featured limited 6-star characters from the previous two banners** (changes with each banner).

### Hard Pity
- For any specific banner, if a player makes **119 pulls** without obtaining the current featured limited 6-star character, the **120th pull** is guaranteed to be the **current featured limited 6-star character**.
- *Note: If the featured limited 6-star character is obtained before reaching 120 pulls, the 120-pull hard pity is completely reset (it only acts as a ceiling for the first copy of the rate-up character).*
- *Note: Upon triggering this 120-pull hard pity and obtaining the featured 6-star, the soft pity counter (pulls without a 6-star) is instantly reset to 0.*

### Banner Transition & Pity Carry-over
- Banners are time-limited. When a banner ends, it is replaced by a new one.
- In a new banner, the standard 6-stars remain unchanged, while the current and the previous two featured limited 6-stars are updated.
- **Soft Pity Carry-over**: The accumulated pull count without a 6-star carries over to the next banner (e.g., if you made 70 pulls without a 6-star, you start the next banner at 70 pulls for soft pity calculation).
- **Hard Pity Reset**: The 120-pull hard pity count **DOES NOT** carry over. The count resets to 0 for every new banner, even if you reached 119 pulls in the previous banner.

### Special Rules
- **30-Pull Reward (Expedited Recruitment)**: Upon reaching **30 pulls** on any single banner, players are rewarded with a **free 10-pull (Expedited Recruitment) for the current banner**. *Note: This is an independent event. It DOES NOT count towards soft or hard pity, and even if it yields the featured 6-star character, it **DOES NOT** reset the main banner's pity counters.*
- **60-Pull Reward**: Upon reaching **60 pulls** on any single banner, players are rewarded with a **free 10-pull for the next banner**. *Note: Unlike the 30-pull reward, this functions as a normal 10-pull in the next banner and WILL affect pity counters normally.*

## Core Features and Architecture

### Configuration and Inputs
To ensure usability and extensibility, input data is strictly separated into **Command Line Arguments (CLI)** and a **Local Configuration File (TOML)**.

#### 1. Command Line Arguments (CLI)
Controls the runtime state for a specific calculation session:
- `--soft-pity <int>`: Current soft pity counter (0-79, default 0).
- `--hard-pity <int>`: Current hard pity counter (0-119, default 0).
- `--budget <int>`: Maximum planned pulls (budget).
- `--engine <mc|dp>`: Calculation engine to use.
- `--config <path>`: Path to the TOML configuration file.

#### 2. Local Configuration File (TOML)
Uses TOML format to store character dictionaries, weight strategies, and a sequence of banners.
The config must include the following core modules:
- `[weights]`: Global character weight dictionary (0.0-1.0).
- `[banner]`: Defines the current banner. Contains `rate_up` (current featured), `rate_up_sequence` (historical sequence used to automatically extract the past two limiteds), and `standard_spooks`.

**Example TOML Config:**
```toml
[banner]
rate_up = "Zhuang_Fangyi"
rate_up_sequence = ["Laevatain", "Gilberta", "Yvonne", "Tangtang", "Rossi", "Zhuang_Fangyi"]
standard_spooks = ["Ember", "Last_Rite", "Ardelia", "Pogranichnik", "Lifeng"]

[weights]
Laevatain = 1.0
Gilberta = 1.0
Yvonne = 1.0
Tangtang = 1.0
Rossi = 1.0
Zhuang_Fangyi = 1.0

# ... other characters
```

### Output Specification
- **Expected Utility**: The comprehensive expected value calculated based on weights.
- **Pull Efficiency**: Expected Utility / Actual Pulls Invested.
- **Statistics**: Probability distribution of getting the rate-up, hard pity trigger rates, average pulls spent, etc.

### Dual-Engine Calculation Architecture
To ensure both flexibility and mathematical precision, the system will implement two calculation engines toggled via CLI options:

1. **Monte Carlo Simulation Engine**
   - **Concept**: Utilizes RNG to run a massive number of independent gacha simulations (e.g., $10^5$ or $10^6$ runs) and averages the utility results.
   - **Pros**: Highly intuitive and easy to implement complex edge cases. Serves as the "Ground Truth" to verify the mathematical model.

2. **Dynamic Programming (DP) Exact Engine**
   - **Concept**: Calculates exact expected values based on probability theory and Backward Induction.
   - **State Space**: Defined as `V(i, p, h, b)`, representing the **expected future utility** when at banner index `i`, with soft pity `p`, hard pity (banner pulls) `h`, and remaining budget `b`.
   - **Transition Logic**:
     - **State Transition Equation**:
       Let $P_6(p)$ be the probability of a 6-star at soft pity $p$. $W_{UP,i}$ is the UP weight for banner $i$, $W_{Spook,i}$ is the average spook weight for banner $i$.
       
       **Case A: 120 Hard Pity Triggered ($h = 119$)**
       Guarantees UP, resets both pities, advances to next banner $i+1$.
       $$V(i, p, 119, b) = W_{UP,i} + V(i+1, 0, 0, b-1)$$
       
       **Case B: Normal Pull ($h < 119$)**
       - If 6-star ($E_{hit}$): 50% UP (advance to next), 50% Spook (stay in current, reset soft pity, keep hard pity).
         $$E_{hit} = 0.5 \times [W_{UP,i} + V(i+1, 0, 0, b-1)] + 0.5 \times [W_{Spook,i} + V(i, 0, h+1, b-1)]$$
       - If miss ($E_{miss}$): Stay in current, increment both pities.
         $$E_{miss} = V(i, p+1, h+1, b-1)$$
       - **Total Bellman Equation**:
         $$V(i, p, h, b) = P_6(p) \cdot E_{hit} + (1 - P_6(p)) \cdot E_{miss}$$
       *(Note: 30/60 rewards add independent utility constants or effective budget when $h+1$ reaches threshold).*

#### Steady-state Residual Value Evaluation (Infinite Horizon)
When the budget is exhausted ($b=0$), the remaining soft pity $p$ has a "residual value". Since banners extend infinitely, we use a **Markov Renewal Process** combined with `[future.banner]` to calculate the exact steady-state value, breaking the infinite recursion cleanly:

**Exact Derivation:**
1. **Expected Cost & Reward per Banner Cycle**: In the infinite future, starting from 0 pity, the expected pulls to get the UP character is a constant, $E_{cost}$ (capped by the 120 hard pity). The expected number of spooks obtained along the way is $E_{spook}$.
2. **Steady-state Pull Utility ($U_{pull}$)**: By Wald's Equation, the long-run average utility per pull is:
   $$U_{pull} = \frac{W_{future\_up} + E_{spook} \times W_{future\_spook}}{E_{cost}}$$
3. **Saved Pulls ($S_p$)**: Having $p$ soft pity reduces the expected pulls to the *first* 6-star by $S_p = E_0 - E_p$.
4. **Final Residual Value ($V(p)$)**: $V(p) = S_p \times U_{pull}$.

**DP Termination**: When $b=0$, the algorithm returns the precalculated $V(p)$ and terminates.
