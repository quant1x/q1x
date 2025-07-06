#include <q1x/runtime/crash.h>
#include <q1x/runtime/core.h>
//#include <spdlog/spdlog.h>
//#define BACKWARD_HAS_DW 1
//#include <backward.hpp>

//namespace {
//    backward::SignalHandling sh; // 自动注册信号处理器
//}

void bar() {
    int* p = nullptr;
    *p = 42; // 真正访问空指针
}

void foo() {
    bar();
}

int main() {
    runtime::global_init();
    //runtime::logger_set(true, true);
    crash::InitCrashHandler();

    //spdlog::info("程序启动成功，即将触发崩溃...");

    foo(); // 触发崩溃

    return 0;
}