#include <q1x/cache.h>
#include <indicators/dynamic_progress.hpp>
#include <indicators/progress_bar.hpp>
#include <boost/pfr/core.hpp>
#include <csv2/writer.hpp>

namespace cache {

    namespace fs = std::filesystem;

    namespace {
        // 常量定义
        constexpr const char *const timeLayoutOfState = "{:%H%M%S}";
        constexpr const char *const timeLayoutOfPhase = "{:%H:%M:%S}";
        const std::string lastUpdateTime = "22:00:00";
        const std::vector<std::string> allDateUpdateTimes = {"15:10:00", lastUpdateTime};

    } // 匿名命名空间

    std::string getVariablePath() {
        return config::default_cache_path() + "/var";
    }

    std::string stateFilename(const std::string& date, const exchange::timestamp& timestamp) {
        std::string fixedDate = exchange::timestamp(date).only_date();
        std::string tm = timestamp.toString(timeLayoutOfState).substr(0,6);

        std::string tmStr = fixedDate + "T" + tm;
        std::string filename = getVariablePath() + "/update." + tmStr;
        return filename;
    }


    bool checkUpdateState(const std::string& date, const exchange::timestamp& timestamp) {
        std::string filename = stateFilename(date, timestamp);
        return !fs::exists(filename);
    }

    void doneUpdate(const std::string& date, const exchange::timestamp& timestamp) {
        std::string filename = stateFilename(date, timestamp);
        util::check_filepath(filename, true);
        io::write_file(filename);
    }

    bool cleanExpiredStateFiles() {
        std::string statePath = getVariablePath();
        std::string pattern = statePath + "/update.*";

        try {
            for (const auto& entry : fs::directory_iterator(statePath)) {
                if (entry.path().string().find("update.") != std::string::npos) {
                    fs::remove(entry.path());
                }
            }
            return true;
        } catch (const std::exception& e) {
            spdlog::error("Error cleaning state files: {}", e.what());
            return false;
        }
    }

    void update_chips(const std::string &code, const std::string& date) {
        std::string securityCode = exchange::CorrectSecurityCode(code);
        std::string factor_date = date;
        auto cache_filename = config::get_historical_trade_filename(securityCode, factor_date);
        if (!fs::exists(cache_filename)) {
            return;
        }
        io::CSVReader<6,io::trim_chars<' ', '\t'>,io::double_quote_escape<',','"'>> csvReader(cache_filename);
        csvReader.read_header(io::ignore_extra_column, "time", "price", "vol", "num", "amount", "buyorsell");
        std::string Time;      // 时间 hh:mm
        f64 Price = 0;     // 价格
        f64 Vol = 0;          // 成交量, 股数
        i64 Num = 0;          // 历史成交数据中无该字段，但仍保留
        f64 Amount = 0;    // 金额
        int BuyOrSell = TradeDirection::TICK_NEUTRAL;    // 交易方向
        tsl::robin_map<int32_t, PriceLine> chipDistributionMap;
        int32_t front = 0;
        bool is_first = true;
        while (csvReader.read_row(Time, Price, Vol, Num, Amount, BuyOrSell)) {
            auto price = int32_t(Price * 100);
            PriceLine pl{};
            pl.price = price;
            if (is_first) {
                switch (BuyOrSell) {
                    case TradeDirection::TICK_BUY:
                        pl.buy = Vol;
                        break;
                    case TradeDirection::TICK_SELL:
                        pl.sell = Vol;
                        break;
                    default:
                        pl.buy = Vol/2;
                        pl.sell = Vol - pl.buy;
                        break;
                }
                is_first = false;
            } else {
                if ( price > front) {
                    BuyOrSell = TradeDirection::TICK_BUY;
                    pl.buy = Vol;
                } else if (price < front) {
                    BuyOrSell = TradeDirection::TICK_SELL;
                    pl.sell = Vol;
                } else {
                    BuyOrSell = TradeDirection::TICK_NEUTRAL;
                    pl.buy = Vol/2;
                    pl.sell = Vol - pl.buy;
                }
            }
            auto it = chipDistributionMap.find(pl.price);
            if(it != chipDistributionMap.end()) {
                pl.buy += it->second.buy;
                pl.sell += it->second.sell;
            }
            chipDistributionMap[pl.price] = pl;
            front = price;
        }
//        // 提取所有 key 到 vector
//        std::vector<int32_t> keys;
//        keys.reserve(chipDistributionMap.size());  // 预分配空间优化性能
//        for (const auto& [key, value] : chipDistributionMap) {
//            keys.push_back(key);
//        }
//        std::sort(keys.begin(), keys.end());
        std::vector<PriceLine> values;
        for (const auto& [_,v] : chipDistributionMap) {
            values.push_back(v);
        }
        std::sort(values.begin(), values.end(), [](const PriceLine& a, const PriceLine &b){
            return a.price< b.price;
        });

        data::Chips chips{};
        auto ofn = config::get_chip_distribution_filename(securityCode, factor_date);
        std::ofstream out(ofn, std::ios::binary);
        chips.set_date(factor_date);
        for(const auto & v : values) {
            auto l = chips.add_dist();
            l->set_price(v.price);
            l->set_buy(v.buy);
            l->set_sell(v.sell);
        }
        bool result = chips.SerializeToOstream(&out);
        (void)result;
    }

    int update_with_adapters(const std::vector<cache::DataAdapter*> &adapters, const exchange::timestamp& feature_date) {
        // 隐藏终端光标以获得更流畅的显示效果
        indicators::show_console_cursor(false);

        // 创建多进度条管理器
        indicators::DynamicProgress<indicators::ProgressBar> bars;

        // 主进度条为适配器
        auto count = adapters.size();
        indicators::ProgressBar barMain{
            indicators::option::BarWidth{50},
            indicators::option::ForegroundColor{indicators::Color::cyan},
            indicators::option::Start{"["},
            indicators::option::Fill{"="},
            indicators::option::Lead{">"},
            indicators::option::Remainder{" "},
            indicators::option::End{"]"},
            indicators::option::ShowElapsedTime{true},
            indicators::option::ShowRemainingTime{true},
            indicators::option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}},
            indicators::option::ShowPercentage{true},
            indicators::option::ShowSpeed{true},
            indicators::option::MaxProgress{count}
        };
        bars.push_back(barMain);
        bars[0].set_progress(0);

        auto first = adapters[0]->Key();
        auto allCodes = exchange::GetCodeList();
        indicators::ProgressBar barCodes(
            indicators::option::BarWidth{50},
            indicators::option::ForegroundColor{indicators::Color::yellow},
            indicators::option::ShowElapsedTime{true},
            indicators::option::ShowRemainingTime{true},
            indicators::option::PrefixText{first + ": fetching..."},
            indicators::option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}},
            indicators::option::ShowPercentage{true},
            indicators::option::ShowSpeed{true},
            indicators::option::MaxProgress{allCodes.size()});
        bars.push_back(barCodes);

        // 缓存日期
        auto cache_date = exchange::next_trading_day(feature_date);

        // 线程池大小，根据CPU核心数调整
        const size_t num_threads = std::min<size_t>(std::thread::hardware_concurrency(), 8);

        for (size_t idx = 0; idx < count; ++idx) {
            auto* adapter = adapters[idx];
            std::string module_name = std::format("{}({}/{})", adapter->Key(), (idx+1), count);

            spdlog::info("[update] plugin={}, start", module_name);
            bars[0].set_option(indicators::option::PrefixText{module_name + ""});
            bars[1].set_option(indicators::option::PrefixText{module_name + ""});
            bars[1].set_progress(0);
            bars[1].mark_as_started();
            bars[1].set_option(indicators::option::Completed {false});
            auto codeCount = allCodes.size();
            bars[1].set_option(indicators::option::MaxProgress{codeCount+0});

            // 初始化特征适配器
            bool is_feature_adapter = false;
            std::string cache_filename;
            cache::FeatureAdapter *featureAdapter = nullptr;

            if((adapter->Kind() & cache::PluginMaskFeature) == cache::PluginMaskFeature) {
                featureAdapter = dynamic_cast<cache::FeatureAdapter*>(adapter);
                if(featureAdapter) {
                    featureAdapter->init(feature_date);
                    cache_filename = featureAdapter->Filename(cache_date);
                    is_feature_adapter = true;
                    spdlog::info("特征适配器[{}]初始化完成，缓存文件: {}", featureAdapter->Name(), cache_filename);
                }
            }

            // 线程安全数据结构
            struct ThreadResult {
                std::vector<std::pair<std::string, std::vector<std::string>>> data; // <code, values>
                std::mutex mutex;
            };
            std::vector<ThreadResult> thread_results(num_threads);
            std::atomic<size_t> processed_codes = 0;
            std::mutex progress_mutex;

            // 线程处理函数
            auto process_batch = [&](/*const std::stop_token& stoken, */size_t thread_idx, size_t start, size_t end) {
                //(void) stoken;
                auto& result = thread_results[thread_idx];

                for (size_t i = start; i < end /*&& !stoken.stop_requested()*/; ++i) {
                    const auto& code = allCodes[i];
                    std::vector<std::string> values;

                    try {
                        // 调用适配器的方法
                        if(is_feature_adapter && featureAdapter) {
                            // 特征数据, 需要先clone一个实例, 然后用这个实例进行更新操作
                            auto feature = featureAdapter->clone();
                            feature->Update(code, feature_date);
                            values = feature->values();
                        } else {
                            // 基础数据是适配器自己内部聚合文件, 不需要外部干预
                            adapter->Update(code, feature_date);
                        }

                        // 线程安全地保存结果
                        if(is_feature_adapter && !values.empty()) {
                            std::lock_guard<std::mutex> lock(result.mutex);
                            result.data.emplace_back(code, std::move(values));
                        }

                        // 更新进度
                        size_t current = ++processed_codes;
                        {
                            std::lock_guard<std::mutex> lock(progress_mutex);
                            std::string codePrefix = std::format("{}({}/{})", code, current, codeCount);
                            bars[1].set_option(indicators::option::PrefixText{codePrefix + ""});
                            bars[1].tick();
                        }
                    } catch (const std::exception &e) {
                        spdlog::error("处理代码 {} 时出错: {}", code, e.what());
                    } catch (...) {
                        spdlog::error("处理代码 {} 时发生未知错误", code);
                    }
                }
            };

            // 创建并分配线程任务
            std::vector<std::thread> workers;
            size_t batch_size = (allCodes.size() + num_threads - 1) / num_threads;

            for (size_t t = 0; t < num_threads; ++t) {
                size_t start = t * batch_size;
                size_t end = std::min(start + batch_size, allCodes.size());
                if (start < end) {
                    workers.emplace_back(process_batch, t, start, end);
                }
            }

            // 等待线程完成
            for (auto& worker : workers) {
                if (worker.joinable()) worker.join();
            }

            // 合并结果并保存特征数据
            if(is_feature_adapter && featureAdapter) {
                try {
                    // 1. 收集所有结果
                    std::vector<std::pair<std::string, std::vector<std::string>>> all_data;
                    for (auto& result : thread_results) {
                        std::lock_guard<std::mutex> lock(result.mutex);
                        all_data.insert(all_data.end(),
                                        std::make_move_iterator(result.data.begin()),
                                        std::make_move_iterator(result.data.end()));
                    }

                    // 2. 按原始代码顺序排序
                    std::unordered_map<std::string, size_t> code_order;
                    for (size_t i = 0; i < allCodes.size(); ++i) {
                        code_order[allCodes[i]] = i;
                    }

                    std::sort(all_data.begin(), all_data.end(),
                              [&code_order](const auto& a, const auto& b) {
                                  return code_order[a.first] < code_order[b.first];
                              });

                    // 3. 准备最终数据
                    std::vector<std::vector<std::string>> final_data;
                    final_data.emplace_back(featureAdapter->headers()); // 表头

                    for (const auto& item : all_data) {
                        if (!item.second.empty()) {
                            final_data.push_back(item.second);
                        }
                    }

                    // 4. 写入文件
                    if (!final_data.empty()) {
                        util::check_filepath(cache_filename, true);
                        std::ofstream out_file(cache_filename, std::ios::binary|std::ios::out | std::ios::trunc);
                        if (out_file) {
                            csv2::Writer<csv2::delimiter<','>> writer(out_file);
                            writer.write_rows(final_data);
                            out_file.close();

                            spdlog::info("成功写入 {} 行数据到 {}", final_data.size(), cache_filename);

                            // 验证文件
                            if (std::filesystem::exists(cache_filename)) {
                                auto size = std::filesystem::file_size(cache_filename);
                                spdlog::info("文件验证: 大小 {} 字节", size);
                            } else {
                                spdlog::error("文件写入后不存在: {}", cache_filename);
                            }
                        } else {
                            spdlog::error("无法打开文件: {}", cache_filename);
                        }
                    } else {
                        spdlog::warn("没有特征数据需要保存");
                    }
                } catch (const std::exception& e) {
                    spdlog::error("保存特征数据时出错: {}", e.what());
                }
            }

            bars[1].set_option(indicators::option::PrefixText{module_name + ""});
            bars[1].mark_as_completed();
            bars[0].tick();
            spdlog::info("[update] plugin={}, end", module_name);
        }

        bars[0].mark_as_completed();
        // 恢复终端光标显示
        indicators::show_console_cursor(true);
        return int(count);
    }

    void update_all() {
        std::string today = api::today();
        std::string last_trading_day = exchange::last_trading_day().only_date();
        std::string current_time = exchange::timestamp::now().toString(timeLayoutOfPhase).substr(0, 8);
        bool should_update = false;
        exchange::timestamp update_phase{};
        // 判断更新时机
        if (today == last_trading_day) { // 交易日
            for (const auto& trigger_time : allDateUpdateTimes) {
                if (current_time >= trigger_time) {
                    update_phase = exchange::timestamp::parse_time(trigger_time);
                    should_update = checkUpdateState(today, update_phase);
                    if (should_update) break;
                }
            }
        } else { // 非交易日
            if (current_time >= lastUpdateTime) {
                update_phase = lastUpdateTime;
                should_update = checkUpdateState(today, update_phase);
            }
        }

        // 执行更新
        if (should_update && !update_phase.empty()) {
            //factors::SwitchDate(cache::DefaultCanReadDate());
            auto all_action = cache::Plugins();
            update_with_adapters(all_action);
            doneUpdate(today, update_phase);
        }
    }
}