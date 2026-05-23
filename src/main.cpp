// Copyright Ohyyp 2026
// SPDX-License-Identifier: MIT

#include <ConfigLoader.h>
#include <DynamicProgrammingEngine.h>
#include <MonteCarloEngine.h>

#include <CLI/CLI.hpp>

#include <iostream>
#include <memory>

int main(int argc, char **argv) {
    CLI::App app{"Arknights: Endfield Gacha Planner"};

    std::string config_path = "config.toml";
    int soft_pity = 0;
    int hard_pity = 0;
    int budget = 100;
    std::string engine = "dp";
    int iterations = 100000;

    app.add_option("-c,--config", config_path, "Path to the TOML configuration file");
    app.add_option("--soft-pity", soft_pity, "Current soft pity count (0-79)")->check(CLI::Range(0, 79));
    app.add_option("--hard-pity", hard_pity, "Current banner pulls for hard pity tracking (0-119)")->check(CLI::Range(0, 119));
    app.add_option("--budget", budget, "Max pulls budget")->check(CLI::NonNegativeNumber);
    app.add_option("--engine", engine, "Calculation Engine: 'mc' or 'dp'")->check(CLI::IsMember({"dp", "mc"}));
    app.add_option("--iterations", iterations, "Iterations for Monte Carlo engine")->check(CLI::PositiveNumber);

    CLI11_PARSE(app, argc, argv);

    try {
        GachaConfig config = ConfigLoader::Load(config_path);
        PlayerStatus status = {soft_pity, hard_pity};

        std::cout << "=======================================\n";
        std::cout << " Arknights: Endfield Gacha Planner\n";
        std::cout << "=======================================\n";
        std::cout << " [Inputs]\n";
        std::cout << "  Engine    : " << (engine == "mc" ? "Monte Carlo" : "Dynamic Programming") << "\n";
        std::cout << "  Soft Pity : " << soft_pity << "\n";
        std::cout << "  Hard Pity : " << hard_pity << " (Banner Hard Pity Tracking)\n";
        std::cout << "  Budget    : " << budget << "\n";
        std::cout << "---------------------------------------\n";

        std::unique_ptr<GachaEngine> selected_engine;
        if (engine == "mc") {
            selected_engine = std::make_unique<MonteCarloEngine>(iterations);
        } else {
            selected_engine = std::make_unique<DynamicProgrammingEngine>();
        }

        SimulationResult res = selected_engine->run(config, status, budget);

        double incremental_utility = res.total_expected_utility - res.base_utility;

        std::cout << " [Results]\n";
        std::cout << "  Raw Expected Pulls to UP  : " << res.raw_expected_pulls << " (Pure mathematical expectation without budget limits/free pulls)\n";
        std::cout << "  Base Utility (Net Worth)  : " << res.base_utility << " (Intrinsic value of your initial budget + pity)\n";
        std::cout << "  Total Expected Utility    : " << res.total_expected_utility << "\n";

        if (budget > 0) {
            double efficiency = incremental_utility / budget;
            double pulls_per_1_0 = (efficiency > 0.0) ? (1.0 / efficiency) : 0.0;
            std::cout << "  Net Gained Utility        : " << incremental_utility << "\n";
            std::cout << "  Utility per Pull          : " << efficiency << "\n";
            std::cout << "  Pulls per 1.0 Utility     : " << pulls_per_1_0 << " (Pulls to get 1.0 equivalent UP)\n";
        }
        std::cout << "=======================================\n";
    } catch (const std::exception &e) {
        std::cerr << "Fatal runtime error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
