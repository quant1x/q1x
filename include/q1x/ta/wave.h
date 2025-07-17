#pragma once
#ifndef QUANT1X_TECHNICAL_ANALYSIS_PATTERNS_WAVE_H
#define QUANT1X_TECHNICAL_ANALYSIS_PATTERNS_WAVE_H 1

#include <q1x/std/simd.h>
#include "trend.h"

#include <vector>

namespace ta::patterns {

    /**
     * @brief 检测波峰波谷
     * @param high 高点序列
     * @param low 低点序列
     * @return 包含波峰和波谷索引的pair, 波峰基于高价序列，波谷基于低价序列
     * @remark 如果只检测一个序列, 比如收盘价, high和low可以传入相同的close序列
     */
    std::pair<std::vector<data_point>, std::vector<data_point>> peaks_and_valleys(const xt::xarray<double> &high,
                                                                                  const xt::xarray<double> &low);

}  // namespace ta::patterns

#endif  // QUANT1X_TECHNICAL_ANALYSIS_PATTERNS_WAVE_H
