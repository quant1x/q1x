#pragma once
#ifndef API_RUNTIME_H
#define API_RUNTIME_H 1

namespace runtime {

    /// 初始化
    void init(void);

    /// 设置服务退出标识
    void set_quit_flag(bool flag);

    /// 等待结束信号, 守护进程使用
    void wait_for_exit();

    // 设置日志模块, debug模式及控制台显示
    void logger_set(bool verbose, bool debug);

} // namespace runtime

#endif //API_RUNTIME_H
