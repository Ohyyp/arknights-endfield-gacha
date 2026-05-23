// Copyright Ohyyp 2026
// SPDX-License-Identifier: MIT

#include <MonteCarloEngine.h>
#include <SteadyStateMath.h>
#include <GachaMechanics.h>

#include <random>

MonteCarloEngine::MonteCarloEngine(int iterations) : iterations_(iterations) {}

SimulationResult MonteCarloEngine::run(const GachaConfig &config, const PlayerStatus &start_status, int budget) {
    auto steady = SteadyStateMath::calculate_steady_state(config.get_banner());

    std::random_device rd;
    std::mt19937_64 rng(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    double total_utility = 0.0;

    for (int i = 0; i < iterations_; ++i) {
        int p = start_status.soft_pity;
        int h = start_status.hard_pity;
        int b = budget;
        double current_utility = 0.0;

        while (b > 0) {
            double w_up = config.get_weight(config.get_banner().get_rate_up());
            double w_spook = config.get_avg_spook_weight();
            double base_prob = GachaMechanics::get_base_probability();
            double up_rate = GachaMechanics::get_up_rate();

            if (h == 29) {
                bool got_free_up = false;
                for (int j = 0; j < 10; ++j) {
                    if (dist(rng) < base_prob) {
                        if (dist(rng) < up_rate) {
                            got_free_up = true;
                            break;
                        }
                    }
                }
                if (got_free_up) {
                    current_utility += w_up;
                    p = 0;
                    h = 0;
                    break;
                }
            }

            if (h == 59) {
                b += 10;
            }

            b--;

            bool is_up = false;
            bool is_spook = false;

            if (h == 119) {
                is_up = true;
            } else {
                double prob = GachaMechanics::get_probability(p);

                if (dist(rng) < prob) {
                    if (dist(rng) < up_rate)
                        is_up = true;
                    else
                        is_spook = true;
                }
            }

            if (is_up) {
                current_utility += w_up;
                p = 0;
                h = 0;
                break;
            } else if (is_spook) {
                current_utility += w_spook;
                p = 0;
                h++;
            } else {
                p++;
                h++;
            }
        }

        double residual = (b * steady.u_pull) + (steady.s_p[p] * steady.u_pull);
        total_utility += current_utility + residual;
    }

    double res = total_utility / iterations_;
    double base_utility = (budget * steady.u_pull) + (steady.s_p[start_status.soft_pity] * steady.u_pull);
    
    return { res, base_utility, steady.e_cost };
}
