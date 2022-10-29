/**
 * @file generator.hpp
 * @author Uberariy
 * @brief 
 * @version 0.1
 * @date 2022-10-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef ANNEALING_GENERATOR_
#define ANNEALING_GENERATOR_

#include <random>
#include <iostream>
#include <fstream>

#include "exception.hpp"

/**
 * @brief Format is:
 * First line tells about number of processors used.
 * Second tells about number of works.
 * Next we determine time period for each work.
 * 
 * @param path 
 * @param procN 
 * @param workN 
 * @param min 
 * @param max 
 */
void generate_test_uniform(std::string path, unsigned long procN, unsigned long long workN, unsigned long long min, unsigned long long max) {
    if ((!procN) || (!workN) || (!min) || (!max)) {
        throw WrongParametersFormat("All the experiment input parameters must be possitive.");
    }

    /* Random engine */
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);

    /* Write in csv file*/

    std::ofstream of;
    of.open(path);
    if (of.is_open()) {
        of << "Processors," << procN << "\n";
        of << "Works," << workN << "\n";
        for (int i = 1; i <= workN; i++) {
            of << i << "," << distrib(gen) << "\n";
        }
    }
}

#endif