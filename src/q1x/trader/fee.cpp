#include <q1x/trader/fee.h>
#include <q1x/std/numerics.h>
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <q1x/runtime/config.h>

namespace trader {

    // 计算价格笼子
    double calculate_price_cage(Direction direction, double price) {
        double priceLimit = 0.000;
        double priceCage, minimumPriceFluctuation;
        auto const &traderParameter = config::TraderConfig();
        if (direction == Direction::BUY) {
            priceCage = price * (1 + traderParameter->PriceCageRatio);
            minimumPriceFluctuation = price + traderParameter->MinimumPriceFluctuationUnit;
            priceLimit = std::max(priceCage, minimumPriceFluctuation);
        } else {
            priceCage = price * (1 - traderParameter->PriceCageRatio);
            minimumPriceFluctuation = price - traderParameter->MinimumPriceFluctuationUnit;
            priceLimit = std::min(priceCage, minimumPriceFluctuation);
        }
        return priceLimit;
    }

    // 计算合适的买入价格
    double calculate_price_limit_for_buy(double last_price, double price_cage_ratio, double minimum_price_fluctuation_unit) {
        // 价格笼子, +2%和+0.10哪个大
        double priceLimit = std::max(last_price * (1 + price_cage_ratio), last_price + minimum_price_fluctuation_unit);
        // 当前价格+0.05
        priceLimit = last_price + 0.05;
        // 最后修订价格
        priceLimit = numerics::decimal(priceLimit);
        return priceLimit;
    }

    // 计算合适的卖出价格
    double calculate_price_limit_for_sell(double last_price, double price_cage_ratio, double minimum_price_fluctuation_unit) {
        // 价格笼子, -2%和-0.10哪个小
        double priceLimit = std::min(last_price * (1 - price_cage_ratio), last_price - minimum_price_fluctuation_unit);
        // 当前价格-0.01
        priceLimit = last_price - 0.01;
        // 最后修订价格
        priceLimit = numerics::decimal(priceLimit);
        return priceLimit;
    }

    // 计算买入总费用
    struct TransactionFeeResult {
        double TotalFee = 0;
        double StampDutyFee = 0;
        double TransferFee = 0;
        double CommissionFee = 0;
        double MarketValue = 0;
    };

    TransactionFeeResult calculate_transaction_fee(Direction direction, double price, int volume, bool align) {
        if (volume < 1) {
            return {InvalidFee, 0, 0, 0, 0};
        }

        double vol = static_cast<double>(volume);
        double amount = vol * price;
        auto const &traderParameter = config::TraderConfig();
        // 1. 印花税
        double _stamp_duty_fee = amount;
        if (direction == Direction::BUY) {
            _stamp_duty_fee *= traderParameter->StampDutyRateForBuy;
        } else if (direction == Direction::SELL) {
            _stamp_duty_fee *= traderParameter->StampDutyRateForSell;
        } else {
            return {InvalidFee, 0, 0, 0, 0};
        }
        if (align) {
            _stamp_duty_fee = numerics::decimal(_stamp_duty_fee);
        }

        // 2. 过户费
        double _transfer_fee = vol * traderParameter->TransferRate;
        if (align) {
            _transfer_fee = numerics::decimal(_transfer_fee);
        }

        // 3. 券商佣金
        double _commission_fee = amount * traderParameter->CommissionRate;
        if (align) {
            _commission_fee = numerics::decimal(_commission_fee);
        }
        if (align && _commission_fee < traderParameter->CommissionMin) {
            _commission_fee = traderParameter->CommissionMin;
        }

        // 4. 股票市值
        double _marketValue = amount;
        if (align) {
            _marketValue = numerics::decimal(_marketValue);
        }

        // 5. 计算费用
        double _fee = _stamp_duty_fee + _transfer_fee + _commission_fee;

        // 6. 计算总费用
        double _total_fee = _fee;
        if (direction == Direction::BUY) {
            // 买入操作, 加上股票市值
            _total_fee += _marketValue;
        } else {
            // 卖出操作, 股票市值减去费用
            _marketValue -= _fee;
        }

        return {_total_fee, _stamp_duty_fee, _transfer_fee, _commission_fee, _marketValue};
    }

    std::string TradeFee::toString() const {
        std::ostringstream oss;
        oss << "trader[" << (Direction == Direction::BUY ? "BUY" : "SELL") << "]: "
            << "code=" << SecurityCode << ", "
            << "综合费用=" << std::fixed << std::setprecision(2) << TotalFee << ", "
            << "委托价格=" << std::fixed << std::setprecision(2) << Price << ", "
            << "数量=" << Volume << ", "
            << "其中印花税=" << std::fixed << std::setprecision(2) << StampDutyFee << ", "
            << "过户费=" << std::fixed << std::setprecision(2) << TransferFee << ", "
            << "佣金=" << std::fixed << std::setprecision(2) << CommissionFee << ", "
            << "股票=" << std::fixed << std::setprecision(2) << MarketValue;
        return oss.str();
    }

    int TradeFee::CalculateNumToBuy(double fund, double price) {
        Direction = Direction::BUY;
        Price = price;

        // 1. 计算每股费用
        auto [_fee, _, __, ___, ____] = calculate_transaction_fee(Direction, Price, UnknownVolume, false);
        if (_fee == InvalidFee) {
            return InvalidVolume;
        }

        // 2. 计算股数
        double _vol = fund / _fee;

        // 3. 换算成手数
        _vol = floor(_vol / 100);

        // 4. 转成整数
        Volume = static_cast<int>(_vol) * 100;

        // 5. 重新计算
        auto [total, stamp, transfer, commission, market] = calculate_transaction_fee(Direction, Price, Volume, true);
        TotalFee = total;
        StampDutyFee = stamp;
        TransferFee = transfer;
        CommissionFee = commission;
        MarketValue = market;

        if (TotalFee == InvalidFee) {
            return InvalidVolume;
        } else if (_fee > fund) {
            // 如果费用超了, 则减去1手(100股)
            Volume -= 100;
            // 重新计算交易费用
            auto [t, s, tr, c, m] = calculate_transaction_fee(Direction, Price, Volume, true);
            TotalFee = t;
            StampDutyFee = s;
            TransferFee = tr;
            CommissionFee = c;
            MarketValue = m;
        }
        return Volume;
    }

    double TradeFee::CalculateFundFromSell(double price, int volume) {
        Direction = Direction::SELL;
        Price = price;
        Volume = volume;

        auto [total, stamp, transfer, commission, market] = calculate_transaction_fee(Direction, Price, Volume, true);
        TotalFee = total;
        StampDutyFee = stamp;
        TransferFee = transfer;
        CommissionFee = commission;
        MarketValue = market;

        if (TotalFee == InvalidFee) {
            return InvalidFee;
        }
        return MarketValue;
    }

    // 评估买入总费用
    TradeFee EvaluateFeeForBuy(const std::string& securityCode, double fund, double price) {
        TradeFee f{};
        f.SecurityCode = securityCode;
        f.Price = price;
        f.Volume = UnknownVolume;
        f.Direction = Direction::BUY;

        f.Volume = f.CalculateNumToBuy(fund, price);
        return f;
    }

    // 评估卖出费用
    TradeFee EvaluateFeeForSell(const std::string& securityCode, double price, int volume) {
        TradeFee f{};
        f.SecurityCode = securityCode;
        f.Price = price;
        f.Volume = volume;
        f.Direction = Direction::SELL;

        auto [total, stamp, transfer, commission, market] = calculate_transaction_fee(f.Direction, f.Price, f.Volume, true);
        f.TotalFee = total;
        f.StampDutyFee = stamp;
        f.TransferFee = transfer;
        f.CommissionFee = commission;
        f.MarketValue = market;

        return f;
    }

    // 评估卖出价格
    TradeFee EvaluatePriceForSell(const std::string& securityCode, double price, int volume, double fixedYield) {
        TradeFee f{};
        f.SecurityCode = securityCode;
        f.Price = price;
        f.Volume = volume;
        f.Direction = Direction::SELL;

        auto [total, stamp, transfer, commission, market] = calculate_transaction_fee(f.Direction, f.Price, f.Volume, true);
        f.TotalFee = total;
        f.StampDutyFee = stamp;
        f.TransferFee = transfer;
        f.CommissionFee = commission;
        f.MarketValue = market;

        if (fixedYield > 0) {
            double fee = (f.TotalFee - f.TransferFee) * (1 + fixedYield) + f.TransferFee;
            double amount = static_cast<double>(volume) * price;
            double marketValue = amount * (1 + fixedYield);
            double totalFee = fee + marketValue;
            double fixedPrice = totalFee / static_cast<double>(volume);
            f.Price = numerics::decimal(fixedPrice);

            auto [t, s, tr, c, m] = calculate_transaction_fee(f.Direction, f.Price, f.Volume, true);
            f.TotalFee = t;
            f.StampDutyFee = s;
            f.TransferFee = tr;
            f.CommissionFee = c;
            f.MarketValue = m;
        }

        return f;
    }

} // namespace trader