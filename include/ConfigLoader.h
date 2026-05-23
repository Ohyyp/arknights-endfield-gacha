/*
 * Copyright Ohyyp 2026
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <GachaConfig.h>

#include <string_view>

class ConfigLoader {
public:
    static GachaConfig Load(std::string_view filepath);
};
