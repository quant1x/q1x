#pragma once
#ifndef QUANT1X_STD_SAFE_H
#define QUANT1X_STD_SAFE_H 1

#include <cerrno>   // C标准库errno
#include <cstdio>   // snprintf
#include <cstring>  // C标准库字符串函数
#include <ctime>
#include <string>

namespace q1x {
    std::tm safe_localtime(std::time_t t) noexcept;
    std::tm safe_gmtime(std::time_t t) noexcept;

    namespace safe {
        // 安全的 localtime 函数，避免线程不安全
        inline std::tm localtime(std::time_t t) noexcept {
            return q1x::safe_localtime(t);
        }

        // 安全的 gmtime 函数，避免线程不安全
        inline std::tm gmtime(std::time_t t) noexcept {
            return q1x::safe_gmtime(t);
        }

        inline std::string strerror(int errnum) {
            char buf[256];

#if defined(_WIN32) || defined(_MSC_VER)
            // MSVC 安全版本
    if (strerror_s(buf, sizeof(buf), errnum) != 0) {
        std::snprintf(buf, sizeof(buf), "Unknown error %d", errnum);
    }
#elif (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && !_GNU_SOURCE
            // POSIX 标准版本（返回 int）
    if (strerror_r(errnum, buf, sizeof(buf)) != 0) {
        std::snprintf(buf, sizeof(buf), "Unknown error %d", errnum);
    }
#else
            // GNU 扩展版本（返回 char*）或其他平台
            char* msg = strerror_r(errnum, buf, sizeof(buf));
            if (msg != buf) {
                std::strncpy(buf, msg, sizeof(buf) - 1);
                buf[sizeof(buf) - 1] = '\0';
            }
#endif

            return buf;
        }
    }  // namespace safe

}  // namespace q1x

#endif  // QUANT1X_STD_SAFE_H