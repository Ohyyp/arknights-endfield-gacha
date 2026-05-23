// Copyright Ohyyp 2026
// SPDX-License-Identifier: MIT

#include <GachaConfig.h>

double GachaConfig::get_weight(const std::string &name) const {
    auto it = weights.find(name);
    return it != weights.end() ? it->second : 0.0;
}

double GachaConfig::get_avg_spook_weight() const {
    auto spooks = banner.get_dynamic_spooks();
    if (spooks.empty())
        return 0.0;
    double sum = 0.0;
    for (const auto &s : spooks) {
        sum += get_weight(s);
    }
    return sum / spooks.size();
}

const std::unordered_map<std::string, double>& GachaConfig::get_weights() const { return weights; }
void GachaConfig::set_weight(const std::string& name, double val) { weights[name] = val; }

const Banner& GachaConfig::get_banner() const { return banner; }
void GachaConfig::set_banner(const Banner& b) { banner = b; }
