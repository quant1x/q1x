#include <api/io/http.h>
#include <cpr/cpr.h>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace io {

    static std::string time_point_to_http_date_cpp20(const std::chrono::system_clock::time_point& tp) {
        // 直接格式化为 GMT 时间
        return std::format("{:%a, %d %b %Y %H:%M:%S} GMT", std::chrono::floor<std::chrono::seconds>(tp));
    }

    static std::tm parseHttpDate(const std::string& httpDate) {
        std::tm tm = {};
        std::istringstream ss(httpDate);
        ss >> std::get_time(&tm, "%a, %d %b %Y %H:%M:%S GMT");
        return tm;
    }

    //const std::chrono::system_clock::time_point _zero_time = std::chrono::system_clock::from_time_t(0);

    // If-Modified-Since
    std::tuple<std::string, std::tm> request(const std::string &url, const std::chrono::system_clock::time_point& fileLastModified) {
        cpr::Header header = {};
        if (fileLastModified != _zero_time) {
            header.insert({"If-Modified-Since", time_point_to_http_date_cpp20(fileLastModified)});
        }
        cpr::Response response = cpr::Get(cpr::Url{url}, header);
        std::tm tm{};
        if (response.status_code == 200) {
            // 获取响应头中的 Last-Modified 字段
            auto lastModifiedIter = response.header.find("Last-Modified");
            if (lastModifiedIter != response.header.end()) {
                std::string lastModified = lastModifiedIter->second;
                //std::cout << "Last-Modified: " << lastModified << std::endl;
                tm = parseHttpDate(lastModified);
                //std::cout << "Last-Modified: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << std::endl;
            } else {
                //std::cout << "Last-Modified header not found in the response." << std::endl;
            }
            return {response.text, tm};
        } else {
            return {"", tm};
        }
    }
}
