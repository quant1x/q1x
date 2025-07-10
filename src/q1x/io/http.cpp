#include <q1x/io/http.h>
#include <cpr/cpr.h>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <q1x/std/time.h>

namespace io {

    static std::string time_point_to_http_date_cpp20(int64_t milliseconds_since_epoch) {
        // 将毫秒时间戳转换为 time_point
        auto tp = std::chrono::system_clock::time_point(
            std::chrono::milliseconds(milliseconds_since_epoch)
        );

        // 直接格式化为 GMT 时间
        return std::format("{:%a, %d %b %Y %H:%M:%S} GMT",
                           std::chrono::floor<std::chrono::seconds>(tp));
    }

//    static std::tm parseHttpDate(const std::string& httpDate) {
//        std::tm tm = {};
//        std::istringstream ss(httpDate);
//        ss >> std::get_time(&tm, "%a, %d %b %Y %H:%M:%S GMT");
//        return tm;
//    }

    static int64_t parse_http_date(const std::string& date) {
        auto ts = api::parse_date(date);
        return ts;
    }

    // If-Modified-Since
    std::tuple<std::string, int64_t> request(const std::string &url, int64_t fileLastModified) {
        cpr::Header header = {};
        if (fileLastModified != 0) {
            header.insert({"If-Modified-Since", time_point_to_http_date_cpp20(fileLastModified)});
        }
        cpr::Response response = cpr::Get(cpr::Url{url}, header);
        int64_t tm = 0;
        if (response.status_code == 200) {
            // 获取响应头中的 Last-Modified 字段
            auto lastModifiedIter = response.header.find("Last-Modified");
            if (lastModifiedIter != response.header.end()) {
                std::string lastModified = lastModifiedIter->second;
                //std::cout << "Last-Modified: " << lastModified << std::endl;
                tm = parse_http_date(lastModified);
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
