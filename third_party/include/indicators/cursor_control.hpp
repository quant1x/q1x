#pragma once
#ifndef INDICATORS_CURSOR_CONTROL
#define INDICATORS_CURSOR_CONTROL 1

#if defined(_MSC_VER)
#if !defined(NOMINMAX)
#define NOMINMAX
#endif
#include <io.h>
#include <windows.h>
#else

#include <cstdio>

#endif

namespace indicators {

#if defined(_MSC_VER)

    static inline void show_console_cursor(bool const show) {
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_CURSOR_INFO cursorInfo;

        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = show; // set the cursor visibility
        SetConsoleCursorInfo(out, &cursorInfo);
    }

    static inline void erase_line() {
        auto hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        if (!hStdout) {
            return;
        }
        CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
        GetConsoleScreenBufferInfo(hStdout, &csbiInfo);

        COORD cursor;

        cursor.X = 0;
        cursor.Y = csbiInfo.dwCursorPosition.Y;

        DWORD count = 0;

        FillConsoleOutputCharacterA(hStdout, ' ', csbiInfo.dwSize.X, cursor, &count);
        FillConsoleOutputAttribute(hStdout, csbiInfo.wAttributes, csbiInfo.dwSize.X, cursor, &count);

        SetConsoleCursorPosition(hStdout, cursor);
    }

#else

    static inline void show_console_cursor(bool const show, bool blink = true) {
        if (show) {
            if (blink) {
                std::fputs("\033[?12;25h", stdout); // 启用光标 + 闪烁
            } else {
                std::fputs("\033[?25h", stdout);     // 启用光标 + 不闪烁
            }
        } else {
            std::fputs("\033[?25l", stdout);         // 关闭光标
        }
        std::fflush(stdout);
    }

    static inline void erase_line() {
        std::fputs("\r\033[K", stdout);
    }

#endif

} // namespace indicators

#endif