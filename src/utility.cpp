//
// Created by aleferna on 06/06/24.
//

#include <algorithm>
#include <sstream>
#include "utility.h"

std::mt19937 rng(std::random_device{}());
std::uniform_real_distribution<> uniform_random(0, 1);

double vec_mean(const std::vector<double> &vec) {
    double sum = std::accumulate(vec.begin(), vec.end(), 0.0);
    return sum / (double) vec.size();
}

double vec_variance(const std::vector<double> &vec) {
    auto mean = vec_mean(vec);
    auto un_op = [mean](double x) { return x - mean; };
    std::vector<double> diff(vec.size());
    std::transform(vec.begin(), vec.end(), diff.begin(), un_op);
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    return std::sqrt(sq_sum / (double) vec.size());
}

std::string vec_to_str(const std::vector<std::string> &vec, const std::string &sep) {
    std::ostringstream result;
    for (auto it = vec.begin(); it != vec.end() - 1; it++) {
        result << *it;
        result << sep;
    }
    result << vec.back();
    return result.str();
}
