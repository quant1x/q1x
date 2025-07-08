#pragma once
#ifndef QUANT1X_CONFIG_DETAIL_PRICE_CAGE_H
#define QUANT1X_CONFIG_DETAIL_PRICE_CAGE_H 1

#include "q1x/std/api.h"

namespace config {

    // 价格笼子
    //
    //	价格笼子是买卖股票申报价格限制的一种制度
    //	对于主板, 本次新增2%有效申报价格范围要求, 同时增加10个申报价格最小变动单位的安排
    //	A股最小交易变动单位是0.01元，10个也就是0.1元
    //	买入价取两者高值，卖出价取两者低值.
    constexpr const double ValidDeclarationPriceRange  = 0.02;  // 价格限制比例
    constexpr const double MinimumPriceFluctuationUnit = 0.10;  // 价格浮动最大值

    // 卖出滑点比例, 默认0.01
    constexpr const double FixedSlippageForSell = 0.01;

    struct price_cage {
        double price_cage_ratio;                // 价格笼子比例
        double minimum_price_fluctuation_unit;  // 最小价格变动单位

        // 默认构造函数
        price_cage(double ratio = ValidDeclarationPriceRange, double unit = MinimumPriceFluctuationUnit)
            : price_cage_ratio(ratio), minimum_price_fluctuation_unit(unit) {}
    };
}  // namespace config

#endif  // QUANT1X_CONFIG_DETAIL_PRICE_CAGE_H