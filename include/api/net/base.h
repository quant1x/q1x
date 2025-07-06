#pragma once
#ifndef API_NET_BASE_H
#define API_NET_BASE_H 1

#if defined(WIN32) || defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00 // for asio
#endif
#ifdef _WIN32
#include <winsock2.h>
#endif
#endif

//#define ASIO_HEADER_ONLY
#include <asio.hpp>
#include <api/std.h>
#include <fmt/core.h>

// 自定义格式化器：将 asio::ip::basic_endpoint 转换为字符串
template <>
struct fmt::formatter<asio::ip::basic_endpoint<asio::ip::tcp>> {
    // 解析格式化规则（这里不需要特殊处理，直接返回）
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    // 格式化逻辑
    template <typename FormatContext>
    auto format(const asio::ip::basic_endpoint<asio::ip::tcp>& endpoint, FormatContext& ctx) const -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), "{}:{}", endpoint.address().to_string(), endpoint.port());
    }
};

namespace net {
    inline std::string address(const asio::ip::tcp::endpoint& ep) {
        return fmt::format("{}:{}", ep.address().to_string(), ep.port());
    }
}

#endif //API_NET_BASE_H
