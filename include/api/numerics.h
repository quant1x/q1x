#pragma once
#ifndef API_NUMERICS_H
#define API_NUMERICS_H 1

#include <api/std.h>
#include <ostream>

namespace numerics {

    /**
     * @brief 适用于高频高精度的银行家四舍五入
     * @param value double类型value
     * @param digits 保留几位小数点
     * @return 银行家四舍五入的double
     */
    f64 decimal(f64 value, int digits = 2);

    inline bool is_equal(f64 a, f64 b, f64 epsilon = 1e-10) {
        return std::fabs(a - b) < epsilon;
    }

    inline f64 change_rate(f64 base, f64 current) {
        return current/base;
    }

    inline f64 net_change_rate(f64 base, f64 current) {
        auto chg = change_rate(base, current);
        return (chg - 1.00) * 100.00;
    }

    /**
     * @brief 数值范围
     * @tparam T
     */
    template<typename T>
    struct number_range {
        T min_;
        T max_;

        // 默认构造：[lowest, max]
        constexpr number_range()
            : min_(std::numeric_limits<T>::lowest())
            , max_(std::numeric_limits<T>::max()) {}

        // 双参数构造：[min, max]
        constexpr number_range(T min, T max)
            : min_(min), max_(max) {}

        // 单参数构造：[min, max()]
        constexpr explicit number_range(T min)
            : number_range(min, std::numeric_limits<T>::max()) {}

        // 新增：从字符串构造（无异常）
        number_range(const std::string& str) {
            min_ = std::numeric_limits<T>::lowest();
            max_ = std::numeric_limits<T>::max();

            std::string text = strings::trim(str);
            size_t pos = text.find('~');

            if (pos == std::string::npos) {
                // 情况1: 无分隔符，视为最小值
                T val = strings::from_string(text, min_);
                min_ = val;
            } else {
                std::string s_min = strings::trim(text.substr(0, pos));
                std::string s_max = strings::trim(text.substr(pos + 1));

                // 情况4: 前后都为空
                if (s_min.empty() && s_max.empty()) {
                    // 已经是默认值
                    return;
                }

                // 情况2: 前空，后为最大值
                if (s_min.empty()) {
                    T val = strings::from_string(s_max, max_);
                    max_ = val;
                }
                    // 情况3: 后空，前为最小值
                else if (s_max.empty()) {
                    T val = strings::from_string(s_min, min_);
                    min_ = val;
                }
                    // 情况5: 前后都有值
                else {
                    T val_min = strings::from_string(s_min, min_);
                    T val_max = strings::from_string(s_max, max_);
                    min_ = val_min;
                    max_ = val_max;
                }
            }
        }
    public:
        // 验证值是否在范围内
        bool validate(double v) const {
            if (min_ == 0 && max_ == 0) {
                return true;
            }
            return v >= min_ && v < max_;
        }

        // 字符串表示
        std::string to_string() const {
            return "{min:" + std::to_string(min_) + ", max:" + std::to_string(max_) + "}";
        }

        friend std::ostream &operator<<(std::ostream &os, const number_range &range) {
            os << range.to_string();
            return os;
        }
    };
}

#endif // API_NUMERICS_H
