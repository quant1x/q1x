#include <q1x/test/test.h>
#include <q1x/runtime/config.h>
#include <q1x/proto/data.h>

TEST_CASE("load-trans", "[chips]") {

}

TEST_CASE("load-chips", "[chips]") {
    std::string securityCode = "sh510050";
    std::string factor_date = "2025-07-17";
    auto ofn = config::get_chip_distribution_filename(securityCode, factor_date);
    std::cout << ofn << std::endl;
    datasets::Chips chips{};
    std::ifstream is(ofn, std::ios::binary);
    bool ok = chips.ParseFromIstream(&is);
    std::cout << ok << std::endl;
    auto date = chips.date();
    std::cout << date << std::endl;
    std::cout << chips.dist().size() << std::endl;
    f64 vol = 0;
    for (auto &pl: chips.dist()) {
        std::cout << pl.DebugString() << std::endl;
        vol += (pl.sell() + pl.buy());
    }
    std::cout << vol << std::endl;
}

#include <q1x/factors/base.h>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <q1x/factors/f10.h>

// TechSignal 技术信号位掩码 (支持组合信号)
typedef uint64_t TechSignal;

const TechSignal ShortTermRebound = 1 << 0;  // 短线止跌     (0000 0001)
const TechSignal ShortTermBreakout = 1 << 1; // 短线突破     (0000 0010)
const TechSignal VolumeBreakout = 1 << 2;    // 放量突破     (0000 0100)
const TechSignal VolumeBreakdown = 1 << 3;   // 放量破位     (0000 1000)
const TechSignal StrongSupport = 1 << 4;      // 强支撑       (0001 0000)
// 可继续扩展其他信号...

// 组合信号示例
const TechSignal ReboundWithSupport = ShortTermRebound | StrongSupport;   // 0001 0001
const TechSignal BreakoutSignals = ShortTermBreakout | VolumeBreakout;    // 0000 0110

// Has 判断是否包含某信号
bool TechSignalHas(TechSignal ts, TechSignal signal) {
    return (ts & signal) != 0;
}

// Add 添加信号
void TechSignalAdd(TechSignal *ts, TechSignal signal) {
    *ts |= signal;
}

// Remove 移除信号
void TechSignalRemove(TechSignal *ts, TechSignal signal) {
    *ts &= ~signal;
}

// 转换为可读字符串
std::string TechSignalToString(TechSignal ts) {
    std::stringstream builder;
    if (TechSignalHas(ts, ShortTermRebound)) {
        builder << "短线止跌|";
    }
    if (TechSignalHas(ts, ShortTermBreakout)) {
        builder << "短线突破|";
    }
    if (TechSignalHas(ts, VolumeBreakout)) {
        builder << "放量突破|";
    }
    if (TechSignalHas(ts, VolumeBreakdown)) {
        builder << "放量破位|";
    }
    if (TechSignalHas(ts, StrongSupport)) {
        builder << "强支撑|";
    }
    std::string str = builder.str();
    if (!str.empty()) {
        str = str.substr(0, str.size() - 1); // 去除末尾的 |
    }
    return str;
}


// DailyData 日线数据结构
struct DailyData {
    datasets::KLine kline;
    double TurnoverRate;
    double Avg;
};

// PeakSignal 峰值信号
struct PeakSignal {
    double Closest;            // 最近峰值价格
    double Extremum;           // 极值价格, 筹码最密集的价位
    double CurrentToPeakVol;   // 当前价格到峰值的累计筹码
    double CurrentToPeakRatio; // 当前价格到峰值的累计筹码占比 (0~1)
    double PeakVolume;         // 峰值对应筹码量
    double PeakRatio;          // 峰值价位本身的筹码占比 (0~1)

    // 以下为扩展分析字段
    double LeftVolume;    // 峰值左侧筹码总量
    double RightVolume;   // 峰值右侧筹码总量
    int AvgHoldDays;      // 峰值区平均持仓天数
    double Concentration; // 筹码集中度 (标准差)
};

// Config 计算配置参数
struct Config {
    double PriceStep;    // 价格最小变动单位
    double DecayFactor;  // 衰减系数
    int ModelType;       // 计算模型 (1:三角形 2:均匀)
    int SearchWindow;    // 峰值搜索窗口大小
};

const Config defaultConfig = {
    0.01,    // PriceStep
    0.9995,  // DecayFactor
    1,       // ModelType
    5        // SearchWindow
};

const double minValidVolume = 1e-8;  // 最小有效成交量阈值
const int maxIterations = 10000;    // 最大迭代次数防止死循环
const int yearPeriod = 1;

// ChipDistribution 筹码分布计算器
class ChipDistribution {
public:
    std::map<double, double> chip;  // 当前筹码分布
    std::vector<DailyData> data;    // 日线数据
    Config config;                  // 计算配置
    double Capital;                 // 流通股本
    int Digits;                     // 小数点位数
    double HoldingPrice;            // 目标价格
    double LastClose;               // 最新收盘
    double High;                    // 最高价
    double Low;                     // 最低价

    // NewChipDistribution 创建新的筹码分布计算实例
    ChipDistribution(Config cfg) {
        if (cfg.PriceStep <= 0) {
            cfg.PriceStep = 0.01;
        }
        if (cfg.SearchWindow <= 0) {
            cfg.SearchWindow = 3;
        }

        config = cfg;
        Digits = 2; // 默认2位小数点
    }

    // FiveYearsAgoJanFirst 获取五年前的1月1日零点
    static std::tm FiveYearsAgoJanFirst() {
        std::time_t now = std::time(nullptr);
        std::tm *tm_now = std::localtime(&now);
        std::tm tm_result = *tm_now;
        tm_result.tm_year -= yearPeriod;
        tm_result.tm_mon = 0;  // January
        tm_result.tm_mday = 1;
        tm_result.tm_hour = 0;
        tm_result.tm_min = 0;
        tm_result.tm_sec = 0;
        return tm_result;
    }

    // 加载数据
    bool LoadCSV(const std::string &code, const std::string &date) {
        // 假设这些函数在C++中有对应的实现
        // std::vector<KLine> klines = base::CheckoutKLines(code, date);
        // factors::L5F10* f10 = factors::GetL5F10(code);
        // if (f10 == nullptr) {
        //     return false;
        // }
        // Capital = f10->Capital;
        // Digits = f10->DecimalPoint;
        auto klines = factors::checkout_klines(code, date);
        if (klines.empty()) {
            return false;
        }
        auto f10 = factors::get_f10(code, date);
        if (!f10.has_value()) {
            return false;
        }

        Capital = f10->Capital;
        Digits = f10->DecimalPoint;

        double high = -std::numeric_limits<double>::infinity();
        double low = std::numeric_limits<double>::infinity();
        double lastClose = 0.00;

        // 计算数据的有效起始日期
        std::tm activeDeadlineTm = FiveYearsAgoJanFirst();
        char activeDeadline[11];
        std::strftime(activeDeadline, sizeof(activeDeadline), "%Y-%m-%d", &activeDeadlineTm);

        // 预分配切片容量
        data.clear();
        data.reserve(klines.size());
        for (const auto &record: klines) {
            if (record.Date < activeDeadline) {
                continue;
            }
            DailyData d;
            d.kline = record;
            d.TurnoverRate = 100 * (record.Volume / Capital);
            d.Avg = record.Amount / record.Volume;
            d.kline.Open = numerics::decimal(d.kline.Open, Digits);
            d.kline.Close = numerics::decimal(d.kline.Close, Digits);
            d.kline.High = numerics::decimal(d.kline.High, Digits);
            d.kline.Low = numerics::decimal(d.kline.Low, Digits);
            d.Avg = numerics::decimal(d.Avg, Digits);
            if (d.kline.High > high) {
                high = d.kline.High;
            }
            if (d.kline.Low < low) {
                low = d.kline.Low;
            }
            data.push_back(d);
            lastClose = d.kline.Close;
        }
        data.shrink_to_fit();
        LastClose = lastClose;
        if (!std::isinf(high)) {
            High = high;
        }
        if (!std::isinf(low)) {
            Low = low;
        }
        return true;
    }

    double RealVolume(double proportion) {
        return Capital * proportion;
    }

    // Calculate 执行筹码分布计算
    bool Calculate() {
        if (data.empty()) {
            return false;
        }

        for (const auto &day: data) {
            switch (config.ModelType) {
                case 1:
                    if (!calculateTriangular(day)) {
                        return false;
                    }
                    break;
                case 2:
                    if (!calculateUniform(day)) {
                        return false;
                    }
                    break;
                default:
                    return false;
            }
        }
        return true;
    }

private:
    // 处理单一价格日的特殊逻辑
    void handleSinglePriceDay(const DailyData &day) {
        double singlePrice = numerics::decimal(day.kline.Close, Digits);

        // 构造全量筹码分布
        std::map<double, double> tmpChip;
        tmpChip[singlePrice] = day.kline.Volume;

        // 应用衰减合并（需特殊处理衰减率）
        DailyData adjustedDay = day;
        // adjustedDay.TurnoverRate = 100; // 强制全量换手
        applyDecayAndMerge(adjustedDay, tmpChip);
    }

    // 三角形分布计算
    bool calculateTriangular(const DailyData &day) {
        // 情况1：处理最高价等于最低价的情况（一字涨停/跌停）
        if (day.kline.High == day.kline.Low) {
            // 直接全部分配到唯一价格点
            handleSinglePriceDay(day);
            return true;
        }

        // 情况2：常规价格区间校验
        if (day.kline.High < day.kline.Low) {
            return false;
        }

        // 生成价格网格（包含容差处理）
        std::vector<double> priceGrid = generatePriceGrid(day.kline.Low, day.kline.High, config.PriceStep, Digits);
        std::map<double, double> tmpChip;

        // 计算归一化系数（处理可能的零除问题）
        double priceRange = day.kline.High - day.kline.Low;
        double h = 2.0 / priceRange; // 保证概率密度积分为1

        for (double price: priceGrid) {
            double x1 = price;
            double x2 = price + config.PriceStep;
            double area;

            // 分情况处理三角形分布
            if (price < day.Avg) {
                // 左三角形处理（包含Avg=Low的边界情况）
                double denominator = day.Avg - day.kline.Low;
                if (denominator <= 1e-8) { // 处理浮点精度误差
                    // 当Avg=Low时退化为矩形分布
                    area = config.PriceStep * h;
                } else {
                    double y1 = h / denominator * (x1 - day.kline.Low);
                    double y2 = h / denominator * (x2 - day.kline.Low);
                    area = config.PriceStep * (y1 + y2) / 2;
                }
            } else {
                // 右三角形处理（包含Avg=High的边界情况）
                double denominator = day.kline.High - day.Avg;
                if (denominator <= 1e-8) {
                    // 当Avg=High时退化为矩形分布
                    area = config.PriceStep * h;
                } else {
                    double y1 = h / denominator * (day.kline.High - x1);
                    double y2 = h / denominator * (day.kline.High - x2);
                    area = config.PriceStep * (y1 + y2) / 2;
                }
            }

            tmpChip[price] = area * day.kline.Volume; // 面积映射到实际成交量
        }

        // 应用衰减和合并
        applyDecayAndMerge(day, tmpChip);
        return true;
    }

    // 均匀分布计算
    bool calculateUniform(const DailyData &day) {
        std::vector<double> priceGrid = generatePriceGrid(day.kline.Low, day.kline.High, config.PriceStep, Digits);
        double eachVol = day.kline.Volume / priceGrid.size();
        std::map<double, double> tmpChip;

        for (double price: priceGrid) {
            tmpChip[price] = eachVol;
        }

        applyDecayAndMerge(day, tmpChip);
        return true;
    }

    // 应用衰减并合并筹码
    void applyDecayAndMerge(const DailyData &day, const std::map<double, double> &newChip) {
        double decayRate = day.TurnoverRate / 100 * config.DecayFactor;
        decayRate = std::min(decayRate, 1.0);

        // 衰减现有筹码
        for (auto &[price, vol]: chip) {
            vol *= (1 - decayRate);
        }

        // 合并新筹码
        for (const auto &[price, vol]: newChip) {
            chip[price] += vol * decayRate;
        }

        // 清理接近零的筹码
        for (auto it = chip.begin(); it != chip.end();) {
            if (it->second < minValidVolume) {
                it = chip.erase(it);
            } else {
                ++it;
            }
        }
    }

    // 生成价格区间网格
    std::vector<double> generatePriceGrid(double low, double high, double step, int digits) {
        if (high < low) {
            std::swap(low, high);
        }
        double scale = std::pow(10, digits);
        int lowInt = static_cast<int>(std::round(low * scale));
        int highInt = static_cast<int>(std::round(high * scale));
        int stepInt = static_cast<int>(std::round(step * scale));

        if (stepInt <= 0) {
            stepInt = 1;
        }

        int length = (highInt - lowInt) / stepInt + 1;
        if (length > maxIterations) {
            stepInt = static_cast<int>(std::ceil(static_cast<double>(highInt - lowInt) / maxIterations));
        }
        std::vector<double> grid;
        grid.reserve(length);
        for (int priceInt = lowInt; priceInt <= highInt; priceInt += stepInt) {
            double price = static_cast<double>(priceInt) / scale;
            price = numerics::decimal(price, digits); // 确保四舍五入
            grid.push_back(price);
        }
        return grid;
    }

    // 辅助函数：查找局部峰值
    std::vector<double> findLocalPeaks(const std::vector<double> &prices, const std::map<double, double> &data) {
        std::vector<double> peaks;
        int n = prices.size();
        if (n < 3) {
            return peaks;
        }

        int windowSize = config.SearchWindow;
        for (int i = 0; i < n; ++i) {
            int left = std::max(0, i - windowSize / 2);
            int right = std::min(n - 1, i + windowSize / 2);

            bool isPeak = true;
            double currentVol = data.at(prices[i]);
            for (int j = left; j <= right; ++j) {
                if (j == i) {
                    continue;
                }
                if (currentVol <= data.at(prices[j])) {
                    isPeak = false;
                    break;
                }
            }

            if (isPeak) {
                peaks.push_back(prices[i]);
            }
        }
        return peaks;
    }

    std::vector<double> sortMapKeys(const std::map<double, double> &m) {
        std::vector<double> keys;
        keys.reserve(m.size());
        for (const auto &[k, v]: m) {
            keys.push_back(k);
        }
        std::sort(keys.begin(), keys.end());
        return keys;
    }

    std::pair<double, double>
    findMaxPeak(double current, const std::vector<double> &prices, const std::map<double, double> &data) {
        double maxVol = 0.0;
        double peak = 0.0;
        for (double p: prices) {
            if (data.at(p) > maxVol) {
                maxVol = data.at(p);
                peak = p;
            }
        }
        double high = peak;
        double low = current;
        if (high < low) {
            std::swap(high, low);
        }
        double vol = 0.0;
        for (double p: prices) {
            if (p >= low && p <= high) {
                vol += data.at(p);
            }
        }
        return {peak, vol};
    }

public:
    // FindMainPeaks 直接使用当前chip
    std::pair<PeakSignal, PeakSignal> FindMainPeaks(double targetPrice) {
        PeakSignal upper{}, lower{};
        if (chip.empty()) {
            return {upper, lower};
        }

        double total = calculateTotalVolume(chip);
        if (total <= 0) {
            return {upper, lower};
        }
        HoldingPrice = targetPrice;
        std::vector<double> sorted = sortMapKeys(chip);
        std::vector<double> peaks = findLocalPeaks(sorted, chip);

        // 分离上下峰值
        std::vector<double> lowerPeaks, upperPeaks;
        for (double p: peaks) {
            if (p < HoldingPrice) {
                lowerPeaks.push_back(p);
            } else if (p > HoldingPrice) {
                upperPeaks.push_back(p);
            }
        }

        // 计算特征点
        lower = calculateChipFeature(lowerPeaks, chip, total, false);
        upper = calculateChipFeature(upperPeaks, chip, total, true);
        return {upper, lower};
    }

private:
    // 计算单个特征点信息
    PeakSignal
    calculateChipFeature(const std::vector<double> &prices, const std::map<double, double> &data, double total,
                         bool isUpper) {
        PeakSignal feature{};

        if (prices.empty()) {
            return feature;
        }

        // 极值计算
        auto [maxPeakPrice, vol] = findMaxPeak(HoldingPrice, prices, data);
        if (maxPeakPrice > 0) {
            feature.Extremum = maxPeakPrice;
            feature.PeakVolume = data.at(maxPeakPrice);
            feature.PeakRatio = feature.PeakVolume / total;
            feature.CurrentToPeakVol = vol;
            feature.CurrentToPeakRatio = feature.CurrentToPeakVol / total;
        }

        // 最近峰值计算
        double closestPrice = 0.0;
        if (isUpper) {
            closestPrice = *std::min_element(prices.begin(), prices.end());
        } else {
            closestPrice = *std::max_element(prices.begin(), prices.end());
        }

        if (closestPrice > 0) {
            feature.Closest = closestPrice;
            // 如果极值未找到，使用最近峰值的量能
            if (feature.PeakVolume == 0) {
                feature.PeakVolume = data.at(closestPrice);
                feature.PeakRatio = feature.PeakVolume / total;
            }
        }

        return feature;
    }

    // 计算总筹码量
    double calculateTotalVolume(const std::map<double, double> &data) {
        double total = 0.0;
        for (const auto &[price, vol]: data) {
            total += vol;
        }
        return total;
    }
};

TEST_CASE("v2", "[chips]") {
    std::string code = "000158";
    std::string date = "2025-07-16";
    std::string security_code = exchange::CorrectSecurityCode(code);
    ChipDistribution cd(defaultConfig);
    bool result = cd.LoadCSV(security_code, date);
    if(!result) {
        return;
    }
    result = cd.Calculate();
    if(!result) {
        return;
    }
    f64 targetPrice = cd.LastClose;
    auto [upper, lower] = cd.FindMainPeaks(targetPrice);
    std::cout << "当前日期: " << date << ", 证券代码: " << security_code << std::endl;
    std::cout << "当前价格: " << targetPrice << " 附近的主要筹码峰:" << std::endl;
    std::printf("压力(上): 最接近=%.2f, 最大=%.2f, 成交量=%.2f股, 占比=%.2f%%\n", upper.Closest, upper.Extremum, cd.RealVolume(upper.CurrentToPeakRatio), 100*upper.CurrentToPeakRatio);
    std::printf("支撑(下): 最接近=%.2f, 最大=%.2f, 成交量=%.2f股, 占比=%.2f%%\n", lower.Closest, lower.Extremum, cd.RealVolume(lower.CurrentToPeakRatio), 100*lower.CurrentToPeakRatio);
}