/**
 * @file temperature.hpp
 * @author Uberariy
 * @brief 
 * @version 0.1
 * @date 2022-10-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef ANNEALING_TEMPERATURE_
#define ANNEALING_TEMPERATURE_

#include <stdio.h>
#include <cmath>

/* Laws */

class Change_Temperature {
public:
    virtual double algorithm_step(double start_temp_, unsigned long iter_) = 0;
};

class Boltzman : Change_Temperature {
public:
    double algorithm_step(double start_temp_, unsigned long iter_);
};

class Cauchy : Change_Temperature {
public:
    double algorithm_step(double start_temp_, unsigned long iter_);
};

class NatLog : Change_Temperature {
public:
    double algorithm_step(double start_temp_, unsigned long iter_);
};

double Boltzman::algorithm_step(double start_temp_, unsigned long iter_) {
    return start_temp_ / std::log(iter_ + 1);
}

double Cauchy::algorithm_step(double start_temp_, unsigned long iter_) {
    return start_temp_ / (iter_ + 1);
}

double NatLog::algorithm_step(double start_temp_, unsigned long iter_) {
    return start_temp_ * std::log(iter_ + 1) / (iter_ + 1);
}

/* Temperature */

enum class LawType {
    BOLTZMAN,
    CAUCHY,
    NATLOG,
};

class Temperature {
public:
    Temperature(LawType law_type_, double start_temp_ = 0);
    void temperature_step(unsigned long iter_);

private:
    double start_temp;
    double current_temp;

    LawType law_type;
    Boltzman boltzman_law;
    Cauchy cauchy_law;
    NatLog natlog_law;
};

Temperature::Temperature(LawType law_type_, double start_temp_ = 0) :
    law_type(law_type_), start_temp(start_temp_), current_temp(start_temp_) {}

void Temperature::temperature_step(unsigned long iter_) {
    if (law_type == LawType::BOLTZMAN)
        current_temp = boltzman_law.algorithm_step(start_temp, iter_);
    else if (law_type == LawType::CAUCHY)
        current_temp = cauchy_law.algorithm_step(start_temp, iter_);
    else if (law_type == LawType::NATLOG)
        current_temp = natlog_law.algorithm_step(start_temp, iter_);
}

#endif