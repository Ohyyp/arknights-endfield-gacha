// Copyright Ohyyp 2026
// SPDX-License-Identifier: MIT

#include <DynamicProgrammingEngine.h>
#include <SteadyStateMath.h>
#include <GachaMechanics.h>
#include <cmath>

SimulationResult DynamicProgrammingEngine::run(const GachaConfig &config, const PlayerStatus &start_status, int budget) {
    memo.clear();
    steady_ = SteadyStateMath::calculate_steady_state(config.get_banner());
    double res = solve(config, start_status.soft_pity, start_status.hard_pity, budget);
    
    double base_utility = (budget * steady_.u_pull) + (steady_.s_p[start_status.soft_pity] * steady_.u_pull);
    return { res, base_utility, steady_.e_cost };
}

uint64_t DynamicProgrammingEngine::encode(int p, int h, int budget) const {
    return (static_cast<uint64_t>(budget) << 16) | (static_cast<uint64_t>(h) << 8) | (static_cast<uint64_t>(p));
}

double DynamicProgrammingEngine::solve(const GachaConfig &config, int p, int h, int budget) {
    if (budget == 0) {
        double residual = steady_.s_p[p] * steady_.u_pull;
        return residual;
    }

    uint64_t key = encode(p, h, budget);
    auto it = memo.find(key);
    if (it != memo.end())
        return it->second;

    double w_up = config.get_weight(config.get_banner().get_rate_up());
    double w_spook = config.get_avg_spook_weight();
    double base_prob = GachaMechanics::get_base_probability();
    double up_rate = GachaMechanics::get_up_rate();

    double prob = GachaMechanics::get_probability(p);
    
    int next_budget = budget - 1;
    if (h == 59) {
        next_budget += 10;
    }

    double v_up = w_up + (next_budget * steady_.u_pull);
    double v_spook = w_spook + solve(config, 0, h + 1, next_budget);
    double v_miss = (prob < 1.0) ? solve(config, p + 1, h + 1, next_budget) : 0.0;
    
    double ev_normal_pull = prob * up_rate * v_up + prob * (1.0 - up_rate) * v_spook + (1.0 - prob) * v_miss;

    if (h == 119) {
        ev_normal_pull = w_up + (next_budget * steady_.u_pull);
    }

    double expected_val = 0.0;
    if (h == 29) {
        double p_free_miss = std::pow(1.0 - base_prob * up_rate, 10);
        double v_free_up = w_up + (budget * steady_.u_pull);
        expected_val = (1.0 - p_free_miss) * v_free_up + p_free_miss * ev_normal_pull;
    } else {
        expected_val = ev_normal_pull;
    }

    memo[key] = expected_val;
    return expected_val;
}
