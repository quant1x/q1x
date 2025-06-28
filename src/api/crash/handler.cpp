#include <api/io/file.h>
#include <api/crash/handler.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <filesystem>
#include <iostream>
#include <vector>

namespace crash {
    namespace detail {

        static std::string application_name() {
            static std::string app_name = io::executable_absolute_path();
            return app_name;
        }

        // 初始化日志, 日志路径与执行程序相同, 去掉扩展名后增加_crash, 避免了创建目录及权限问题
        static void init_logger() {
            std::filesystem::path app_path = detail::application_name(); // 获取可执行文件完整路径
            std::filesystem::path log_file = app_path.stem().string() + "_crash.log"; // myapp_crash.log
            std::filesystem::path log_dir = app_path.parent_path(); // 同目录

            std::string filelog_name = (log_dir / log_file).string(); // 最终路径
            std::filesystem::create_directories(log_dir); // 创建目录（如果不存在）

            auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(filelog_name, 0, 0, false);
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

            std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
            auto logger = std::make_shared<spdlog::logger>(crash_logger_name, sinks.begin(), sinks.end());
            logger->set_level(spdlog::level::debug);
            spdlog::register_logger(logger);
        }

        std::once_flag once_crash_logger;

        static std::shared_ptr<spdlog::logger> get_logger() {
            std::call_once(once_crash_logger, [&]() {
                detail::init_logger();
            });
            auto logger = spdlog::get(crash_logger_name);
            if(!logger) {
                logger = spdlog::stdout_color_mt(crash_logger_name);
            }
            return logger;
        }

//        std::vector<CrashHook> hooks;
//        void invoke_hooks(int sig) {
//            for (const auto& hook : hooks) {
//                hook(sig);
//            }
//        }

    } // namespace detail
} // namespace crash

#include <csignal>
#if defined(OS_IS_WINDOWS)
//#define BACKWARD_HAS_DWARF 1 // 并不好用
#elif defined(OS_IS_LINUX)
#define BACKWARD_HAS_DW 1 // dw
#elif defined(OS_IS_APPLE)
#define BACKWARD_HAS_DWARF 1
#else
#error "not support this platform"
#endif

#include "backward.hpp"
//#if defined(WIN32) || defined(_WIN32)
//#   include <windows.h>
//#   include <dbghelp.h>
//#endif

namespace crash {

    namespace detail {
        static void LogStackTrace(const backward::StackTrace &st) {
            std::ostringstream oss;
            backward::Printer p;
            p.address = true;
            p.object = true;
            p.print(st, oss);
            get_logger()->error("调用栈：\n{}", oss.str());
            get_logger()->flush();
        }
    }

    namespace {

        constexpr int MAX_FRAMES = 64;
        // 全局变量：启用 backward-cpp 的信号处理
        backward::SignalHandling sh;

#if defined(WIN32) || defined(_WIN32)
        // Windows 下生成 .dmp 文件
        bool WriteMiniDump(EXCEPTION_POINTERS* pExceptionInfo) {
            auto dump_filename = detail::application_name() + ".dmp";
            HANDLE hFile = CreateFileA(dump_filename.c_str(),
                                       GENERIC_READ | GENERIC_WRITE,
                                       0,
                                       nullptr,
                                       CREATE_ALWAYS,
                                       FILE_ATTRIBUTE_NORMAL,
                                       nullptr);

            if (hFile != INVALID_HANDLE_VALUE) {
                MINIDUMP_EXCEPTION_INFORMATION mdei{};
                mdei.ThreadId = GetCurrentThreadId();
                mdei.ExceptionPointers = pExceptionInfo;

                BOOL result = MiniDumpWriteDump(GetCurrentProcess(),
                                                GetCurrentProcessId(),
                                                hFile,
                                                MiniDumpNormal,
                                                &mdei,
                                                nullptr,
                                                nullptr);

                CloseHandle(hFile);
                return result != FALSE;
            }

            return false;
        }

        // Windows 异常处理回调
        LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS* pExceptionInfo) {
            detail::get_logger()->error("[CRASH] Windows 异常被捕获: {:#08X}", pExceptionInfo->ExceptionRecord->ExceptionCode);

            SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
            SymInitialize(GetCurrentProcess(), nullptr, TRUE);

            // 打印调用栈（即使不能解析符号，也尽量显示函数名）
            backward::StackTrace st;
            st.load_from(pExceptionInfo, MAX_FRAMES);
            detail::LogStackTrace(st);

            // 保存 .dmp 文件
            WriteMiniDump(pExceptionInfo);

            return EXCEPTION_EXECUTE_HANDLER;
        }
#endif

#ifndef _WIN32
        // Unix 信号处理函数（Linux/macOS）
        void posix_signal_handler(int sig) {
            detail::get_logger()->error("[CRASH] Signal {} ({}) 被捕获", strsignal(sig), sig);
            switch (sig) {
                case SIGSEGV:
                    detail::get_logger()->error("[CRASH] SIGSEGV: 段错误, 访问非法内存地址");
                    break;
                case SIGBUS:
                    detail::get_logger()->error("[CRASH] SIGBUS: 总线错误, 对齐错误等硬件相关问题");
                    break;
                case SIGABRT:
                    detail::get_logger()->error("[CRASH] SIGABRT: 程序中止");
                    break;
                case SIGFPE:
                    detail::get_logger()->error("[CRASH] SIGFPE: 浮点运算异常（如除以零）");
                    break;
                case SIGILL:
                    detail::get_logger()->error("[CRASH] SIGILL: 非法指令（执行了非法操作码）");
                    break;
                default:
                    detail::get_logger()->error("[CRASH] 未知信号被捕获[{}]", sig);
            }

            // 打印调用栈（函数名 + 行号）
            backward::StackTrace st;
            st.load_here(MAX_FRAMES);
            detail::LogStackTrace(st);

            spdlog::shutdown();

            std::signal(sig, SIG_DFL);
            std::raise(sig);
            _exit(EXIT_FAILURE);
        }

        void install_posix() {
            struct sigaction sa;
            sa.sa_handler = posix_signal_handler;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = SA_RESTART;

            sigaction(SIGSEGV, &sa, nullptr);
            sigaction(SIGBUS,  &sa, nullptr);
            sigaction(SIGFPE,  &sa, nullptr);
            sigaction(SIGILL,  &sa, nullptr);
            sigaction(SIGABRT, &sa, nullptr);
            sigaction(SIGTRAP, &sa, nullptr);
        }
#endif

        // 初始化崩溃处理器
        void setup_crash_handlers() {
#if defined(WIN32) || defined(_WIN32)
            SetUnhandledExceptionFilter(windows_exception_handler);
#else
            install_posix();
#endif
        }

    } // anonymous namespace

    std::once_flag once_crash_handler;

    void InitCrashHandler() {
        std::call_once(once_crash_handler, [&]() {
            setup_crash_handlers();
            spdlog::info("[INFO] 崩溃处理器已初始化");
        });
    }

} // namespace crash
