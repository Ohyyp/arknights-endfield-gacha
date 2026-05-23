/*
 * Copyright Ohyyp 2026
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <GachaEngine.h>
#include <SteadyStateMath.h>

#include <cstdint>
#include <unordered_map>

class DynamicProgrammingEngine : public GachaEngine {
public:
    SimulationResult run(const GachaConfig &config, const PlayerStatus &start_status, int budget) override;

private:
    std::unordered_map<uint64_t, double> memo;
    SteadyStateResult steady_;

    double solve(const GachaConfig &config, int p, int h, int budget);

    uint64_t encode(int p, int h, int budget) const;
};
