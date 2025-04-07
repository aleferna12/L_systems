//
// Created by aleferna on 06/06/24.
//

#ifndef L_SYSTEMS_UTILITY_H
#define L_SYSTEMS_UTILITY_H

#include <vector>
#include <random>

extern std::uniform_real_distribution<> uniform_random;

float vecMean(const std::vector<float> &vec);

float vecVariance(const std::vector<float> &vec);

std::string vecToStr(const std::vector<std::string> &vec, const std::string &sep);

void print_memory_info();

#endif //L_SYSTEMS_UTILITY_H
