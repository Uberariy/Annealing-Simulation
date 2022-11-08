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

#include <unordered_map>
#include <vector>
#include <thread>
#include <future>
#include <mutex>

#include "Schedule.hpp"
#include "Temperature.hpp"

/* Algorithm implementation */

template<typename TSchedule>
class Annealing_Simulation {
public:
    Annealing_Simulation(TSchedule start_schedule_, Temperature start_temp_, unsigned long iter_) :
    start_schedule(start_schedule_), best_schedule(start_schedule_), current_schedule(start_schedule_), 
    temperature(start_temp_), iterations(iter_) {}

    int multy_algorithm(std::mutex& sch_mutex, TSchedule& mutex_best_schedule, unsigned int int_name) {
        algorithm(int_name);
        sch_mutex.lock();
        if (mutex_best_schedule.get_criterion() > best_schedule.get_criterion()) {
            mutex_best_schedule = best_schedule;
        }
        sch_mutex.unlock();
        return 0;
    }

    TSchedule algorithm(unsigned int int_name = 0) {
        unsigned long boredom_current = 0;
        unsigned long iter_current = 0;

        unsigned long long current_criterion = current_schedule.get_criterion();
        unsigned long long best_criterion = best_schedule.get_criterion();
        unsigned long long tmp_criterion;

        MyMutation<Schedule> mutation;

        while (boredom_current < boredom_constraint) {
            // current_schedule = best_schedule; // Modification
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
            if (int_name != 0) std::cerr << "Process NUMBER: " << int_name << "\t";
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

template<typename TSchedule>
TSchedule Parallel_Simulation(TSchedule start_schedule_, Temperature start_temp_, unsigned long iter_, const unsigned long boredom_parallel_constraint_, unsigned int Nproc_) {
    unsigned long current_boredom_parallel = 0;

    TSchedule start_schedule = start_schedule_;
    TSchedule mutex_best_schedule = start_schedule_;
    std::mutex sch_mutex;

    while (current_boredom_parallel < boredom_parallel_constraint_) {
        std::vector<Annealing_Simulation<TSchedule>> simulations_vec;
        std::vector<std::thread> thread_vec;

        for (unsigned int i = 0; i < Nproc_; i++) {
            simulations_vec.push_back(Annealing_Simulation<TSchedule>(start_schedule, start_temp_, iter_));
        }

        for (unsigned int i = 0; i < Nproc_; i++) {
            thread_vec.push_back(std::thread {
                [&mutex_best_schedule, &simulations_vec, &sch_mutex, i] {
                    simulations_vec[i].multy_algorithm(sch_mutex, mutex_best_schedule, i+1); 
                }
            });
        }

        for (unsigned int i = 0; i < Nproc_; i++) {
            thread_vec[i].join();
        }

        if (start_schedule.get_criterion() == mutex_best_schedule.get_criterion()) {
            current_boredom_parallel++;
        } else {
            current_boredom_parallel = 0;
            start_schedule = mutex_best_schedule;
        }

        std::cerr << "Parallel iteration. Current best criterion: " << mutex_best_schedule.get_criterion() << "\tBoredom: " << current_boredom_parallel << "\n";
    }

    return mutex_best_schedule;
}

#endif