#include "q1x/std/safe.h"

namespace q1x {

    std::tm safe_localtime(std::time_t t) noexcept {
        std::tm result{};
#ifdef _WIN32
        localtime_s(&result, &t);
#else
        localtime_r(&t, &result);
#endif
        return result;
    }

    std::tm safe_gmtime(std::time_t t) noexcept {
        std::tm result{};
#ifdef _WIN32
        gmtime_s(&result, &t);
#else
        gmtime_r(&t, &result);
#endif
        return result;
    }
}  // namespace q1x