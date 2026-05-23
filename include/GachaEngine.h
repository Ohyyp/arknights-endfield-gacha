/*
 * Copyright Ohyyp 2026
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <GachaConfig.h>
#include <PlayerStatus.h>
#include <SimulationResult.h>

class GachaEngine {
public:
    virtual ~GachaEngine() = default;

    // Abstract interface for running the simulation.
    virtual SimulationResult run(const GachaConfig &config, const PlayerStatus &start_status, int budget) = 0;
};
