#pragma once
#ifndef API_RUNTIME_H
#define API_RUNTIME_H 1

#include <api/std.h>

namespace runtime {

    using task_id = int64_t;

    void console_set_utf8(void);

    /// 全局初始化, 注册退出清理函数
    void global_init(const std::string& logs_path = "logs");

    /// 设置日志模块, debug模式及控制台显示
    void logger_set(bool verbose = false, bool debug = false);

    /// 设置服务退出标识
    void SetQuitFlag(bool flag);

    /// 等待结束信号, 守护进程使用
    void wait_for_exit();

    /// 追加一个任务到全局任务调度器
    task_id add_task(const std::string& cron_expr, std::function<void()> task);

    /// 取消一个任务
    void cancel_task(task_id id);
}

#endif //API_RUNTIME_H
