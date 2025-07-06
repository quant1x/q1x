#pragma once
#ifndef QUANT1X_STD_SAFE_H
#define QUANT1X_STD_SAFE_H 1

#include <ctime>

namespace q1x {
    std::tm safe_localtime(std::time_t t) noexcept;
    std::tm safe_gmtime(std::time_t t) noexcept;
}  // namespace q1x

#endif  // QUANT1X_STD_SAFE_H