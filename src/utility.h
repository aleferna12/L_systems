//
// Created by aleferna on 06/06/24.
//

#ifndef L_SYSTEMS_UTILITY_H
#define L_SYSTEMS_UTILITY_H

#include <random>

extern std::uniform_real_distribution<> uniform_random;

double vecMean(const std::vector<double> &vec);

double vecVariance(const std::vector<double> &vec);

std::string vecToStr(const std::vector<std::string> &vec, const std::string &sep);

#endif //L_SYSTEMS_UTILITY_H
