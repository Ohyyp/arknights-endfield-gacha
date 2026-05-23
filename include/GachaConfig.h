/*
 * Copyright Ohyyp 2026
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <Banner.h>

#include <string>
#include <unordered_map>

class GachaConfig {
private:
    std::unordered_map<std::string, double> weights;
    Banner banner;

public:
    const std::unordered_map<std::string, double>& get_weights() const;
    void set_weight(const std::string& name, double val);

    const Banner& get_banner() const;
    void set_banner(const Banner& b);

    double get_weight(const std::string &name) const;
    double get_avg_spook_weight() const;
};
