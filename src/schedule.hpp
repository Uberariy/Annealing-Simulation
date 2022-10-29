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
#include <unordered_map>

class Task_Solution {
public:

private:
    unsigned long procN;
    unsigned long workN;
    std::vector<std::vector<unsigned long>> schedule;
    std::unordered_map<unsigned long, unsigned long> time_dict;

};

class Schedule : Task_Solution {
public:

private:

};

#endif