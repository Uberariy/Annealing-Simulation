/**
 * @file schedule.hpp
 * @author Uberariy
 * @brief 
 * @version 0.1
 * @date 2022-10-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef ANNEALING_SCHEDULE_
#define ANNEALING_SCHEDULE_

#include <vector>
#include <random>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

/* Schedules */

class Task_Solution {
public:
    virtual unsigned long long get_criterion() = 0;
private:

};

class Schedule : Task_Solution {
public:
    // Schedule(unsigned long procN_, const std::unordered_map<unsigned long, unsigned long> &time_dict_);
    Schedule(std::string path, bool random);
    unsigned long long get_criterion(); // Less => better
    unsigned long long get_procN() const;
    unsigned long long get_workN() const;
    // std::vector<std::vector<unsigned long>> get_proc_tasks();
    // std::vector<std::vector<unsigned long>>& set_proc_tasks();
    std::vector<std::vector<unsigned long>> proc_tasks;
private:
    unsigned long procN;
    unsigned long workN;
    std::unordered_map<unsigned long, unsigned long> time_dict;

};

// Schedule::Schedule(unsigned long procN_, const std::unordered_map<unsigned long, unsigned long> &time_dict_) : 
//     procN(procN_), time_dict(time_dict_) {}

Schedule::Schedule(std::string path, bool random = true) {
    /* Read from csv file*/

    std::stringstream ss;
    std::ifstream iff;
    iff.open(path);
    if (iff.is_open()) {
        std::string reader;
        getline(iff, reader);
        ss = std::stringstream(reader);
        getline(ss, reader, ',');
        if (reader == "Processors") {
            ss >> procN;
        }
        getline(iff, reader);
        ss = std::stringstream(reader);
        getline(ss, reader, ',');
        // std::cerr << reader;
        if (reader == "Works") {
            ss >> workN;
            // std::cerr << workN;
        }

        /* Random engine */
    
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, procN - 1);
        
        for (unsigned long i = 0; i < procN; i++) { 
            proc_tasks.push_back(std::vector<unsigned long>()); 
        }
        for (unsigned long i = 1; i <= workN; i++) {
            getline(iff, reader);
            ss = std::stringstream(reader);
            getline(ss, reader, ',');
            ss >> time_dict[i];
            // getline(ss, reader, ',');
            // std::cerr << reader << "|";
            // time_dict[i] = std::stoul(reader);
            // unsigned long proc = distrib(gen);
            if (random) {
                proc_tasks[distrib(gen)].push_back(i);
            } else {
                proc_tasks[0].push_back(i);
            }
            // std::cerr << i << "\t" << time_dict[i] << "\n";
        }
    }
    iff.close();
}

unsigned long long Schedule::get_criterion() {
    /* Second criterion implementation */
    unsigned long long result_time = 0;
    unsigned long long tmp_time;
    for (auto &proc : proc_tasks) {
        tmp_time = 0;
        for (auto &task : proc) {
            result_time += tmp_time + time_dict[task];
            tmp_time += time_dict[task];
            // std::cerr << "POINT1\n";
        }
    }
    return result_time;
}

unsigned long long Schedule::get_procN() const {
    return procN;
}

unsigned long long Schedule::get_workN() const {
    return workN;
}

// std::vector<std::vector<unsigned long>>& Schedule::set_proc_tasks() {
//     return proc_tasks;
// }

/* Mutations */

template<typename TSchedule>
class Mutation {
public:
    virtual TSchedule mutate(TSchedule schedule) = 0;
};

template<typename TSchedule>
class MyMutation : Mutation<TSchedule> {
public:
    TSchedule mutate(TSchedule schedule) {
        if (schedule.get_procN() == 1) {
            return schedule;
        }

        /* Random engine */

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> work_distrib(1, schedule.get_workN());
        std::uniform_int_distribution<> proc_distrib(0, schedule.get_procN() - 1);

        /* Rearrange 10% of works */

        for (unsigned long i = 1; i <= schedule.get_workN() / 10; i++) {
            unsigned long moving_work = work_distrib(gen);
            // std::cerr << "p" << moving_work << "p";
            for (unsigned long j = 0; j < schedule.get_procN(); j++) {
                // if (std::find(schedule.proc_tasks[j].begin(), schedule.proc_tasks[j].end(), moving_work) != schedule.proc_tasks[j].end()) {
                //     auto it = schedule.proc_tasks[j].begin();
                //     while (it != schedule.proc_tasks[j].end()) {
                //         if (*it == moving_work) {
                //             it = schedule.proc_tasks[j].erase(it);
                //             break;
                //         }
                //         else {
                //             ++it;
                //         }
                //     }
                // }
                if (std::find(schedule.proc_tasks[j].begin(), schedule.proc_tasks[j].end(), moving_work) != schedule.proc_tasks[j].end()) {
                    remove(schedule.proc_tasks[j].begin(), schedule.proc_tasks[j].end(), moving_work);
                    schedule.proc_tasks[j].pop_back();
                    // if (std::find(schedule.proc_tasks[j].begin(), schedule.proc_tasks[j].end(), moving_work) != schedule.proc_tasks[j].end()) {
                    //     std::cerr << "POINT";
                    // }
                    // std::cerr << "POINT";
                    break;
                }
            }
            unsigned long target_proc;
            // while (true) {
            target_proc = proc_distrib(gen);
            if (schedule.proc_tasks[target_proc].size() == 0) {
                schedule.proc_tasks[target_proc].push_back(moving_work);
            } else {
                std::uniform_int_distribution<> proc_work_distrib(0, schedule.proc_tasks[target_proc].size());
                unsigned long target_proc_work = proc_work_distrib(gen);
                // std::cerr << "p" << target_proc_work << "p";
                if (schedule.proc_tasks[target_proc].size() == target_proc_work) {
                    schedule.proc_tasks[target_proc].push_back(moving_work);
                }
                // std::cerr << "p" << target_proc_work << "p";
                schedule.proc_tasks[target_proc].push_back(schedule.proc_tasks[target_proc][target_proc_work]);
                schedule.proc_tasks[target_proc][target_proc_work] = moving_work;
            }
            // }
            // unsigned long target_proc = proc_distrib(gen);
            // std::uniform_int_distribution<> proc_work_distrib(0, schedule.proc_tasks[target_proc].size() - 1);
            // unsigned long target_proc_work = proc_work_distrib(gen);
            // // std::cerr << "p" << target_proc_work << "p";
            // schedule.proc_tasks[target_proc].push_back(schedule.proc_tasks[target_proc][target_proc_work]);
            // schedule.proc_tasks[target_proc][target_proc_work] = moving_work;
        }
        // std::cout << "|" << schedule.get_criterion() << "|";

        return schedule;
    }
};

#endif