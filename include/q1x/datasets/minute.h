#pragma once
#ifndef QUANT1X_DATASETS_MINUTE_H
#define QUANT1X_DATASETS_MINUTE_H 1

#include "xdxr.h"
#include <ranges>

namespace datasets {

    /**
     * @brief 分时数据, 每天固定条数
     */
    class DataMinute : public cache::DataAdapter {
    public:
        cache::Kind Kind() const override;
        std::string Owner() override;
        std::string Key() const override;
        std::string Name() const override;
        std::string Usage() const override;
        void Print(const std::string &code, const std::vector<exchange::timestamp> &dates) override;
        void Update(const std::string &code, const exchange::timestamp &date) override;
    };
}

#endif //QUANT1X_DATASETS_MINUTE_H
