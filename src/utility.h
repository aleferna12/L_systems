//
// Created by aleferna on 06/06/24.
//

#ifndef L_SYSTEMS_UTILITY_H
#define L_SYSTEMS_UTILITY_H

#include <random>

extern std::mt19937 rng;
extern std::uniform_real_distribution<double> uniform_random;

double vec_mean(const std::vector<double> &vec);

double vec_variance(const std::vector<double> &vec);

std::string vec_to_str(const std::vector<std::string> &vec, const std::string &sep);

#endif //L_SYSTEMS_UTILITY_H
