/*
 * Copyright Ohyyp 2026
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <GachaEngine.h>

class MonteCarloEngine : public GachaEngine {
public:
    explicit MonteCarloEngine(int iterations);

    SimulationResult run(const GachaConfig &config, const PlayerStatus &start_status, int budget) override;

private:
    int iterations_;
};
