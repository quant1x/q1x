#include <q1x/datasets.h>
#include <q1x/datasets/xdxr.h>
#include <q1x/datasets/kline.h>
#include <q1x/datasets/minute.h>
#include <q1x/factors/f10.h>
#include <q1x/factors/history.h>

namespace datasets {

    void init() {
        // 基础数据
        // 除权除息
        cache::Register(std::make_unique<DataXdxr>());
        // 日线
        cache::Register(std::make_unique<DataKLineRaw>());
        cache::Register(std::make_unique<DataKLine>());
        cache::Register(std::make_unique<DataMinute>());

        // 特征数据
        cache::Register(std::make_unique<F10Feature>());
        cache::Register(std::make_unique<HistoryFeature>());
    }

} // namespace datasets