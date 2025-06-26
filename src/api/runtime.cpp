#include <api/runtime.h>
#include <vector>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace runtime {

    // 一般性退出, 包括正常退出和异常
    void shutdown() {
        init();
        spdlog::warn("刷新日志");
        spdlog::default_logger()->flush();
        spdlog::shutdown();
        _exit(0);
        //std::exit(0);
    }

    void console_set_utf8(void) {
#ifdef OS_IS_WINDOWS
        // 设置控制台输出和输入代码页为UTF-8
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        //std::locale::global(std::locale(".65001"));
#endif
    }

    // 设置日志模块, debug模式及控制台显示
    void logger_set(bool verbose, bool debug) {
        init();
        if (verbose) {
            std::vector<spdlog::sink_ptr> &tmp_sinks = spdlog::default_logger()->sinks();
            // 创建控制台 sink
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            tmp_sinks.push_back(console_sink);
        }
        if (debug) {
            spdlog::set_level(spdlog::level::debug);
        }
    }

    std::atomic<bool> global_quit_flag(false);  // 全局退出标志
    std::atomic<bool> global_wait_flag(false);

    void set_quit_flag(bool flag) {
        global_quit_flag = flag;
    }

    void init(void) {

    }

    // 等待结束信号, 守护进程使用
    void wait_for_exit() {
        init();
        global_wait_flag = true;
        while (!global_quit_flag.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        shutdown();
    }

} // namespace runtime