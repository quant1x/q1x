#pragma once
#ifndef QUANT1X_DATASETS_KLINE_H
#define QUANT1X_DATASETS_KLINE_H 1

#include <q1x/datasets/xdxr.h>

namespace datasets {

    // 日K线 结构体
    struct KLine {
        std::string Date;             // 日期
        double      Open;             // 开盘价
        double      Close;            // 收盘价
        double      High;             // 最高价
        double      Low;              // 最低价
        double      Volume;           // 成交量(股)
        double      Amount;           // 成交金额(元)
        int         Up;               // 上涨家数 / 外盘
        int         Down;             // 下跌家数 / 内盘
        std::string Datetime;         // 时间
        int         AdjustmentCount;  // 新增：除权除息次数

        void adjust(double m, double a, int number);

        static std::vector<std::string> headers() {
            return {"Date",
                    "Open",
                    "Close",
                    "High",
                    "Low",
                    "Volume",
                    "Amount",
                    "Up",
                    "Down",
                    "Datetime",
                    "AdjustmentCount"};
        }

        friend std::ostream &operator<<(std::ostream &os, const KLine &line) {
            os << "Date: " << line.Date << " Open: " << line.Open << " Close: " << line.Close << " High: " << line.High
               << " Low: " << line.Low << " Volume: " << line.Volume << " Amount: " << line.Amount << " Up: " << line.Up
               << " Down: " << line.Down << " Datetime: " << line.Datetime
               << " AdjustmentCount: " << line.AdjustmentCount;
            return os;
        }
    };

    std::vector<KLine> read_kline_from_csv(const std::string &filename);
    std::vector<KLine> load_kline(const std::string &code);

    class DataKLine : public cache::DataAdapter {
    public:
        cache::Kind Kind() const override { return BaseKLine; }

        std::string Owner() override { return cache::DefaultDataProvider; }

        std::string Key() const override { return "day"; }

        std::string Name() const override { return "日K线"; }

        std::string Usage() const override { return "日K线"; }

        void Print(const std::string &code, const std::vector<exchange::timestamp> &dates) override;

        void Update(const std::string &code, const exchange::timestamp &date) override;
    };

}  // namespace datasets

#endif  // QUANT1X_DATASETS_KLINE_H
