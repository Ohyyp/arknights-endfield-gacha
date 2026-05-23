/*
 * Copyright Ohyyp 2026
 * SPDX-License-Identifier: MIT
 */

#pragma once


#include <string>
#include <vector>

class Banner {
private:
    std::string rate_up;
    std::vector<std::string> rate_up_sequence;
    std::vector<std::string> standard_spooks;
    double future_up_weight = 1.0;
    double future_avg_spook_weight = 0.0;

public:
    const std::string& get_rate_up() const;
    void set_rate_up(const std::string& val);

    const std::vector<std::string>& get_rate_up_sequence() const;
    void set_rate_up_sequence(const std::vector<std::string>& val);
    void add_to_rate_up_sequence(const std::string& val);

    const std::vector<std::string>& get_standard_spooks() const;
    void set_standard_spooks(const std::vector<std::string>& val);
    void add_to_standard_spooks(const std::string& val);

    double get_future_up_weight() const;
    void set_future_up_weight(double val);

    double get_future_avg_spook_weight() const;
    void set_future_avg_spook_weight(double val);

    std::vector<std::string> get_dynamic_spooks() const;
};
