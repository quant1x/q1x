#pragma once
#ifndef QUANT1X_CONFIG_H
#define QUANT1X_CONFIG_H 1

#include <q1x/std/api.h>
#include <mutex>

// 全部的配置信息
namespace config {
    constexpr const int cn_pre_market_hour   = 9; ///< 盘前9点
    constexpr const int cn_pre_market_minute = 0; ///< 盘点9点0分
    constexpr const int cn_pre_market_second = 0; ///< 盘点9点0分0秒

    constexpr const char *const cache_filename_date_layout = "{:%Y%m%d}"; ///< 缓存路径的日期格式

    constexpr const int64_t TenThousand = 10000;               // 万
    constexpr const int64_t Million     = 100 * TenThousand; // 百万
    constexpr const int64_t Billion     = 100 * Million;     // 1亿
}

#include <q1x/config/trader-parameter.h>

namespace config {

    //// 内存守卫结构
    //struct ConfigMemoryGuard {
    //    uint64_t     magic_header = 0xDEADBEEFCAFEBABE;  // 头部魔数
    //    std::string *filename_ptr;                       // 监控的字符串地址
    //    size_t       initial_capacity;                   // 初始容量
    //    size_t       initial_size;                       // 初始大小
    //    uint64_t     magic_footer = 0xCAFEBABEDEADBEEF;  // 尾部魔数
    //};

    struct BaseConfig {
        std::string filename;
        std::string homeDir;
        std::string cacheDir;
        std::string logsDir;
        bool running_in_debug = false;
    };

    // 全局守卫实例
    //extern ConfigMemoryGuard g_config_guard;

    extern std::once_flag global_cache_once;
    extern BaseConfig global_quant1x_config;

    // 配置文件路径
    std::string config_filename();
    // 获取交易配置参数
    std::shared_ptr<TraderParameter> TraderConfig();
    // 是否调试模式
    bool is_debug() noexcept;

    // 获取用户主路径
    std::string default_home_path();

    // 获取默认缓存路径
    std::string default_cache_path();

    // 获取元数据路径
    std::string get_meta_path();

    // 获取日志路径
    std::string get_logs_path();

    // 获取交易日历的缓存文件名
    std::string get_calendar_filename();

    // 获取证券列表的缓存文件名
    std::string get_security_filename();

    // 获取板块列表的缓存文件名${~/.quant1x/meta/blocks.${YYYY-mm-dd}}
    std::string get_sector_filename(const std::string &date);

    // 历史成交记录
    // 目录结构${trans}/${YYYY}/${YYYYMMDD}/${SecurityCode}.csv
    std::string get_historical_trade_filename(const std::string &code, const std::string& cache_date);

    // 筹码分布
    // 目录结构${trans}/${YYYY}/${YYYYMMDD}/${SecurityCode}.cd
    std::string get_chip_distribution_filename(const std::string &code, const std::string& cache_date);

    // 板块数据文件路径
    std::string get_block_path();

    // 除权除息文件路径
    std::string get_xdxr_path();

    // 日K线文件路径
    std::string get_day_path();
    // 除权除息文件名
    std::string get_xdxr_filename(const std::string &code);

    /**
     * @brief 日K线文件名
     * @param code 证券代码
     * @param forward 是否前复权, 后复权不考虑
     * @return 前复权返回文件名后缀是csv, 不复权是raw
     */
    std::string get_kline_filename(const std::string &code, bool forward = true);
    // 分时数据文件名
    std::string get_minute_filename(const std::string &code, const std::string &cache_date);

    // top10_holders_filename 前十大流通股股东缓存文件名
    std::string top10_holders_filename(const std::string& code, const std::string& date);

    // 财报报告数据文件名
    std::string reports_filename(const std::string& data);

    // 获取qmt缓存路径
    std::string get_qmt_cache_path();
}

#endif //QUANT1X_CONFIG_H
