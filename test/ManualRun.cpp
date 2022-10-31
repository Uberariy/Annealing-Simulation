/**
 * @file manual_test.cpp
 * @author Uberariy
 * @brief 
 * @version 0.1
 * @date 2022-10-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <chrono>
#include <ctime>

#include "../src/Generator.hpp"
#include "../src/Schedule.hpp"
#include "../src/Annealing.hpp"

int main() {
    std::vector<unsigned long> processors = {2, 4, 6, 8, 12, 16, 24, 32, 48, 64, 96, 128};
    std::vector<unsigned long> works = {25000, 20000, 15000, 10000, 7500, 5000, 2500, 1000, 750, 500, 250, 100};

    for (int p = 0; p < 4; p++) {
        for (auto &i: processors) {
            for (auto &j: works) {
                generate_test_uniform("../input_example/test1.csv", i, j, 5, 25);

                Schedule StartSchedule("../input_example/test1.csv", false);

                auto t_start = std::chrono::high_resolution_clock::now();

                Temperature StartTemp(LawType::BOLTZMAN, 232);
                Annealing_Simulation<Schedule> AS(StartSchedule, StartTemp, 20);
                Schedule FinalSchedule = AS.algorithm();

                auto t_end = std::chrono::high_resolution_clock::now();
                auto t_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t_end - t_start).count();

                std::cout << "Processors:" << i << ",Works:" << j << ",Duration:" << t_duration / 1e9 << ",StartCriterion:" << StartSchedule.get_criterion() << ",FinalCriterion:" << FinalSchedule.get_criterion() << "\n";
            }
        }
    }
    return 0;
}