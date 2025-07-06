#pragma once
#ifndef QUANT1X_DATASETS_KLINE_RAW_H
#define QUANT1X_DATASETS_KLINE_RAW_H 1

#include <q1x/datasets/xdxr.h>

namespace datasets {

    namespace detail {
        // 日线最小容错回溯(偏移)天数
        constexpr const size_t MAX_KLINE_LOOKBACK_DAYS = 1;
        // 拉取数据
        std::vector<level1::SecurityBar> fetch_kline(const std::string &code, u16 start, u16 count);
    }  // namespace detail

    // 日K线 结构体
    struct KLineRaw {
        std::string Date;      // 日期
        double      Open;      // 开盘价
        double      Close;     // 收盘价
        double      High;      // 最高价
        double      Low;       // 最低价
        double      Volume;    // 成交量(股)
        double      Amount;    // 成交金额(元)
        int         Up;        // 上涨家数 / 外盘
        int         Down;      // 下跌家数 / 内盘
        std::string Datetime;  // 时间

        // 复权
        void adjust(double m, double a, int number);

        static std::vector<std::string> headers() {
            return {"Date", "Open", "Close", "High", "Low", "Volume", "Amount", "Up", "Down", "Datetime"};
        }

        friend std::ostream &operator<<(std::ostream &os, const KLineRaw &line) {
            os << "Date: " << line.Date << " Open: " << line.Open << " Close: " << line.Close << " High: " << line.High
               << " Low: " << line.Low << " Volume: " << line.Volume << " Amount: " << line.Amount << " Up: " << line.Up
               << " Down: " << line.Down << " Datetime: " << line.Datetime;
            return os;
        }
    };

    // 加载原始K线
    std::vector<KLineRaw> load_kline_raw(const std::string &code);

    class DataKLineRaw : public cache::DataAdapter {
    public:
        cache::Kind Kind() const override { return RawKLine; }

        std::string Owner() override { return cache::DefaultDataProvider; }

        std::string Key() const override { return "day_raw"; }

        std::string Name() const override { return "日K线RAW"; }

        std::string Usage() const override { return "日K线RAW"; }

        void Print(const std::string &code, const std::vector<exchange::timestamp> &dates) override;

        void Update(const std::string &code, const exchange::timestamp &date) override;
    };

}  // namespace datasets

#endif  // QUANT1X_DATASETS_KLINE_RAW_H
