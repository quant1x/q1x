#include <q1x/factors/base.h>
#include <q1x/exchange.h>

namespace factors {

    namespace {
        std::unordered_map<std::string, std::vector<level1::XdxrInfo>> xdxrs_map;
        auto xdxrs_once =  RollingOnce::create("factors-xdxrs", exchange::cron_expr_daily_9am);

        void lazy_load_xdxr_list() {
            xdxrs_map.clear();
            auto all_codes = exchange::GetCodeList();
            // 预分配空间，防止插入时频繁 rehash
            xdxrs_map.reserve(all_codes.size());
            for(auto const & security_code : all_codes) {
                auto xdxr_infos = datasets::load_xdxr(security_code);
                std::sort(xdxr_infos.begin(), xdxr_infos.end(),
                          [](level1::XdxrInfo& a, level1::XdxrInfo& b) {
                              return a.Date < b.Date;
                          });
                xdxrs_map.emplace(security_code, std::move(xdxr_infos));
            }
        }
    }

    // 通过证券代码获取最新的除权除息列表
    std::span<const level1::XdxrInfo> get_xdxr_list(const std::string& security_code) {
        xdxrs_once->Do(lazy_load_xdxr_list);
        auto it = xdxrs_map.find(security_code);
        if(it != xdxrs_map.end()) {
            return it->second;
        }
        return {};
    }

    // 从除权除息的列表提取IPO日期
    std::optional<std::string> ipo_date_from_xdxrs(std::span<const level1::XdxrInfo> xdxrs) {
        for(auto const &v : xdxrs) {
            if(v.Category != 5) continue;
            // 如果首次, 前流通前总股本为0且后流通后总股本大于0, 即为上市日期
            if(v.QianLiuTong == 0 && v.QianZongGuBen == 0 && v.HouLiuTong > 0 && v.HouZongGuBen>0) {
                return v.Date;
            }
        }
        return std::nullopt;
    }

    // 聚合给定一个时间范围内的复权因子
    std::vector<CumulativeAdjustment> combine_adjustments_in_period(
        std::span<const level1::XdxrInfo> xdxrs,
        const exchange::timestamp& start_date,
        const exchange::timestamp& end_date)
    {
        // 1. 过滤符合时间范围的除权除息数据
        std::vector<CumulativeAdjustment> result;
        for (const auto& info : xdxrs) {
            if (!info.is_adjust()) continue;

            // 统一盘前时间
            exchange::timestamp event_ts = exchange::timestamp::parse(info.Date).pre_market_time();
            if (event_ts < start_date || event_ts > end_date) continue;

            auto [m, a] = info.adjustFactor();
            for(auto & i : result) {
                auto factor = &i;
                // 叠加复权因子
                factor->m *= m;
                factor->a = m*factor->a + a;
                ++factor->no;
            }
            result.push_back({event_ts,m,a,1});
        }
        return result;
    }

    // 全局变量
    namespace forward {
        std::mutex mutex_klines;
        std::unordered_map<std::string, std::vector<datasets::KLine>> routineLocal_klines;
    }

    // 检查K线偏移量
    template<typename T>
    int check_kline_offset(const std::vector<T> &klines, const std::string &date) {
        size_t rows = klines.size();
        int offset = 0;
        for (size_t i = 0; i < rows; i++) {
            std::string klineDate = klines[rows - 1 - i].Date;
            if (klineDate < date) {
                return -1;
            } else if (klineDate == date) {
                break;
            } else {
                offset++;
            }
        }
        if (size_t(offset) + 1 >= rows) {
            return -1;
        }
        return offset;
    }

    // 更新缓存K线
    void UpdateCacheKLines(const std::string &securityCode, const std::vector<datasets::KLine> &klines) {
        if (klines.empty()) {
            return;
        }
        std::lock_guard<std::mutex> lock(forward::mutex_klines);
        forward::routineLocal_klines[securityCode] = klines;
    }

    // 捡出指定日期的K线数据
    std::vector<datasets::KLine> checkout_klines(const std::string &code, const std::string &date) {
        std::string securityCode = exchange::CorrectSecurityCode(code);
        exchange::timestamp ts(date);
        std::string fixed_date = ts.only_date();

        // 1. 取缓存的K线
        std::vector<datasets::KLine> cacheKLines;
        {
            std::lock_guard<std::mutex> lock(forward::mutex_klines);
            auto it = forward::routineLocal_klines.find(securityCode);
            if (it != forward::routineLocal_klines.end()) {
                cacheKLines = it->second;
            }
        }

        if (cacheKLines.empty()) {
            cacheKLines = datasets::load_kline(securityCode);
            UpdateCacheKLines(securityCode, cacheKLines);
        }

        size_t rows = cacheKLines.size();
        if (rows == 0) {
            return {};
        }

        // 1.1 检查是否最新数据
        datasets::KLine kline = cacheKLines[rows - 1];
        if (kline.Date < fixed_date) {
            // 数据太旧, 重新加载
            cacheKLines = datasets::load_kline(securityCode);
            UpdateCacheKLines(securityCode, cacheKLines);
        }

        // 2. 对齐数据缓存的日期, 过滤可能存在停牌没有数据的情况
        int offset = check_kline_offset(cacheKLines, fixed_date);
        if (offset < 0) {
            return {};
        }

        // 3. 返回指定日期前的K线数据
        std::vector<datasets::KLine> result(cacheKLines.begin(), cacheKLines.end() - offset);
        return result;
    }

    namespace raw {
        std::mutex mutex_raw_klines;
        std::unordered_map<std::string, std::vector<datasets::KLineRaw>> routineLocal_raw_klines;

        void update_cache_raw_klines(const std::string &securityCode, const std::vector<datasets::KLineRaw> &klines) {
            if (klines.empty()) {
                return;
            }
            std::lock_guard<std::mutex> lock(mutex_raw_klines);
            routineLocal_raw_klines[securityCode] = klines;
        }
    }

    std::vector<datasets::KLine> convert_to_klines(const std::vector<datasets::KLineRaw>& raws, int offset){
        size_t fixed_count = raws.size() - offset;
        std::vector<datasets::KLine> result;
        result.reserve(fixed_count);

        for (size_t i = 0; i < fixed_count; ++i) {
            const auto& raw = raws[i];
            result.emplace_back(datasets::KLine{
                raw.Date,
                raw.Open,
                raw.Close,
                raw.High,
                raw.Low,
                raw.Volume,
                raw.Amount,
                raw.Up,
                raw.Down,
                raw.Datetime,
                0
            });
        }

        return result;
    }

    // 原始数据一次性复权
    std::vector<datasets::KLine> klines_forward_adjusted_to_date(const std::string& code, const std::string& date) {
        // 1. 获取 securityCode
        // 2. 获取缓存或加载原始数据
        // 3. 检查是否需要更新
        // 4. 检查 offset
        // 5. 转换 KLineRaw -> KLine
        // 6. 应用复权因子
        std::string securityCode = exchange::CorrectSecurityCode(code);
        exchange::timestamp ts(date);
        std::string fixed_date = ts.only_date();

        // 1. 取缓存的K线
        std::vector<datasets::KLineRaw> cache_raw_klines;
        {
            std::lock_guard<std::mutex> lock(raw::mutex_raw_klines);
            auto it = raw::routineLocal_raw_klines.find(securityCode);
            if (it != raw::routineLocal_raw_klines.end()) {
                cache_raw_klines = it->second;
            }
        }

        if (cache_raw_klines.empty()) {
            cache_raw_klines = datasets::load_kline_raw(securityCode);
            raw::update_cache_raw_klines(securityCode, cache_raw_klines);
        }

        size_t rows = cache_raw_klines.size();
        if (rows == 0) {
            return {};
        }

        // 1.1 检查是否最新数据
        datasets::KLineRaw last_kline = cache_raw_klines[rows - 1];
        if (last_kline.Date < fixed_date) {
            // 数据太旧, 重新加载
            cache_raw_klines = datasets::load_kline_raw(securityCode);
            raw::update_cache_raw_klines(securityCode, cache_raw_klines);
        }

        // 2. 对齐数据缓存的日期, 过滤可能存在停牌没有数据的情况
        int offset = check_kline_offset(cache_raw_klines, fixed_date);
        if (offset < 0) {
            return {};
        }

        auto result = convert_to_klines(cache_raw_klines, offset);
        auto xdxrs = get_xdxr_list(securityCode);
        // 确定前复权的时间范围
        auto ts_start = exchange::timestamp(result[0].Date).pre_market_time();
        auto ts_end = exchange::timestamp(result.back().Date).pre_market_time();
        //auto cbs = combine_adjustments_in_period(xdxrs, ts_start, ts_end);
        apply_forward_adjustments(result, xdxrs, ts_start, ts_end);
        return result;
    }

} // namespace factors