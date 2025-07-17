#pragma once
#ifndef QUANT1X_TECHNICAL_ANALYSIS_TREND_H
#define QUANT1X_TECHNICAL_ANALYSIS_TREND_H 1

#include <q1x/std/api.h>

struct data_point {
    int x;
    f64 y;
public:
    std::string to_string() const {
        return fmt::format("{{x={},y={}}}", x, y);
    }
};

#endif //QUANT1X_TECHNICAL_ANALYSIS_TREND_H
