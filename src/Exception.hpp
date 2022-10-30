/**
 * @file exception.hpp
 * @author Uberariy
 * @brief 
 * @version 0.1
 * @date 2022-10-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef ANNEALING_EXCEPT_
#define ANNEALING_EXCEPT_

#include <exception>
#include <string>

class WrongParametersFormat : std::exception {
private:
    const char* message;

public:
    WrongParametersFormat() : message("The parameters of a class or function are wrong!") {}
    explicit WrongParametersFormat(const char* msg) : message(msg) {}
    explicit WrongParametersFormat(const std::string& msg) : message(msg.c_str()) {}
    const char* what () {
        return message;
    }
};

#endif