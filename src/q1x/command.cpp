#include <q1x/command.h>
#include <q1x/engine/action.h>
#include <q1x/cache.h>

namespace quant1x {

    // 更新数据, 只更新最近一个交易日的数据
    int update(argparse::ArgumentParser &sub_parser) {
        std::vector<cache::DataAdapter*> adapters;
        std::cout << "全部数据 = " << updateAll.value << "," << sub_parser.is_used(cmd_flag_all) << std::endl;
        std::cout << "基础数据 = " << updateBase.value << "," << sub_parser.is_used(cmd_flag_base) << std::endl;
        std::cout << "特征数据 = " << updateFeatures.value << "," << sub_parser.is_used(cmd_flag_features) << std::endl;

        // 全部数据
        if(sub_parser.is_used(cmd_flag_all)) {
            auto all = cache::Plugins();
            adapters.insert(adapters.end(), all.begin(), all.end());
        } else {
            // 判断基础数据
            if (sub_parser.is_used(cmd_flag_base)) {
                std::vector<cache::DataAdapter *> base;
                if (updateBase.value == default_all) {
                    base = cache::Plugins(cache::PluginMaskBaseData);
                } else {
                    std::vector<std::string> names = strings::split(updateBase.value, ",");
                    base = cache::PluginsWithName(cache::PluginMaskBaseData, names);
                }
                adapters.insert(adapters.end(), base.begin(), base.end());
            }

            // 判断特征数据
            if (sub_parser.is_used(cmd_flag_features)) {
                std::vector<cache::DataAdapter *> features;
                if (updateFeatures.value == default_all) {
                    features = cache::Plugins(cache::PluginMaskFeature);
                } else {
                    std::vector<std::string> names = strings::split(updateFeatures.value, ",");
                    features = cache::PluginsWithName(cache::PluginMaskFeature, names);
                }
                adapters.insert(adapters.end(), features.begin(), features.end());
            }
        }
        if(adapters.empty()) {
            spdlog::warn("没有需要更新的数据适配器");
            return 0;
        }
        // 判断开始日期
        std::string tmp_start_date = exchange::last_trading_day().only_date();
        if(sub_parser.is_used(cmd_flag_start)) {
            tmp_start_date = updateStartDate.value;
        }

        exchange::timestamp start_date = exchange::timestamp::parse(tmp_start_date).pre_market_time();
        exchange::timestamp end_date = exchange::last_trading_day().pre_market_time();
        auto const & dates = exchange::date_range(start_date, end_date);
        int count = 0;
        for (auto const &timestamp : dates) {
            std::cout << "sample date: " << timestamp.only_date() << std::endl;
            count += cache::update_with_adapters(adapters, timestamp);
        }

        return count;
    }

} // namespace quant1x