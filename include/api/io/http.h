#pragma once
#ifndef API_IO_HTTP_H
#define API_IO_HTTP_H 1

#include <chrono>
#include <format>
#include <string>

namespace io {
    const std::chrono::system_clock::time_point _zero_time = std::chrono::system_clock::from_time_t(0);

    // If-Modified-Since
    std::tuple<std::string, std::tm> request(const std::string &url, const std::chrono::system_clock::time_point& fileLastModified=_zero_time);
}

#endif //API_IO_HTTP_H
