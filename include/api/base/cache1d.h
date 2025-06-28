#pragma once
#ifndef API_BASE_CACHE1D_H
#define API_BASE_CACHE1D_H 1

// ==============================
// 每日定时更新的缓存机制
// ==============================

#include "once.h"

namespace runtime {

    template<typename T>
    class cache1d {
    public:
        template<typename Func, typename = std::enable_if_t<!std::is_same_v<std::decay_t<Func>, cache1d>>>
        explicit cache1d(Func&& init) : once_(PeriodicOnce<T>(std::forward<Func>(init))){
            //runtime::add_task(spec(), init_);
            //once_ = PeriodicOnce<T>(init_);
        }
        virtual std::string spec() const = 0;

        //cache1d() = delete;

        T& get() {
            return once_.get();
        }

        // 允许隐式转换
        operator T& () {
            return once_.get();
        }
        // 允许隐式转换
        operator const T& () const {
            return once_.get();
        }
    private:
        PeriodicOnce<T> once_;
    };

    // 基础数据
    template<typename T>
    class basedata : public runtime::cache1d<T> {
    public:
        template<typename Func>
        explicit basedata(Func init) : runtime::cache1d<T>(init){
            task_id_ = runtime::add_task(spec(), init);
            spdlog::warn("basedata add({})", task_id_);
        }

        ~basedata() {
            spdlog::warn("basedata cancel({})", task_id_);
            runtime::cancel_task(task_id_);
        }

        // 每天9点再次初始化
        std::string spec() const override {
            return "0 0 9 * * *";
            //return "*/5 * * * * *"; // 测试用的cron规则, 每5秒触发一次
        }
    private:
        int64_t task_id_;
    };

    // 基础数据
    template<typename T>
    class realtime : public runtime::cache1d<T> {
    public:
        template<typename Func>
        explicit realtime(Func init) : runtime::cache1d<T>(init){
            task_id_ = runtime::add_task(spec(), init);
            spdlog::warn("basedata add({})", task_id_);
        }

        ~realtime() {
            spdlog::warn("basedata cancel({})", task_id_);
            runtime::cancel_task(task_id_);
        }

        std::string spec() const override {
            return "*/1 * * * * *";
        }
    private:
        int64_t task_id_;
    };
}

#endif //API_BASE_CACHE1D_H
