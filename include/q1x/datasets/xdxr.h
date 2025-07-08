#pragma once
#ifndef QUANT1X_DATASETS_XDXR_H
#define QUANT1X_DATASETS_XDXR_H 1

#include "q1x/level1/client.h"
#include "q1x/engine/action.h"

namespace datasets {
    constexpr cache::Kind baseKind = 0;
    constexpr cache::Kind BaseXdxr                = cache::PluginMaskBaseData | (baseKind +  1); // 基础数据-除权除息
    constexpr cache::Kind RawKLine                = cache::PluginMaskBaseData | (baseKind +  2); // 基础数据-未复权K线
    constexpr cache::Kind BaseKLine               = cache::PluginMaskBaseData | (baseKind +  3); // 基础数据-前复权K线
    constexpr cache::Kind BaseTransaction         = cache::PluginMaskBaseData | (baseKind +  4); // 基础数据-历史成交
    constexpr cache::Kind BaseMinutes             = cache::PluginMaskBaseData | (baseKind +  5); // 基础数据-分时数据
    constexpr cache::Kind BaseQuarterlyReports    = cache::PluginMaskBaseData | (baseKind +  6); // 基础数据-季报
    constexpr cache::Kind BaseSafetyScore         = cache::PluginMaskBaseData | (baseKind +  7); // 基础数据-安全分
    constexpr cache::Kind BaseWideKLine           = cache::PluginMaskBaseData | (baseKind +  8); // 基础数据-宽表
    constexpr cache::Kind BasePerformanceForecast = cache::PluginMaskBaseData | (baseKind +  9); // 基础数据-业绩预告
    constexpr cache::Kind BaseChipDistribution    = cache::PluginMaskBaseData | (baseKind + 10); // 基础数据-筹码分布

    // 市场开埠日期
    inline exchange::timestamp market_first_date = exchange::timestamp::parse(exchange::market_cn_first_listtime).pre_market_time();
}

namespace datasets {

    // 加载除权除息记录
    std::vector<level1::XdxrInfo> load_xdxr(const std::string& code);

    // 除权除息
    class DataXdxr : public cache::DataAdapter {
    public:
        cache::Kind Kind() const override;
        std::string Owner() override ;
        std::string Key() const override ;
        std::string Name() const override ;
        std::string Usage() const override;
        void Print(const std::string &code, const std::vector<exchange::timestamp> &dates) override;
        void Update(const std::string &code, const exchange::timestamp &date) override;
    private:
        //void save_xdxr(const std::string &code, const std::string &date, const std::vector<level1::XdxrInfo>& values);
    };
}

#endif //QUANT1X_DATASETS_XDXR_H
