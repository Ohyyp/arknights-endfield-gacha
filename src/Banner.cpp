// Copyright Ohyyp 2026
// SPDX-License-Identifier: MIT

#include <Banner.h>

#include <algorithm>

std::vector<std::string> Banner::get_dynamic_spooks() const {
    std::vector<std::string> spooks = standard_spooks;
    auto it = std::find(rate_up_sequence.rbegin(), rate_up_sequence.rend(), rate_up);
    if (it != rate_up_sequence.rend()) {
        if (it + 1 != rate_up_sequence.rend()) {
            spooks.push_back(*(it + 1));
            if (it + 2 != rate_up_sequence.rend()) {
                spooks.push_back(*(it + 2));
            }
        }
    }
    return spooks;
}

const std::string& Banner::get_rate_up() const { return rate_up; }
void Banner::set_rate_up(const std::string& val) { rate_up = val; }

const std::vector<std::string>& Banner::get_rate_up_sequence() const { return rate_up_sequence; }
void Banner::set_rate_up_sequence(const std::vector<std::string>& val) { rate_up_sequence = val; }
void Banner::add_to_rate_up_sequence(const std::string& val) { rate_up_sequence.push_back(val); }

const std::vector<std::string>& Banner::get_standard_spooks() const { return standard_spooks; }
void Banner::set_standard_spooks(const std::vector<std::string>& val) { standard_spooks = val; }
void Banner::add_to_standard_spooks(const std::string& val) { standard_spooks.push_back(val); }

double Banner::get_future_up_weight() const { return future_up_weight; }
void Banner::set_future_up_weight(double val) { future_up_weight = val; }

double Banner::get_future_avg_spook_weight() const { return future_avg_spook_weight; }
void Banner::set_future_avg_spook_weight(double val) { future_avg_spook_weight = val; }

