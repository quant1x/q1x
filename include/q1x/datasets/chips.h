//#pragma once
//#ifndef QUANT1X_DATASETS_CHIPS_H
//#define QUANT1X_DATASETS_CHIPS_H 1
//
//#include "xdxr.h"
//
//namespace datasets {
//
//    // 统计指定日期的筹码分布情况
//    int update_dateset_chips(const std::string &start = cache::trains_begin_date, const std::string &end = exchange::current_day) {
//        // 隐藏终端光标以获得更流畅的显示效果
//        indicators::show_console_cursor(false);
//        auto dates = exchange::get_date_range(start, end);
//        auto count = dates.size();
//        indicators::ProgressBar barDates{
//                indicators::option::BarWidth{50},
//                indicators::option::Start{"["},
//                indicators::option::Fill{"="},
//                indicators::option::Lead{">"},
//                indicators::option::Remainder{" "},
//                indicators::option::End{"]"},
//                indicators::option::ShowElapsedTime{true},
//                indicators::option::ShowRemainingTime{true},
//                indicators::option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}},
//                indicators::option::ShowPercentage{true},
//                indicators::option::MaxProgress{count}
//        };
//        // 创建多进度条管理器
//        indicators::DynamicProgress<indicators::ProgressBar> bars(barDates);
//        std::string first = dates[0];
//        std::string last = dates[count - 1];
//        auto allCodes = exchange::GetCodeList();
//        indicators::ProgressBar barCodes(indicators::option::BarWidth{50},
//                                         indicators::option::ForegroundColor{indicators::Color::red},
//                                         indicators::option::ShowElapsedTime{true},
//                                         indicators::option::ShowRemainingTime{true},
//                                         indicators::option::PrefixText{first + ": fetching..."},
//                                         indicators::option::FontStyles{
//                                                 std::vector<indicators::FontStyle>{indicators::FontStyle::bold}},
//                                         indicators::option::MaxProgress{allCodes.size()});
//        bars.push_back(barCodes);
//        for (size_t i = 0; i < count; ++i) {
//            auto &feature_date = dates[i];
//            std::string front = feature_date;
//            bars[0].set_option(indicators::option::PostfixText{front + "/" + last});
//            bars[1].set_option(indicators::option::PrefixText{feature_date + ": fetching..."});
//            bars[1].set_progress(0);
//            for (auto &code: allCodes) {
//                cache::update_chips(code, feature_date);
////            bars[1].set_option(indicators::option::PrefixText{code + ": Pull complete "});
////            bars[1].tick();
////            if (bars[1].is_completed()) {
////                bars[1].mark_as_completed();
////                break;
////            }
//            }
//            bars[1].set_option(indicators::option::PrefixText{feature_date + ": Pull complete "});
//            bars[1].mark_as_completed();
//            bars[0].tick();
//        }
//        bars[0].mark_as_completed();
//        // 恢复终端光标显示
//        indicators::show_console_cursor(true);
//        return count;
//    }
//}
//
//#endif //QUANT1X_DATASETS_CHIPS_H
