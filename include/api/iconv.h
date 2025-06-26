#pragma once
#ifndef API_BASE_ICONV_H
#define API_BASE_ICONV_H 1

#include <string>

/// 编码
namespace charsets {
    std::string utf8_to_gbk(const std::string& utf8_str);
    std::string gbk_to_utf8(const std::string& gbk_str);
} // namespace charsets



#endif //API_BASE_ICONV_H