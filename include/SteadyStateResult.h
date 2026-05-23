/*
 * Copyright Ohyyp 2026
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <vector>

struct SteadyStateResult {
    double u_pull;
    std::vector<double> s_p;
    double e_cost;
};
