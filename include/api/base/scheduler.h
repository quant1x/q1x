#pragma once
#ifndef API_BASE_SCHEDULER_H
#define API_BASE_SCHEDULER_H 1

#include <api/runtime.h>
#include <chrono>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <unordered_map>
#include <unordered_set>
#define BS_THREAD_POOL_NATIVE_EXTENSIONS
#include <BS_thread_pool.hpp>
#include <croncpp.h>
#include <spdlog/spdlog.h>

/**
 * @brief 调度任务自带延迟, 即上次没执行完本次延后到下一次执行
 */
class AsyncScheduler {
public:
    using Clock = std::chrono::system_clock;
    using TimePoint = Clock::time_point;

    struct ScheduledTask {
        TimePoint next_run;
        std::function<void()> task;
        runtime::task_id id;
        [[maybe_unused]] bool repeat;

        bool operator<(const ScheduledTask& rhs) const {
            return next_run > rhs.next_run; // 最小堆排序
        }
    };

    explicit AsyncScheduler(size_t thread_count = std::thread::hardware_concurrency())
            : pool_(thread_count), running_(true), next_id_(1) {
        scheduler_thread_ = std::thread([this] { scheduler_loop(); });
    }

    ~AsyncScheduler() {
        spdlog::warn("stop scheduler begin");
        stop();
        spdlog::warn("stop scheduler end");
    }

    runtime::task_id schedule_cron(const std::string& cron_expr, std::function<void()> task) {
        std::lock_guard lock(mutex_);

        try {
            const auto id = next_id_++;
            const auto cron = cron::make_cron(cron_expr);
            const auto first_run = cron::cron_next(cron, Clock::now());
            cron_tasks_.emplace(id, CronTask{false, cron, std::move(task)});
            enqueue_task(ScheduledTask{
                    first_run,
                    [this, id] { execute_cron_task(id); },
                    id,
                    true
            });

            return id;
        } catch (const cron::bad_cronexpr& e) {
            throw std::invalid_argument("Invalid cron expression: " + std::string(e.what()));
        }
    }

    void cancel(runtime::task_id id) {
        std::lock_guard lock(mutex_);
        // 防御性检查：确保任务存在
        if (cron_tasks_.find(id) != cron_tasks_.end()) {
            canceled_tasks_.insert(id);
            cron_tasks_.erase(id);
        }
        condition_.notify_all();
    }

    void stop() {
        if (!running_.exchange(false)) return;  // 确保只执行一次关闭

        // 1. 取消所有待处理任务
        {
            std::lock_guard lock(mutex_);
            canceled_tasks_.clear();
            cron_tasks_.clear();
            while (!task_queue_.empty()) {
                task_queue_.pop();
            }
        }

        // 2. 唤醒所有等待线程
        condition_.notify_all();

        // 3. 等待调度器线程结束
        if (scheduler_thread_.joinable()) {
            scheduler_thread_.join();
        }

        // 4. 等待线程池任务完成
        pool_.wait();
    }

private:
    struct CronTask {
        bool cron_running;
        cron::cronexpr expr;
        std::function<void()> task;
    };

    BS::thread_pool<BS::tp::none> pool_;
    std::thread scheduler_thread_;
    std::atomic<bool> running_;
    std::priority_queue<ScheduledTask> task_queue_;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::atomic<runtime::task_id> next_id_;
    std::unordered_set<runtime::task_id> canceled_tasks_;
    std::unordered_map<runtime::task_id, CronTask> cron_tasks_;

    void enqueue_task(ScheduledTask&& task) {
        task_queue_.push(std::move(task));
        condition_.notify_one();
    }

    void execute_cron_task(runtime::task_id id) {
        std::function<void()> task;
        cron::cronexpr cron;

        {
            std::lock_guard lock(mutex_);
            if (canceled_tasks_.count(id)) return;

            auto& cron_task = cron_tasks_.at(id);
            // 如果已经在运行，则跳过本次调度
            if (cron_task.cron_running) {
                spdlog::warn("Task {} skipped: previous execution still running", id);
                return;
            }
            //spdlog::debug("cron-2");

            cron_task.cron_running = true; // 标记为正在运行
            task = cron_task.task;
            cron = cron_task.expr;
        }

        try {
            task();
        } catch (...) {
            // 处理任务异常
        }
        {
            std::lock_guard lock(mutex_);
            if (canceled_tasks_.count(id)) return;

            auto& cron_task = cron_tasks_.at(id);
            cron_task.cron_running = false; // 标记为不运行
        }

        reschedule_cron(id, cron);
    }

    void reschedule_cron(runtime::task_id id, const cron::cronexpr& cron) {
        std::lock_guard lock(mutex_);
        if (canceled_tasks_.count(id)) return;

        try {
            const auto next_time = cron::cron_next(cron, Clock::now());
            enqueue_task(ScheduledTask{
                    next_time,
                    [this, id] { execute_cron_task(id); },
                    id,
                    true
            });
        } catch (const cron::bad_cronexpr& e) {
            // 处理无效的cron表达式
        }
    }

    void scheduler_loop() {
        while (running_) {
            std::unique_lock lock(mutex_);

            if (task_queue_.empty()) {
                // 添加运行状态检查的条件等待
                condition_.wait(lock, [this] {
                    return !task_queue_.empty() || !running_;
                });
                if (!running_) break;
                continue;
            }

            const auto& top_task = task_queue_.top();
            if (Clock::now() >= top_task.next_run) {
                auto& task = top_task;
                task_queue_.pop();
                lock.unlock();

                if (!canceled_tasks_.count(task.id)) {
                    // 在线程池任务中添加运行状态检查
                    pool_.detach_task([this, task] {
                        if (running_) {
                            task.task();
                        }
                    });
                }
            } else {
                // 带条件的超时等待
                condition_.wait_until(lock, top_task.next_run,
                                      [this] { return !running_; });
                if (!running_) break;
            }
        }
    }
};

#endif //API_BASE_SCHEDULER_H
