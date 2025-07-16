#include <q1x/ta/wave.h>

int compare(double a, double b) {
    if (a < b) return -1;
    else if (a > b) return 1;
    else return 0;
}

// 主函数：寻找波峰和波谷
std::pair<std::vector<int>, std::vector<int>> basic_peaks_and_valleys(const std::vector<double>& high, const std::vector<double>& low) {
    /**
     * 输入: high - 高价序列, low - 低价序列
     * 返回: 包含波峰和波谷索引的pair
     * 波峰基于高价序列，波谷基于低价序列
     */

    size_t n = high.size();
    std::vector<int> diff_high(n, 0);  // 高价差分序列
    std::vector<int> diff_low(n, 0);   // 低价差分序列

    // 第一步：计算一阶差分
    for (size_t i = 0; i < n - 1; ++i) {
        diff_high[i] = compare(high[i + 1], high[i]);
        diff_low[i] = compare(low[i + 1], low[i]);
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
        int d_low = diff_low[i + 1] - diff_low[i];

        if (d_high == -2) {  // 高价序列由上升到下降，形成波峰
            peaks.push_back(i + 1);
        }
        if (d_low == 2) {    // 低价序列由下降到上升，形成波谷
            valleys.push_back(i + 1);
        }
    }

    return make_pair(peaks, valleys);
}

std::pair<std::vector<int>, std::vector<int>> peaks_and_valleys(
    const xt::xarray<double>& high,
    const xt::xarray<double>& low)
{
    // 确保输入大小一致
    if (high.shape() != low.shape()) {
        throw std::invalid_argument("High and low arrays must have same size");
    }

    size_t n = high.size();

    // 第一步：计算一阶差分 (向量化操作)
    xt::xarray<int> diff_high = xt::zeros<int>({n});
    xt::xarray<int> diff_low = xt::zeros<int>({n});

    // 计算差分 (更高效的向量化方式)
    xt::view(diff_high, xt::range(0, n-1)) = xt::sign(xt::view(high, xt::range(1, n)) - xt::view(high, xt::range(0, n-1)));
    xt::view(diff_low, xt::range(0, n-1)) = xt::sign(xt::view(low, xt::range(1, n)) - xt::view(low, xt::range(0, n-1)));

    // 第二步：处理平台区域 (向量化处理)
    auto process_plateaus = [](xt::xarray<int>& diff) {
        size_t n = diff.size();

        // 前向填充非零值
        int last_non_zero = 0;
        for (size_t i = 0; i < n; ++i) {
            if (diff[i] != 0) {
                last_non_zero = diff[i];
            } else if (last_non_zero != 0) {
                diff[i] = last_non_zero;
            }
        }

        // 反向填充剩余零值
        last_non_zero = 0;
        for (size_t i = n; i-- > 0; ) {
            if (diff[i] != 0) {
                last_non_zero = diff[i];
            } else if (last_non_zero != 0) {
                diff[i] = last_non_zero;
            }
        }
    };

    process_plateaus(diff_high);
    process_plateaus(diff_low);

    // 第三步：识别波峰和波谷 (向量化操作)
    auto d_high = xt::view(diff_high, xt::range(1, n)) - xt::view(diff_high, xt::range(0, n-1));
    auto d_low = xt::view(diff_low, xt::range(1, n)) - xt::view(diff_low, xt::range(0, n-1));

    // 找出满足条件的索引
    auto peak_indices = xt::flatten_indices(xt::where(xt::equal(d_high, -2)));
    auto valley_indices = xt::flatten_indices(xt::where(xt::equal(d_low, 2)));

    // 转换为std::vector并调整索引(+1)
    std::vector<int> peaks(peak_indices.begin(), peak_indices.end());
    std::vector<int> valleys(valley_indices.begin(), valley_indices.end());

    for (auto& idx : peaks) ++idx;
    for (auto& idx : valleys) ++idx;

    return {peaks, valleys};
}