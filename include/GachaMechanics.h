/*
 * Copyright Ohyyp 2026
 * SPDX-License-Identifier: MIT
 */

#pragma once

class GachaMechanics {
public:
    static double get_up_rate();
    static double get_base_probability();
    static double get_probability(int soft_pity);
    static int get_free_pulls(int hard_pity);
};
