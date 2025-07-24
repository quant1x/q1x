#include <q1x/test/test.h>

#include <q1x/ta/wave.h>

TEST_CASE("patterns-wave-v1", "[release]") {
    // 创建测试数据
    xt::xarray<double> high = {1.0, 1.1, 1.2, 1.1, 1.0, 1.2, 1.3, 1.2};
    xt::xarray<double> low = {0.9, 1.0, 1.1, 1.0, 0.9, 1.1, 1.2, 1.1};

    auto [peaks, valleys] = ta::patterns::peaks_and_valleys(high, low);
    // 输出结果
    std::cout << "波峰位置: ";
    for (auto p : peaks) {
        std::cout << p.x << " ";
    }
    std::cout << "\n波谷位置: ";
    for (auto v : valleys) {
        std::cout << v.x << " ";
    }
}


#include <q1x/factors/base.h>
#include <q1x/dataframe/dataframe.h>
#include <q1x/std/format.h>
#include <span>
#include <ranges>

// 线性回归模型
struct LinearRegressionModel {
    double slope;     // a: 斜率
    double intercept; // b: 截距

    double predict(double x) const {
        return slope * x + intercept;
    }

    std::string to_string() const {
        return fmt::format("y = {:.4f} * x + {:.4f}", slope, intercept);
    }
};

// 线性回归拟合
LinearRegressionModel linear_regression(const std::vector<ta::point>& points) {
    int n = int(points.size());
    if (n < 2) {
        return {0.0, 0.0}; // 数据不足
    }

    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_xx = 0.0;

    for (const auto& pt : points) {
        double x = pt.x;
        double y = pt.y;
        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_xx += x * x;
    }

    double denominator = n * sum_xx - sum_x * sum_x;
    if (std::abs(denominator) < 1e-9) {
        return {0.0, sum_y / n}; // 垂直线或数据垂直
    }

    double a = (n * sum_xy - sum_x * sum_y) / denominator;
    double b = (sum_y - a * sum_x) / n;

    return {a, b};
}

TEST_CASE("patterns-wave-v2", "[release]") {
    std::string code = "sz300773";
    std::string date = "2025-07-15";
    int N = 50;
    auto klines = factors::checkout_klines(code, date);
    DataFrame df = DataFrame::from_struct_vector(klines);
    auto const& col_high = df.get<f64>("high");
    const xt::xarray<f64>& HIGH = xt::adapt(col_high);
    auto high = xt::view(HIGH, xt::range(HIGH.shape()[0] - N, HIGH.shape()[0]), xt::all());
    auto const& col_low = df.get<f64>("low");
    const xt::xarray<f64>& LOW = xt::adapt(col_low);
    auto low = xt::view(LOW, xt::range(LOW.shape()[0] - N, LOW.shape()[0]), xt::all());
    auto [peaks, valleys] = ta::patterns::peaks_and_valleys(high, low);
    // 输出结果
    std::cout << "波峰位置: ";
    for (auto const & p : peaks) {
        std::cout << p.to_string() << " ";
    }
    std::cout << "\n波谷位置: ";
    for (auto const & v : valleys) {
        std::cout << v.to_string() << " ";
    }
    // 查找最低的波谷
    if (!valleys.empty()) {
        auto min_it = std::min_element(valleys.begin(), valleys.end(),
                                       [](const ta::point& a, const ta::point& b) {
                                           return a.y < b.y;
                                       });

        std::cout << "\n最低波谷（std::min_element）: " << min_it->to_string() << std::endl;
    }
    // 查找最低波谷
    if (valleys.empty()) {
        std::cout << "没有检测到波谷" << std::endl;
        return;
    }

    const ta::point* min_valley = &valleys[0];
    for (const auto& pt : valleys) {
        if (pt.y < min_valley->y) {
            min_valley = &pt;
        }
    }
    std::cout << "最低波谷: " << min_valley->to_string() << std::endl;

    const ta::point* max_peak = &peaks[0];
    for (const auto& pt : peaks) {
        if (pt.y > max_peak->y) {
            max_peak = &pt;
        }
    }
    std::cout << "最高波谷: " << max_peak->to_string() << std::endl;

    // 获取最低波谷和最高波峰之间的波峰
    std::vector<ta::point> future_peaks;
    for (const auto& peak : peaks) {
        if (peak.x > min_valley->x && peak.x < max_peak->x) {
            future_peaks.push_back(peak);
            std::cout << peak.to_string() << std::endl;
        }
    }

    if (future_peaks.size() < 2) {
        std::cout << "用于回归的波峰点不足" << std::endl;
        return;
    }

    // 计算线性回归
    auto model = linear_regression(future_peaks);

    std::cout << "线性回归方程: " << model.to_string() << std::endl;

    // 可选：输出每个点和预测值对比
    std::cout << "波峰点与预测值：" << std::endl;
    for (const auto& pt : future_peaks) {
        double pred = model.predict(pt.x);
        std::cout << pt.to_string() << " -> 预测 y=" << pred << std::endl;
    }
}