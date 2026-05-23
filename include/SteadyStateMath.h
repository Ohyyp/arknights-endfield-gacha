/*
 * Copyright Ohyyp 2026
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <Banner.h>
#include <SteadyStateResult.h>


class SteadyStateMath {
public:
    // Use Wald's Equation and backward induction to precompute infinite horizon residual values
    static SteadyStateResult calculate_steady_state(const Banner &banner);
};
