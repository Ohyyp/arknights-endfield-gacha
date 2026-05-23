// Copyright Ohyyp 2026
// SPDX-License-Identifier: MIT

#include <SteadyStateMath.h>

#include <cmath>
#include <vector>
#include <GachaMechanics.h>

SteadyStateResult SteadyStateMath::calculate_steady_state(const Banner &banner) {
    // e_pulls[p][h] = Expected pulls to get an UP character starting from p soft pity and h hard pity
    std::vector<std::vector<double>> e_pulls(81, std::vector<double>(120, 0.0));

    // e_spooks[p][h] = Expected number of spooks obtained before getting the UP character
    std::vector<std::vector<double>> e_spooks(81, std::vector<double>(120, 0.0));

    double base_prob = GachaMechanics::get_base_probability();
    double up_rate = GachaMechanics::get_up_rate();

    for (int h = 119; h >= 0; --h) {
        for (int p = 79; p >= 0; --p) {
            if (h == 119) {
                e_pulls[p][h] = 1.0;
                e_spooks[p][h] = 0.0;
            } else {
                double prob = GachaMechanics::get_probability(p);

                double expected_future_miss = e_pulls[p + 1][h + 1];
                double expected_future_spook = e_pulls[0][h + 1];

                double step_cost = 1.0;
                if (h == 59) step_cost -= 10.0; // 60-pull rebate

                int free_pulls = GachaMechanics::get_free_pulls(h);
                if (free_pulls > 0 && h == 29) { // 30-pull expedited recruitment
                    // Chance to miss UP in free rolls
                    double p_free_miss = std::pow(1.0 - base_prob * up_rate, 10);
                    expected_future_miss *= p_free_miss;
                    expected_future_spook *= p_free_miss;
                }

                e_pulls[p][h] = step_cost + (1.0 - prob) * expected_future_miss + prob * (1.0 - up_rate) * expected_future_spook;
                e_spooks[p][h] = (1.0 - prob) * e_spooks[p + 1][h + 1] + prob * (1.0 - up_rate) * (1.0 + e_spooks[0][h + 1]);
            }
        }
    }

    double u_pull = (banner.get_future_up_weight() + e_spooks[0][0] * banner.get_future_avg_spook_weight()) / e_pulls[0][0];

    std::vector<double> s_p(80, 0.0);

    for (int p = 0; p < 80; ++p) {
        double pulls_saved = e_pulls[0][0] - e_pulls[p][0];
        double spooks_lost = e_spooks[0][0] - e_spooks[p][0];

        // True utility residual value of starting at pity p
        double true_residual_utility = pulls_saved * u_pull - spooks_lost * banner.get_future_avg_spook_weight();

        // Convert back to "equivalent saved pulls" for internal use
        s_p[p] = true_residual_utility / u_pull;
    }

    return {u_pull, s_p, e_pulls[0][0]};
}
