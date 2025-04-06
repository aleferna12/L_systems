//
// Created by aleferna on 06/06/24.
//

#include <algorithm>
#include <sstream>
#include "utility.h"

std::uniform_real_distribution<> uniform_random(0, 1);

float vecMean(const std::vector<float> &vec) {
    float sum = std::accumulate(vec.begin(), vec.end(), 0.0);
    return sum / (float) vec.size();
}

float vecVariance(const std::vector<float> &vec) {
    auto mean = vecMean(vec);
    auto un_op = [mean](float x) { return x - mean; };
    std::vector<float> diff(vec.size());
    std::transform(vec.begin(), vec.end(), diff.begin(), un_op);
    float sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    return std::sqrt(sq_sum / (float) vec.size());
}

std::string vecToStr(const std::vector<std::string> &vec, const std::string &sep) {
    if (vec.empty())
        return "";

    std::ostringstream result;
    for (auto it = vec.begin(); it != vec.end() - 1; it++) {
        result << *it;
        result << sep;
    }
    result << vec.back();
    return result.str();
}
