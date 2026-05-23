// Copyright Ohyyp 2026
// SPDX-License-Identifier: MIT

#include <GachaMechanics.h>

double GachaMechanics::get_up_rate() { return 0.5; }
double GachaMechanics::get_base_probability() { return 0.008; }

double GachaMechanics::get_probability(int soft_pity) {
    if (soft_pity >= 79) return 1.0;
    if (soft_pity < 65) return get_base_probability();
    return get_base_probability() + (soft_pity - 64) * 0.05;
}

int GachaMechanics::get_free_pulls(int hard_pity) {
    if (hard_pity == 29) return 10;
    return 0;
}
