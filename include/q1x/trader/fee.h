#pragma once
#ifndef QUANT1X_TRADER_FEE_H
#define QUANT1X_TRADER_FEE_H 1

#include <q1x/std/api.h>

// ==============================
// 交易费用
// ==============================

namespace trader {
    constexpr const double  InvalidFee     = 0.0;  // 无效的费用
    constexpr const int64_t InvalidVolume  = 0;    // 无效的股数
    constexpr const int64_t UnknownVolume  = 1;    // 未知的股数
    constexpr const int64_t InvalidOrderId = -1;   // 无效的订单ID
    constexpr const int64_t MinimumNumberOfOrders = 2;  // 最小订单数为2, 预防一个策略资金都打到一个标的上面
    constexpr const double backtestAccountTheoreticalFund = 10000.00;  // 测试可用10000.00元

    // 价格笼子
    //
    //	价格笼子是买卖股票申报价格限制的一种制度
    //	对于主板, 本次新增2%有效申报价格范围要求, 同时增加10个申报价格最小变动单位的安排
    //	A股最小交易变动单位是0.01元，10个也就是0.1元
    //	买入价取两者高值，卖出价取两者低值.
    constexpr const double validDeclarationPriceRange  = 0.02;  // 价格限制比例
    constexpr const double minimumPriceFluctuationUnit = 0.10;  // 价格浮动最大值

    // 交易方向
    enum class Direction {
        UNKNOWN,  // 未知、无效的
        BUY,      // 买入
        SELL,     // 卖出
        JUNK      // 废单
    };

    // 交易费用
    struct TradeFee {
        Direction   Direction = Direction::UNKNOWN;  // 交易方向
        std::string SecurityCode;                    // 证券代码
        double      Price         = 0;               // 价格
        int         Volume        = 0;               // 数量
        double      StampDutyFee  = 0;  // 印花税, 按照成交金额计算, 双向, 默认单向, 费率0.1%
        double      TransferFee   = 0;  // 过户费, 按照股票数量, 双向, 默认是0.06%
        double      CommissionFee = 0;  // 券商佣金, 按照成交金额计算, 双向, 0.025%
        double      MarketValue   = 0;  // 股票市值
        double      TotalFee      = 0;  // 支出总费用

        TradeFee() = default;

        double CalculateFundFromSell(double price, int volume);

        int CalculateNumToBuy(double fund, double price);

        std::string toString() const;
    };

    // 评估买入总费用
    TradeFee EvaluateFeeForBuy(const std::string &securityCode, double fund, double price);
    // 评估卖出费用
    TradeFee EvaluateFeeForSell(const std::string &securityCode, double price, int volume);

}  // namespace trader

#endif  // QUANT1X_TRADER_FEE_H
