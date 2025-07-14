#include <q1x/factors/history.h>
#include <q1x/formula.h>
#include <q1x/datasets/trans.h>
#include <q1x/dataframe/dataframe.h>
#include <boost/pfr.hpp>
#include <q1x/encoding/csv.h>

void History::adjust(double m, double a, int number) {
    (void)m;
    (void)a;
    (void)number;
}

cache::Kind HistoryFeature::Kind() const {
    return factors::FeatureHistory;
}

std::string HistoryFeature::Owner() {
    return cache::DefaultDataProvider;
}

std::string HistoryFeature::Key() const {
    return "history";
}

std::string HistoryFeature::Name() const {
    return "history";
}

std::string HistoryFeature::Usage() const {
    return "历史数据";
}

void HistoryFeature::Print(const std::string &code, const std::vector<exchange::timestamp> &dates) {
    (void)code;
    (void)dates;
}

void HistoryFeature::Update(const std::string &code, const exchange::timestamp &date) {
    (void)code;
    (void)date;
    std::string feature_date = date.only_date();
    exchange::timestamp ts_cache = exchange::next_trading_day(date);
    history.Date = ts_cache.only_date();
    history.Code = code;
    auto klines = factors::klines_forward_adjusted_to_date(code, feature_date);
    if(klines.size() < factors::KLineMin) {
        spdlog::warn("code={},date={}, 日线数据不足", code, feature_date);
        return;
    }
    DataFrame df = DataFrame::from_struct_vector(klines);
    // 直接获取列
    auto const& col_open = df.get<f64>("open");
    const xt::xarray<f64>& OPEN = xt::adapt(col_open);
    auto const& col_close = df.get<f64>("close");
    const xt::xarray<f64>& CLOSE = xt::adapt(col_close);
    auto const& col_high = df.get<f64>("high");
    const xt::xarray<f64>& HIGH = xt::adapt(col_high);
    auto const& col_low = df.get<f64>("low");
    const xt::xarray<f64>& LOW = xt::adapt(col_low);

    auto const& col_vol = df.get<f64>("volume");
    const xt::xarray<f64>& VOL = xt::adapt(col_vol);
    auto const& col_amount = df.get<f64>("amount");
    const xt::xarray<f64>& AMOUNT = xt::adapt(col_amount);

    auto r1Close = formula::ref(CLOSE,1);

    auto ma2 = formula::ma(CLOSE, 2);
    history.MA2 = formula::at(ma2, -1);

    auto ma3 = formula::ma(CLOSE, 3);
    history.MA3 = formula::at(ma3, -1);
    auto mv3 = formula::ma(VOL, 3);
    history.MV3 = formula::at(mv3, -1);

    auto ma4 = formula::ma(CLOSE, 4);
    history.MA4 = formula::at(ma4, -1);

    auto ma5 = formula::ma(CLOSE, 5);
    history.MA5 = formula::at(ma5, -1);
    auto mv5 = formula::ma(VOL, 5);
    history.MV5 = formula::at(mv5, -1);

    auto ma9 = formula::ma(CLOSE, 9);
    history.MA9 = formula::at(ma9, -1);
    auto mv9 = formula::ma(VOL, 9);
    history.MV9 = formula::at(mv9, -1);

    auto ma10 = formula::ma(CLOSE, 10);
    history.MA10 = formula::at(ma10, -1);
    auto mv10 = formula::ma(VOL, 10);
    history.MV10 = formula::at(mv10, -1);

    auto ma19 = formula::ma(CLOSE, 19);
    history.MA19 = formula::at(ma19, -1);
    auto mv19 = formula::ma(VOL, 19);
    history.MV19 = formula::at(mv19, -1);

    auto ma20 = formula::ma(CLOSE, 20);
    history.MA20 = formula::at(ma20, -1);
    auto mv20 = formula::ma(VOL, 20);
    history.MV20 = formula::at(mv20, -1);

    history.LastClose = formula::at(r1Close, -1);
    history.OPEN = formula::at(OPEN, -1);
    history.CLOSE = formula::at(CLOSE, -1);
    history.HIGH = formula::at(HIGH, -1);
    history.LOW = formula::at(LOW, -1);
    history.VOL = formula::at(VOL, -1);
    history.AMOUNT = formula::at(AMOUNT, -1);

    const xt::xarray<f64>& ap = AMOUNT / VOL;
    history.AveragePrice = formula::at(ap, -1);

    // 计算多头排列: 5日线高于10日线, 10日线高于20日线
    auto bullC = ma5>ma10 && ma10>ma20;
    auto bullN = formula::bars_last_count(bullC);
    history.BullN = formula::at(bullN, -1);

    // 最近一次向上的跳空缺口到现在的周期数
    auto grapUpWard = LOW > formula::ref(HIGH, 1);
    auto upward_n = formula::bars_last(grapUpWard);
    history.UpwardN = formula::at(upward_n, -1);

    // 收盘价,最高价和成交量连续走高
    auto isClosingPriceStrong = CLOSE>r1Close;
    auto isVolStrong = VOL>formula::ref(VOL,1);
    auto newHigh = isClosingPriceStrong && HIGH>formula::ref(HIGH,1) && isVolStrong;
    auto newHighN = formula::bars_last_count(newHigh);
    history.NewHighN = formula::at(newHighN, -1);

    // 低点走高次数
    auto noLow = isClosingPriceStrong && isVolStrong;
    auto noLowN = formula::bars_last_count(noLow);
    history.NewNoLowN = formula::at(noLowN, -1);

    // 最低价连续走低
    auto newLow = LOW < formula::ref(LOW, 1);
    auto newLowN = formula::bars_last_count(newLow);
    history.NewLowN = formula::at(newLowN, -1);

    // 成交统计概要数据
    auto list = datasets::CheckoutTransactionData(code, date, true);
    if(list.empty()) {
        spdlog::warn("code={},date={}, 分笔成交数据为空", code, feature_date);
    }
    auto summary = datasets::CountInflow(list, code, ts_cache);
    history.OpenVolume = summary.OpenVolume;

    // 1号策略补充
    auto baseHigh = formula::ref(HIGH, 1);
    //UP_TREND := HIGH > VAR1;    {最高价上行}
    auto UP_TREND = HIGH > baseHigh;
    xt::xarray<bool> b_up = xt::cast<bool>(UP_TREND);
    history.no1_up_trend = formula::at(b_up, -1);
    //DOWN_TREND := HIGH < VAR1;  {最高价下行}
    auto DOWN_TREND = HIGH < baseHigh;
    xt::xarray<bool> b_down = xt::cast<bool>(DOWN_TREND);
    history.no1_down_trend = formula::at(b_down, -1);
    //
    //{记录最高价上行起点（使用 LLV 计算最近低点）}
    //START_CONDITION := UP_TREND AND REF(DOWN_TREND, 1);
    auto START_CONDITION = UP_TREND && formula::ref(DOWN_TREND, 1);
    xt::xarray<bool> b_start = xt::cast<bool>(START_CONDITION);
    history.no1_start_condition = formula::at(b_start, -1);
    //START_PRICE := VALUEWHEN(START_CONDITION, LOW);  {仅在条件成立时记录 LOW}
    auto START_PRICE = formula::value_when(START_CONDITION, LOW);
    history.no1_start_price = formula::at(START_PRICE, -1);
    //
    //{判断是否创新高}
    //NEW_HIGH := HIGH > HHV(HIGH, BARSLAST(START_CONDITION) + 1);
    auto new_high_n = formula::bars_last(START_CONDITION)+1;
    const xt::xarray<double> highest = formula::rolling(HIGH, new_high_n, formula::rolling_ops::hhv);
    auto NEW_HIGH = HIGH > highest;
    //
    //{买入信号：突破下行趋势}
    //BUY_SIGNAL := CROSS(HIGH, REF(HIGH, 1)) AND REF(DOWN_TREND, 1);
    auto b_cross_high = formula::cross(HIGH, formula::ref(HIGH,1));
    history.no1_cross_high = formula::at(b_cross_high, -1);
    auto break_high = xt::cast<bool>(b_cross_high);
    auto down_trend = xt::cast<bool>(formula::ref(DOWN_TREND, 1));
    auto BUY_SIGNAL = break_high && down_trend;
    auto buy_cond = xt::eval(BUY_SIGNAL);  // 先评估为具体数组

    //
    //{卖出信号1：跌破上行起点}
    //SELL_SIGNAL1 := LOW < START_PRICE AND START_PRICE > 0;
    auto SELL_SIGNAL1 = LOW<START_PRICE && START_PRICE>0;
    //
    //{卖出信号2：无新高后下行}
    //SELL_SIGNAL2 := REF(UP_TREND, 1) AND DOWN_TREND AND NOT(NEW_HIGH);
    auto SELL_SIGNAL2 = formula::ref(UP_TREND, 1) && DOWN_TREND && !NEW_HIGH;
    //
    //{综合卖出信号}
    //SELL_SIGNAL := SELL_SIGNAL1 OR SELL_SIGNAL2;
    auto SELL_SIGNAL = SELL_SIGNAL1 || SELL_SIGNAL2;

    history.UpdateTime = api::get_timestamp();
    history.State |= factors::FeatureHistory;
}

void HistoryFeature::init(const exchange::timestamp &timestamp) {
    (void)timestamp;
}

std::unique_ptr<cache::FeatureAdapter> HistoryFeature::clone() const {
    return std::make_unique<HistoryFeature>(*this);
}

std::vector<std::string> HistoryFeature::headers() const {
    std::vector<std::string> header;
    boost::pfr::for_each_field(History{}, [&](auto& field, auto idx) {
        (void)field;
        constexpr auto field_name = boost::pfr::get_name<idx, History>();
        header.emplace_back(field_name);
    });
    return header;
}

std::vector<std::string> HistoryFeature::values() const {
    std::vector<std::string> row;
    boost::pfr::for_each_field(history, [&](auto& field, auto /*idx*/) {
        row.emplace_back(encoding::csv::detail::to_csv_string(field));
    });
    return row;
}

std::ostream &operator<<(std::ostream &os, const History &history) {
    os << "Date: " << history.Date << " Code: " << history.Code << " MA2: " << history.MA2 << " MA3: " << history.MA3
       << " MV3: " << history.MV3 << " MA4: " << history.MA4 << " MA5: " << history.MA5 << " MV5: " << history.MV5
       << " MA9: " << history.MA9 << " MV9: " << history.MV9 << " MA10: " << history.MA10 << " MV10: " << history.MV10
       << " MA19: " << history.MA19 << " MV19: " << history.MV19 << " MA20: " << history.MA20 << " MV20: "
       << history.MV20 << " OPEN: " << history.OPEN << " CLOSE: " << history.CLOSE << " HIGH: " << history.HIGH
       << " LOW: " << history.LOW << " VOL: " << history.VOL << " AMOUNT: " << history.AMOUNT << " AveragePrice: "
       << history.AveragePrice << " LastClose: " << history.LastClose << " BullN: " << history.BullN << " UpwardN: "
       << history.UpwardN << " NewHighN: " << history.NewHighN << " NewNoLowN: " << history.NewNoLowN << " NewLowN: "
       << history.NewLowN << " OpenVolume: " << history.OpenVolume << " UpdateTime: " << history.UpdateTime
       << " State: " << history.State;
    return os;
}

namespace factors {

    namespace {
        inline std::mutex g_factor_history_mutex{};
        inline tsl::robin_map<std::string, History> g_factor_history_map{};
        inline exchange::timestamp                  g_factor_history_date{};
    }

    void check_and_update(const exchange::timestamp& timestamp) {
        std::lock_guard<std::mutex> lock{g_factor_history_mutex};
        exchange::timestamp algin_date = timestamp.pre_market_time();
        if(g_factor_history_map.empty() || g_factor_history_date != algin_date) {
            g_factor_history_date = algin_date;
            auto adapter = HistoryFeature();
            auto cache_filename = adapter.Filename(g_factor_history_date);
            if(!std::filesystem::exists(cache_filename)) {
                spdlog::error("[history] cache file[{}], not found", cache_filename);
                return;
            }
            std::vector<History> list = encoding::csv::csv_to_slices<History>(cache_filename);
            for(auto const &v : list) {
                g_factor_history_map.insert_or_assign(v.Code, v);
            }
        }
    }

    /// 获取指定日期的History数据
    std::optional<History> get_history(const std::string& code, const exchange::timestamp& timestamp) {
        check_and_update(timestamp);
        auto it = g_factor_history_map.find(code);
        if(it != g_factor_history_map.end()) {
            return it->second;
        }
        return std::nullopt;
    }

} // namespace factors