// Copyright Ohyyp 2026
// SPDX-License-Identifier: MIT

#include <ConfigLoader.h>

#include <iostream>
#include <stdexcept>
#include <toml++/toml.h>

GachaConfig ConfigLoader::Load(std::string_view filepath) {
    GachaConfig config;
    toml::table tbl;

    try {
        tbl = toml::parse_file(filepath);
    } catch (const toml::parse_error &err) {
        std::cerr << "Error parsing TOML config: " << err << "\n";
        throw;
    }

    if (auto weights = tbl["weights"].as_table()) {
        for (auto &[key, val] : *weights) {
            config.set_weight(std::string(key.str()), val.value_or(0.0));
        }
    }

    if (auto banner_node = tbl["banner"].as_table()) {
        Banner b;
        b.set_rate_up(banner_node->at("rate_up").value_or(""));

        if (auto arr = banner_node->at("rate_up_sequence").as_array()) {
            for (auto &elem : *arr) {
                if (elem.is_string())
                    b.add_to_rate_up_sequence(elem.as_string()->get());
            }
        }
        if (auto arr = banner_node->at("standard_spooks").as_array()) {
            for (auto &elem : *arr) {
                if (elem.is_string())
                    b.add_to_standard_spooks(elem.as_string()->get());
            }
        }
        
        config.set_banner(b);
    }
    return config;
}
