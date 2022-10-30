/**
 * @file annealing.hpp
 * @author Uberariy
 * @brief 
 * @version 0.1
 * @date 2022-10-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef ANNEALING_ANNEALING_
#define ANNEALING_ANNEALING_

#include <vector>
#include <unordered_map>

#include "Schedule.hpp"
#include "Temperature.hpp"

/* Algorithm implementation */

template<typename TSchedule>
class Annealing_Simulation {
public:
    Annealing_Simulation(TSchedule start_schedule_, Temperature start_temp_, unsigned long iter_) :
    start_schedule(start_schedule_), best_schedule(start_schedule_), current_schedule(start_schedule_), 
    temperature(start_temp_), iterations(iter_) {}

    TSchedule algorithm() {
        unsigned long boredom_current = 0;
        unsigned long iter_current = 0;

        unsigned long long current_criterion = current_schedule.get_criterion();
        unsigned long long best_criterion = best_schedule.get_criterion();
        unsigned long long tmp_criterion;

        MyMutation<Schedule> mutation;

        while (boredom_current < boredom_constraint) {
            current_schedule = best_schedule; // Modification
            iter_current++;
            for (int i = 0; i < iterations; i++) {
                boredom_current++;
                TSchedule tmp_schedule = mutation.mutate(current_schedule);
                tmp_criterion = tmp_schedule.get_criterion();

                if ((tmp_criterion <= current_criterion) ||
                    (temperature.is_better_enough((long long)tmp_criterion - (long long)current_criterion))) {
                    if (tmp_criterion < best_criterion) {
                        best_schedule = tmp_schedule;
                        best_criterion = best_schedule.get_criterion();
                        boredom_current = 0;
                    }
                    current_schedule = tmp_schedule;
                    current_criterion = current_schedule.get_criterion();
                }
            }
            std::cerr << "Temperature: " << temperature.get_current_temp() << "\tIter: " << iter_current << "  \tCriterion: " << best_criterion << "\n";
            temperature.temperature_step(iter_current);
        }

        return best_schedule;
    }
private:
    const unsigned long boredom_constraint = 100;
    unsigned long iterations;

    TSchedule start_schedule;
    TSchedule current_schedule;
    TSchedule best_schedule;

    Temperature temperature;

};

#endif