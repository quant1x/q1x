//============================================================
// exchange 证券代码相关                                      //
//============================================================
#include <q1x/exchange/code.h>
#include <q1x/std/api.h>
#include <vector>

namespace exchange {

    /**
     * @brief 根据市场类型和代码生成完整证券代码
     * @param market 市场类型
     * @param symbol 原始代码
     * @return 完整证券代码（格式：市场标识+代码）
     */
    std::string GetSecurityCode(MarketType market, const std::string &symbol) {
        switch (market) {
            case MarketType::USA:
                return market_usa + symbol;
            case MarketType::HongKong:
                return market_hongkong + symbol.substr(0, 5);
            case MarketType::BeiJing:
                return market_beijing + symbol.substr(0, 6);
            case MarketType::ShenZhen:
                return market_shenzhen + symbol.substr(0, 6);
            default:
                return market_shangHai + symbol.substr(0, 6);
        }
    }

    // 上海证券交易所
    // 主板: 60xxxx
    // 科创板: 688xxx
    // B股: 900xxx
    // 优先股: 360xxx
    // 科创板存托凭证: 689xxx
    // 申购/配股/投票: 7xxxxx
    // 上海总规则: http://www.sse.com.cn/lawandrules/guide/stock/jyglywznylc/zn/a/20230209/4ae280c58535e0424b3a9c743c47e6b9.docx
    // 0: 国债/指数, 000 上证指数系列和中证指数系列, 00068x科创板指数
    // 1: 债券
    // 2: 回购
    // 3: 期货
    // 4: 备用
    // 5: 基金/权证
    // 6: A股
    // 7: 非交易业务(发行, 权益分配)
    // 8: 备用, 通达信编制板块指数占用880,881
    // 9: B股
    static const std::vector<std::string> shanghaiMainBoardPrefixes = {"50", "51", "60", "68", "90", "110", "113", "132", "204"};
    static const std::vector<std::string> shanghaiSpecialPrefixes = {"5", "6", "9", "7"};
    static const std::vector<std::string> shanghaiOtherPrefixes = {"88"};
    // 深圳交易所
    // 主板: 000,001
    // 中小板: 002,003,004
    // 创业板: 30xxxx
    // 优先股: 140xxx
    // 深圳总规则: https://zhuanlan.zhihu.com/p/63064991
    // 0: 股票
    // 1: 国债/基金
    // 2: B股
    // 30: 创业板
    // 36: 投票, 369999用于深交所认证业务的密码激活/密码挂失
    // 37: 增发/可转债申购
    // 38: 配股/可转债优先权
    // 395: 成家量统计指数
    // 399: 指数
    static const std::vector<std::string> shenzhenMainBoardPrefixes = {"00", "12", "13", "18", "15", "16", "18", "20", "30", "39",
                                                                       "115", "1318"};
    // 北京交易所, 只处理4和8开头的代码
    static const std::vector<std::string> beijingMainBoardPrefixes = {"4", "8"};

    /**
     * @brief 根据代码判断所属市场
     * @param symbol 证券代码
     * @return 市场标识（sh/sz/bj等）
     */
    std::string GetMarket(const std::string &symbol) {
        std::string code = strings::trim(symbol);
        std::string market = market_shangHai;

        if (strings::startsWith(code, marketFlags)) {
            market = code.substr(0, 2);
            market = strings::to_lower(market);
        } else if (strings::endsWith(code, marketFlags)) {
            size_t len = code.size();
            market = code.substr(len - 2);
            market = strings::to_lower(market);
        } else if (strings::startsWith(code, shanghaiMainBoardPrefixes)) {
            market = market_shangHai;
        } else if (strings::startsWith(code, shenzhenMainBoardPrefixes)) {
            market = market_shenzhen;
        } else if (strings::startsWith(code, shanghaiSpecialPrefixes)) {
            market = market_shangHai;
        } else if (strings::startsWith(code, shanghaiOtherPrefixes)) {
            market = market_shangHai;
        } else if (strings::startsWith(code, beijingMainBoardPrefixes)) {
            market = market_beijing;
        }
        return market;
    }

    /**
     * @brief 获取市场ID
     * @param symbol 证券代码
     * @return 市场类型枚举值
     */
    MarketType GetMarketId(const std::string &symbol) {
        std::string market = GetMarket(symbol);
        if (market == market_shangHai) return MarketType::ShangHai;
        if (market == market_shenzhen) return MarketType::ShenZhen;
        if (market == market_beijing) return MarketType::BeiJing;
        return MarketType::ShangHai;
    }

    /**
     * @brief 根据市场ID获取市场标识
     * @param marketId 市场类型枚举
     * @return 市场标识字符串
     */
    std::string GetMarketFlag(MarketType marketId) {
        switch (marketId) {
            case MarketType::ShenZhen:
                return market_shenzhen;
            case MarketType::BeiJing:
                return market_beijing;
            case MarketType::HongKong:
                return market_hongkong;
            case MarketType::USA:
                return market_usa;
            default:
                return market_shangHai;
        }
    }

    /**
     * @brief 综合解析证券代码
     * @param symbol 原始证券代码
     * @return 元组（市场ID，市场标识，纯代码）
     */
    std::tuple<MarketType, std::string, std::string> DetectMarket(const std::string &symbol) {
        std::string pureCode = strings::trim(symbol);
        std::string marketCode = market_shangHai;

        if (strings::startsWith(pureCode, marketFlags)) {
            marketCode = pureCode.substr(0, 2);
            //std::transform(marketCode.begin(), marketCode.end(), marketCode.begin(), ::tolower);
            marketCode = strings::to_lower(marketCode);
            pureCode = (pureCode[2] == '.') ? pureCode.substr(3) : pureCode.substr(2);
        } else if (strings::endsWith(pureCode, marketFlags)) {
            size_t len = pureCode.size();
            marketCode = pureCode.substr(len - 2);
            //std::transform(marketCode.begin(), marketCode.end(), marketCode.begin(), ::tolower);
            marketCode = strings::to_lower(marketCode);
            pureCode = pureCode.substr(0, len - 3);
        } else if (strings::startsWith(pureCode, shanghaiMainBoardPrefixes)) {
            marketCode = market_shangHai;
        } else if (strings::startsWith(pureCode, shenzhenMainBoardPrefixes)) {
            marketCode = market_shenzhen;
        } else if (strings::startsWith(pureCode, shanghaiSpecialPrefixes)) {
            marketCode = market_shangHai;
        } else if (strings::startsWith(pureCode, shanghaiOtherPrefixes)) {
            marketCode = market_shangHai;
        } else if (strings::startsWith(pureCode, beijingMainBoardPrefixes)) {
            marketCode = market_beijing;
        }

        MarketType marketId = MarketType::ShangHai;
        if (marketCode == market_shangHai) marketId = MarketType::ShangHai;
        else if (marketCode == market_shenzhen) marketId = MarketType::ShenZhen;
        else if (marketCode == market_beijing) marketId = MarketType::BeiJing;
        else if (marketCode == market_hongkong) marketId = MarketType::HongKong;

        return {marketId, marketCode, pureCode};
    }

    /**
     * @brief 判断是否为指数代码（通过市场ID和纯代码）
     * @param marketId 市场ID
     * @param symbol 纯代码
     * @return 是否为指数
     */
    bool AssertIndexByMarketAndCode(MarketType marketId, const std::string &symbol) {
        if (marketId == MarketType::ShangHai && strings::startsWith(symbol, {"000", "880", "881"})) return true;
        if (marketId == MarketType::ShenZhen && strings::startsWith(symbol, {"399"})) return true;
        return false;
    }

    /**
     * @brief 判断是否为指数代码（通过完整证券代码）
     * @param securityCode 完整证券代码
     * @return 是否为指数
     */
    bool AssertIndexBySecurityCode(const std::string &securityCode) {
        auto [marketId, _, code] = DetectMarket(securityCode);
        return AssertIndexByMarketAndCode(marketId, code);
    }

    /**
     * @brief 判断并修正板块代码
     * @param securityCode 完整证券代码（会被修改）
     * @return 是否为板块代码
     */
    bool AssertBlockBySecurityCode(std::string *securityCode) {
        auto [marketId, flag, code] = DetectMarket(*securityCode);
        if (marketId != MarketType::ShangHai || !strings::startsWith(code, {"880", "881"})) return false;
        *securityCode = flag + code;
        return true;
    }

    /**
     * @brief 判断是否为ETF（通过市场ID和纯代码）
     * @param marketId 市场ID
     * @param symbol 纯代码
     * @return 是否为ETF
     */
    bool AssertETFByMarketAndCode(MarketType marketId, const std::string &symbol) {
        return marketId == MarketType::ShangHai && strings::startsWith(symbol, {"510"});
    }

    /**
     * @brief 判断是否为个股（通过市场ID和纯代码）
     * @param marketId 市场ID
     * @param symbol 纯代码
     * @return 是否为个股
     */
    bool AssertStockByMarketAndCode(MarketType marketId, const std::string &symbol) {
        if (marketId == MarketType::ShangHai && strings::startsWith(symbol, {"60", "68", "510"})) return true;
        if (marketId == MarketType::ShenZhen && strings::startsWith(symbol, {"00", "30"})) return true;
        return false;
    }

    /**
     * @brief 判断是否为个股（通过完整证券代码）
     * @param securityCode 完整证券代码
     * @return 是否为个股
     */
    bool AssertStockBySecurityCode(const std::string &securityCode) {
        auto [marketId, _, code] = DetectMarket(securityCode);
        return AssertStockByMarketAndCode(marketId, code);
    }

    /**
     * @brief 修正证券代码格式
     * @param symbol 原始代码
     * @return 标准化后的证券代码
     */
    std::string CorrectSecurityCode(const std::string &symbol) {
        if (symbol.empty()) return "";
        auto [_, mFlag, mSymbol] = DetectMarket(symbol);
        return mFlag + mSymbol;
    }

    /**
     * @brief 判断证券代码类型
     * @param securityCode 完整证券代码
     * @return 证券类型枚举
     */
    TargetKind AssertCode(const std::string &securityCode) {
        auto [marketId, _, code] = DetectMarket(securityCode);
        if (marketId == MarketType::ShangHai) {
            if (strings::startsWith(code, {"880", "881"})) return TargetKind::BLOCK;
            if (strings::startsWith(code, {"000"})) return TargetKind::INDEX;
            if (strings::startsWith(code, {"510"})) return TargetKind::ETF;
        }
        if (marketId == MarketType::ShenZhen && strings::startsWith(code, {"399"})) return TargetKind::INDEX;
        return TargetKind::STOCK;
    }

    // 检查指数和个股
    bool checkIndexAndStock(const std::string &securityCode) {
        if (AssertIndexBySecurityCode(securityCode)) {
            return true;
        }
        if (AssertStockBySecurityCode(securityCode)) {
            return true;
        }
        return false;
    }
}
