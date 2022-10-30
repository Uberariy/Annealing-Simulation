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
    generate_test_uniform("../input_example/test1.csv", 1000, 20000, 5, 40);

    Schedule StartSchedule("../input_example/test1.csv");

    auto t_start = std::chrono::high_resolution_clock::now();

    Temperature StartTemp(LawType::BOLTZMAN, 239.9);
    Annealing_Simulation<Schedule> AS(StartSchedule, StartTemp, 20);
    Schedule FinalSchedule = AS.algorithm();

    auto t_end = std::chrono::high_resolution_clock::now();
    auto t_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t_end - t_start).count();

    std::cout << "Duration:" << t_duration / 1e9 << ",StartCriterion:" << StartSchedule.get_criterion() << ",FinalCriterion:" << FinalSchedule.get_criterion() << "\n";

    return 0;
}