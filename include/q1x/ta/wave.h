#pragma once
#ifndef QUANT1X_TECHNICAL_ANALYSIS_PATTERNS_WAVE_H
#define QUANT1X_TECHNICAL_ANALYSIS_PATTERNS_WAVE_H 1

#include <q1x/std/simd.h>

#include <vector>

#include "trend.h"

namespace ta::patterns {

    template<typename T>
    inline int compare(const T& a, const T& b) {
        if (a < b)
            return -1;
        else if (a > b)
            return 1;
        else
            return 0;
    }

    /**
     * @brief 检测波峰波谷
     * @param high 高点序列
     * @param low 低点序列
     * @return 包含波峰和波谷索引的pair, 波峰基于高价序列，波谷基于低价序列
     * @remark 如果只检测一个序列, 比如收盘价, high和low可以传入相同的close序列
     */
    template<typename T>
    std::pair<std::vector<int>, std::vector<int>> basic_peaks_and_valleys(const std::vector<T> &high,
                                                                          const std::vector<T> &low) {
        size_t           n = high.size();
        std::vector<int> diff_high(n, 0);  // 高价差分序列
        std::vector<int> diff_low(n, 0);   // 低价差分序列

        // 第一步：计算一阶差分
        for (size_t i = 0; i < n - 1; ++i) {
            diff_high[i] = compare(high[i + 1], high[i]);
            diff_low[i]  = compare(low[i + 1], low[i]);
        }

        // 第二步：处理平台区域（差分为0的情况）
        for (size_t i = 0; i < n - 1; ++i) {
            // 处理高价序列的平台
            if (diff_high[i] == 0) {
                if (i == 0) {  // 如果是第一个点
                    for (size_t j = i + 1; j < n - 1; ++j) {
                        if (diff_high[j] != 0) {
                            diff_high[i] = diff_high[j];
                            break;
                        }
                    }
                } else if (i == n - 2) {  // 如果是最后一个点
                    diff_high[i] = diff_high[i - 1];
                } else {  // 中间点
                    diff_high[i] = diff_high[i + 1];
                }
            }

            // 处理低价序列的平台
            if (diff_low[i] == 0) {
                if (i == 0) {
                    for (size_t j = i + 1; j < n - 1; ++j) {
                        if (diff_low[j] != 0) {
                            diff_low[i] = diff_low[j];
                            break;
                        }
                    }
                } else if (i == n - 2) {
                    diff_low[i] = diff_low[i - 1];
                } else {
                    diff_low[i] = diff_low[i + 1];
                }
            }
        }

        // 第三步：识别波峰和波谷
        std::vector<int> peaks;    // 波峰索引
        std::vector<int> valleys;  // 波谷索引

        for (size_t i = 0; i < n - 1; ++i) {
            int d_high = diff_high[i + 1] - diff_high[i];
            int d_low  = diff_low[i + 1] - diff_low[i];

            int index = int(i) + 1;  // 波峰和波谷的索引是i+1
            if (d_high == -2) {      // 高价序列由上升到下降，形成波峰
                peaks.push_back(index);
            }
            if (d_low == 2) {  // 低价序列由下降到上升，形成波谷
                valleys.push_back(index);
            }
        }

        return make_pair(peaks, valleys);
    }

    /**
     * @brief 检测波峰波谷
     * @param high 高点序列
     * @param low 低点序列
     * @return 包含波峰和波谷索引的pair, 波峰基于高价序列，波谷基于低价序列
     * @remark 如果只检测一个序列, 比如收盘价, high和low可以传入相同的close序列
     */
    std::pair<std::vector<point>, std::vector<point>> peaks_and_valleys(const xt::xarray<double> &high,
                                                                        const xt::xarray<double> &low);

}  // namespace ta::patterns

#endif  // QUANT1X_TECHNICAL_ANALYSIS_PATTERNS_WAVE_H
