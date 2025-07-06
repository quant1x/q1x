#include <api/runtime.h>

#include <atomic>
#include <csignal>
#include <iostream>
#include <mutex>
#include <utility>

#ifdef _WIN32
#include <windows.h>
#else
#include <signal.h>
#endif

#include <api/crash/handler.h>
#include <api/except.h>
#include <api/base/scheduler.h>
#include <api/io/file.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace runtime {

    static std::string global_logs_path;

	/// 初始化日志路径, 如果全局路径为空且入参非空, 则设置全局路径
    static void init_logs_path(const std::string& logs_path) {
        auto temp_logs_path = strings::trim(logs_path);
        // 如果全局和入参都为空，抛异常
        if (global_logs_path.empty() && temp_logs_path.empty()) {
            throw std::invalid_argument("日志路径不能为空");
        }
        // 只在全局为默认值且入参非空时设置
        if (global_logs_path.empty() && !temp_logs_path.empty()) {
            // 确保路径以斜杠结尾
            if (temp_logs_path.back() != '/') {
                temp_logs_path += '/';
            }
            global_logs_path=temp_logs_path;
        }
        // 其它情况不做任何操作
    }

    std::atomic<bool> global_quit_flag(false);  // 全局退出标志
    std::atomic<bool> global_wait_flag(false);

    static void shutdown();

    void set_quit_flag(bool flag) {
        global_quit_flag = flag;
    }

#ifdef _WIN32
    // Windows控制台事件处理函数
    static BOOL WINAPI ConsoleHandler(DWORD event) {
        BOOL result = FALSE;
        switch (event) {
        case CTRL_C_EVENT:  // 必选事件：用户按下 Ctrl+C。
            spdlog::info("signal> Ctrl+C pressed. Exiting...");
            global_quit_flag = true;
            result = TRUE;
            break;
        case CTRL_CLOSE_EVENT:  // 必选事件：用户点击控制台窗口的关闭按钮（❌）
            spdlog::info("signal> Console closed. Saving state...");
            global_quit_flag = true;
            result = TRUE;
            break;
        case CTRL_SHUTDOWN_EVENT:  // 必选事件：系统即将关机或重启
            spdlog::info("signal> System shutting down. Cleaning up...");
            global_quit_flag = true;
            result = TRUE;
            break;
        case CTRL_BREAK_EVENT:  // 可选事件：用户按下 Ctrl+Break（或程序调用 GenerateConsoleCtrlEvent）
            spdlog::info("signal> Ctrl+Break pressed.");
            global_quit_flag = true;
            result = TRUE;  // 不退出，仅记录
            break;
        case CTRL_LOGOFF_EVENT:  // 可选事件：用户注销（Logoff）或切换账户
            spdlog::info("signal> User logging off.");
            global_quit_flag = true;
            result = TRUE;
            break;
        default:
            result = FALSE;
            break;
        }
        if (!global_wait_flag.load() && result) {
            shutdown();
        }
        return result;
    }

#else
    // Unix信号处理函数
    static void SignalHandler(int signum) {
        (void)signum;
        spdlog::warn("signal: {}", signum);
        global_quit_flag.store(true);
        if (!global_wait_flag.load() && global_quit_flag) {
            shutdown();
        }
    }
#endif

    // 设置信号/事件处理函数
    static void SetupSignalHandlers() {
#ifdef _WIN32
        SetConsoleCtrlHandler(ConsoleHandler, TRUE);
#else
        struct sigaction sa;
        sa.sa_handler = SignalHandler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        // 注册SIGINT（Ctrl+C）和SIGTERM（kill默认信号）
        sigaction(SIGINT, &sa, nullptr);
        sigaction(SIGTERM, &sa, nullptr);
        sigaction(SIGHUP, &sa, nullptr);
#endif
    }

    static void global_terminate_handler() {
        if (auto ex = std::current_exception()) {
            try {
                std::rethrow_exception(ex);
            }
            catch (const BaseException& e) {  // 捕获自定义异常
                spdlog::error("全局捕获 - 文件:{} 行号:{} 错误:{}", e.getFile(), e.getLine(), e.what());
            }
            catch (const std::exception& e) {  // 其他标准异常
                spdlog::error("全局捕获 - 标准异常: {} (type: {})", e.what(), typeid(e).name());
                // 对于system_error可以记录更多信息
                if (auto se = dynamic_cast<const std::system_error*>(&e)) {
                    spdlog::error("全局捕获 - Error code: {}, category: {}", se->code().value(), se->code().category().name());
                }
            }
            catch (...) {  // 未知异常;
                spdlog::error("全局捕获 - 未知异常");
            }
        }
        shutdown();
    }

    /// 隐藏全局初始化函数
    namespace {
        // 全局调度器, 智能指针动态分配
        // static std::unique_ptr<AsyncScheduler> global_scheduler = nullptr;
        AsyncScheduler* global_scheduler() {
            static AsyncScheduler scheduler;
            return &scheduler;
        }

        // 注册全部组件
        void init_all_components(const std::string& logs_path) {
            // auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
                logs_path + "/info.log", 0, 0, false);
            // 组合多个 sink 到一个 logger 中
            std::vector<spdlog::sink_ptr> sinks{ file_sink };
            std::string application_name = io::executable_name();
            auto combined_logger = std::make_shared<spdlog::logger>(application_name, begin(sinks), end(sinks));
            // 将组合后的 logger 设置为全局默认
            spdlog::set_default_logger(combined_logger);

            // 现在可以直接使用 spdlog::info(), spdlog::error() 等
            spdlog::info("This will go to both console and log file.");
            // spdlog的shutdown方法放在runtime::shotdown()中进行
            // std::atexit(spdlog::shutdown);  // 注册退出处理
            std::atexit(shutdown);
            std::set_terminate(global_terminate_handler);
            // 每3秒自动刷新一次（单位：秒）
            spdlog::flush_every(std::chrono::seconds(3));
            // spdlog::flush_on(spdlog::level::debug);
            console_set_utf8();
            SetupSignalHandlers();
            // global_scheduler = std::make_unique<AsyncScheduler>();
            // global_scheduler = new AsyncScheduler();
        }
    }  // namespace

    // 懒加载标志
    std::once_flag global_task_once;

    void console_set_utf8(void) {
#ifdef _WIN32
        // 设置控制台输出和输入代码页为UTF-8
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        //std::locale::global(std::locale(".65001"));
#endif
    }

    /// 全局初始化, 注册退出清理函数
    void global_init(const std::string& logs_path) {
		std::call_once(global_task_once, [&] {
            init_logs_path(logs_path);
            init_all_components(global_logs_path);
            crash::InitCrashHandler();
            });
    }

    // 设置日志模块, debug模式及控制台显示
    void logger_set(bool verbose, bool debug) {
        global_init();
        auto& tmp_sinks = spdlog::default_logger()->sinks();

        if (verbose) {
            // 检查是否已存在 console sink，避免重复添加
            bool has_console = false;
            for (const auto& sink : tmp_sinks) {
                if (dynamic_cast<spdlog::sinks::stdout_color_sink_mt*>(sink.get())) {
                    has_console = true;
                    break;
                }
            }
            if (!has_console) {
                auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
                tmp_sinks.push_back(console_sink);
            }
        } else {
            // 移除所有 console sink
            tmp_sinks.erase(
                std::remove_if(
                    tmp_sinks.begin(),
                    tmp_sinks.end(),
                    [](const spdlog::sink_ptr& sink) {
                        return dynamic_cast<spdlog::sinks::stdout_color_sink_mt*>(sink.get()) != nullptr;
                    }
                ),
                tmp_sinks.end()
            );
        }

        if (debug) {
            spdlog::set_level(spdlog::level::debug);
        }
    }

    // 追加一个任务到全局任务调度器
    task_id add_task(const std::string& cron_expr, std::function<void()> task) {
        global_init();
        auto id = global_scheduler()->schedule_cron(cron_expr, std::move(task));
        return id;
    }

    // 取消一个任务
    void cancel_task(task_id id) {
        global_init();
        global_scheduler()->cancel(id);
    }

    // 一般性退出, 包括正常退出和异常
    void shutdown() {
        global_init();
        spdlog::warn("刷新日志");
        spdlog::default_logger()->flush();
        spdlog::shutdown();
        _exit(0);
        //std::exit(0);
    }

    // 等待结束信号, 守护进程使用
    void wait_for_exit() {
        global_init();
        global_wait_flag = true;
        while (!global_quit_flag.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        spdlog::info("[*] 守护进程已关闭");
        shutdown();
    }
}  // namespace runtime
