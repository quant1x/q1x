#include <q1x/test/test.h>
#include <q1x/exchange.h>

TEST_CASE("update-calendar", "[calendar]") {
    runtime::global_init();
    spdlog::set_level(spdlog::level::debug);
    spdlog::debug("calendar-1");
    exchange::update_calendar();
    spdlog::debug("calendar-2");
//    auto list = util::js_decode(text);
//    for(const auto & v: list) {
//        std::cout << v << std::endl;
//    }
}

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <ctime>

class Decoder {
private:
    // Constants
    static constexpr int64_t l = 86400000; // 毫秒每天 (864e5)
    static constexpr int64_t u_base = 7657;
    static constexpr int32_t d_mask = ~(3 << 30);
    static constexpr int64_t f_val = 1LL << 30;

    // Base64 字符表（标准顺序）
    static const std::string c_alphabet;

    // 预计算 2^i
    std::vector<int64_t> h;

    // 输入数据（base64 解码后索引）
    std::vector<int> i_data;
    int e = 0; // 字节索引
    int o = 0; // 位偏移
    int n = 0; // 总字节数

    // 状态结构体
    struct State {
        int d = 0;   // day offset
        int p = 0;   // precision
        int ld = 0;  // last delta?
        int64_t cd = 0; // close delta
        int c = 0;   // control flag
        int m = 0;   // pow(10, p)

        // For _()
        int la = 0, lp = 0, lv = 0, tv = 0, rv = 0, zv = 0, pp = 0;
        int64_t pc = 0, cp = 0;
        int64_t da = 0, sa = 0, sv = 0;

        // For T()
        int md = 0, mv = 0;
        std::vector<int64_t> cv = {0, 0};

        // For k()
        int lk = 0; // 对应 JS 中的 r.l （我重命名为 lk 避免冲突）

        // For _mi_run
        int f = 0;
        std::vector<int> dv;
        std::vector<int> dl;
    } r;

    int s = 0;           // mode from header
    int u_header = 0;    // 控制分支

public:
    struct Candle {
        std::string day;
        double open = 0.0, high = 0.0, low = 0.0, close = 0.0;
        int64_t volume = 0;
        double prevclose = 0.0;
    };

    struct Trade {
        std::string date;
        double price = 0.0, avg_price = 0.0;
        int64_t volume = 0;
        double prevclose = 0.0;
    };

    struct MultiField {
        std::vector<int> values;
    };

    struct DailyClose {
        std::string day;
        double close;
        bool has_prevclose = false;
        double prevclose = 0.0;

        // 构造函数：第一个用 with_prev
        DailyClose(const std::string& d, double c) : day(d), close(c) {}
        DailyClose with_prev(double pc) const {
            DailyClose copy(*this);
            copy.has_prevclose = true;
            copy.prevclose = pc;
            return copy;
        }
    };

private:
    char v(int x) { return static_cast<char>(x); }

    void initialize(const std::string& t) {
        h.resize(64);
        for (int i = 0; i < 64; ++i) {
            h[i] = 1LL << i;
        }

        i_data.clear();
        for (char ch : t) {
            size_t pos = c_alphabet.find(ch);
            if (pos != std::string::npos) {
                i_data.push_back(static_cast<int>(pos));
            } else {
                i_data.push_back(-1);
            }
        }
        n = static_cast<int>(i_data.size());
        e = 0;
        o = 0;
    }

    bool y() {
        if (e >= n) return false;
        bool result = (i_data[e] & (1 << o)) != 0;
        o++;
        if (o >= 6) {
            o -= 6;
            e++;
        }
        return result;
    }

    int64_t N() {
        int64_t t = y() ? 1 : 0;
        int64_t e_count = 1;
        while (y()) {
            e_count++;
        }
        return e_count * (2 * t - 1);
    }

    std::vector<int64_t> w(const std::vector<int>& t,
                           const std::vector<int>& r_flag = {},
                           const std::vector<int>& a_flag = {}) {
        std::vector<int64_t> result;
        std::vector<int> r_use = r_flag.empty() ? std::vector<int>(t.size(), 0) : r_flag;
        std::vector<int> a_use = a_flag.empty() ? std::vector<int>(t.size(), 0) : a_flag;

        for (size_t idx = 0; idx < t.size(); ++idx) {
            int len = t[idx];
            if (len == 0) {
                result.push_back(0);
                continue;
            }
            if (e >= n) break;

            int64_t val = 0;
            int bits_left = len;

            while (bits_left > 0 && e < n) {
                int take = std::min(bits_left, 6 - o);
                uint8_t mask = (1 << take) - 1;
                val |= ((i_data[e] >> o) & mask) << (len - bits_left);

                o += take;
                bits_left -= take;

                if (o >= 6) {
                    o -= 6;
                    e++;
                }
            }

            if (len <= 30 && r_use[idx] && val >= h[len - 1]) {
                val -= h[len];
            } else if (len > 30) {
                auto sub = w({30, len - 30}, {0, r_use[idx]});
                if (!a_use[idx]) {
                    val = sub[0] + sub[1] * h[30];
                } else {
                    result.push_back(sub[0]);
                    result.push_back(sub[1]);
                    continue;
                }
            }
            result.push_back(val);
        }
        return result;
    }

    std::string x(int day_offset) {
        int64_t total_days = u_base + r.d + day_offset;
        int64_t millis = total_days * l;
        time_t sec = millis / 1000;
        tm* ptm = gmtime(&sec);

        std::ostringstream oss;
        oss << (1900 + ptm->tm_year)
            << '-' << std::setfill('0') << std::setw(2) << (ptm->tm_mon + 1)
            << '-' << std::setfill('0') << std::setw(2) << ptm->tm_mday;
        return oss.str();
    }

    std::vector<DailyClose> S() {
        if (s >= 1) return {};

        r.d = w({18}, {1})[0] - 1;
        auto a = w({3, 3, 30, 6});
        r.p = a[0]; r.ld = a[1]; r.cd = a[2]; r.c = a[3];
        r.m = static_cast<int>(std::pow(10, r.p));
        double initial_pc = r.cd / static_cast<double>(r.m);  // prevclose

        std::vector<DailyClose> result;
        int index = 0;

        while (true) {
            // 解析 day offset
            if (!y()) break;

            auto cmd = w({3})[0];
            int day_offset = 1;  // default

            if (cmd == 0) {
                day_offset = w({6})[0];
            } else if (cmd == 1) {
                r.d = w({18})[0];
                day_offset = 0;
            } else {
                day_offset = cmd;
            }

            std::string date_str = x(day_offset);

            // 更新 ld
            if (y()) {
                r.ld += N();
            }

            // 更新 cd
            auto delta_vec = w({3 * r.ld}, {1});
            r.cd += delta_vec[0];
            double current_close = r.cd / static_cast<double>(r.m);

            DailyClose bar(date_str, current_close);

            // 只有第一个加 prevclose
            if (index == 0) {
                bar.has_prevclose = true;
                bar.prevclose = initial_pc;
            }

            result.push_back(bar);

            // 循环终止条件
            if (e >= n) break;
            if (e == n - 1 && ((r.c ^ (index + 1)) & 63) == 0) break;

            index++;
        }

        return result;
    }

    std::vector<Trade> _() {
        if (s > 2) return {};

        r.d = w({18}, {1})[0] - 1;
        Trade first;
        first.date = x(1);

        std::vector<int> lens = (s >= 1) ? std::vector<int>{4,4,4,1,1,1,3} : std::vector<int>{3,3,4,1,1,1,5};
        auto a = w(lens);
        r.la = a[0]; r.lp = a[1]; r.lv = a[2]; r.tv = a[3];
        r.rv = a[4]; r.zv = a[5]; r.pp = a[6];

        r.m = static_cast<int>(std::pow(10, r.pp));

        int a_len;
        if (s >= 1) {
            auto tmp = w({3, 3});
            r.c = tmp[0];
            a_len = tmp[1];
        } else {
            r.c = 2;
            a_len = 5;
        }

        r.pc = w({6 * a_len})[0];
        first.prevclose = r.pc / static_cast<double>(r.m);
        r.cp = r.pc;
        r.da = r.sa = r.sv = 0;

        std::vector<Trade> trades;
        int t_idx = 0;
        while (e < n) {
            Trade trade;
            bool has_vol_update = r.tv ? y() : true;

            int64_t op_p = 0, op_v = 0, op_a = 0;

            for (int i = 0; i < 3; ++i) {
                bool updated = has_vol_update ? y() : false;
                if (updated) {
                    int64_t delta = N();
                    if (i == 0) r.lv += delta;
                    else if (i == 1) r.lp += delta;
                    else if (i == 2) r.la += delta;
                }

                bool raw_mode = (i == 0 && r.rv) ? y() : true;
                int bit_len = 3 * (i == 0 ? r.lv : (i == 1 ? r.lp : r.la));
                if (i == 0 && !raw_mode) bit_len += 7;

                auto val_vec = w({bit_len}, {i != 0});
                int64_t raw_val = val_vec[0];
                int64_t final_val = raw_mode ? raw_val : raw_val * 100;

                if (i == 0) op_v = final_val;
                else if (i == 1) op_p = final_val;
                else if (i == 2) op_a = final_val;

                if (i == 0 && op_v == 0 && (s <= 1 || t_idx >= 241) && (r.zv ? !y() : true)) {
                    op_p = 0;
                    break;
                }
            }

            if (op_p == 0) continue;

            r.sv += op_v;
            r.cp += op_p;
            r.sa += op_v * r.cp;

            trade.price = r.cp / static_cast<double>(r.m);
            trade.volume = op_v;
            if (op_a == 0) {
                trade.avg_price = trades.empty() ? trade.price : trades.back().avg_price;
            } else {
                if (r.sv == 0) {
                    trade.avg_price = trade.price + r.da / 1000.0;
                } else {
                    int64_t temp = (r.sa * 2000LL / r.m + r.sv);
                    trade.avg_price = ((temp / r.sv) / 2 + r.da) / 1000.0;
                }
            }

            trades.push_back(trade);
            t_idx++;

            if (e >= n) break;
            if (e == n - 1 && ((r.c ^ t_idx) & 7) == 0) break;
        }

        if (!trades.empty()) {
            trades[0].date = first.date;
            trades[0].prevclose = first.prevclose;
        }
        return trades;
    }

    std::vector<Candle> T() {
        if (s >= 1) return {};
        r.lv = r.ld = r.cd = 0;
        r.cv = {0, 0};
        r.p = w({6})[0];
        r.d = w({18}, {1})[0] - 1;
        r.m = static_cast<int>(std::pow(10, r.p));
        auto md_mv = w({3, 3});
        r.md = md_mv[0]; r.mv = md_mv[1];

        const std::vector<int> p_table = {0,3,5,6,9,10,12,15,17,18,20,23,24,27,29,30};

        std::vector<Candle> candles;
        while (true) {
            auto a = w({6});
            if (a.empty()) break;
            int cmd = a[0];

            Candle bar;
            int x_d = r.ld, x_v = r.lv, u_d = r.ld, u_v = r.lv;
            (void)u_d;
            (void)u_v;
            if (cmd & 32) {
                while (true) {
                    a = w({6});
                    if (a.empty()) break;
                    int sub = a[0];
                    if ((sub & 63) == (16 | 63)) {
                        bool is_x = !(sub & 16);
                        auto vals = w({3,3});
                        if (is_x) { x_d = vals[0] + r.md; x_v = vals[1] + r.mv; }
                        else { r.md = vals[0] + r.md; r.mv = vals[1] + r.mv; }
                        break;
                    } else if (sub & 32) {
                        bool is_d = !(sub & 8), is_x = !(sub & 16);
                        int val = (sub & 7);
                        if (is_x) { if (is_d) x_d = val + r.md; else x_v = val + r.mv; }
                        else { if (is_d) r.md = val + r.md; else r.mv = val + r.mv; }
                        break;
                    } else {
                        int oc = sub & 15;
                        if (oc == 0) bar.day = x(w({6})[0]);
                        else if (oc == 1) { r.d = w({18})[0]; bar.day = x(0); }
                        else bar.day = x(oc);
                        if (!(sub & 16)) break;
                    }
                }
            } else {
                bar.day = x(cmd & 63);
            }

            std::vector<int> lens = {x_d, x_d, x_d, x_d, x_v};
            int p_idx = p_table[cmd & 15];
            if (x_v & 1) p_idx = 31 - p_idx;
            if (cmd & 16) lens[4] += 2;
            for (int i = 0; i < 5; ++i) {
                if (p_idx & (1 << (4 - i))) lens[i]++;
                lens[i] *= 3;
            }

            auto diffs = w(lens, {1,0,0,1,1}, {0,0,0,0,1});
            int64_t base = r.cd + diffs[0];
            bar.open = base / static_cast<double>(r.m);
            bar.high = (base + diffs[1]) / static_cast<double>(r.m);
            bar.low = (base - diffs[2]) / static_cast<double>(r.m);
            bar.close = (base + diffs[3]) / static_cast<double>(r.m);

            int64_t vol_raw = diffs[4];
            std::vector<int64_t> vol_pair = {vol_raw >= 0 ? vol_raw : (vol_raw & (f_val - 1)), vol_raw >= 0 ? 0 : -1};

            r.cd = base + diffs[3];
            int64_t new_low = r.cv[0] + vol_pair[0];
            int64_t carry = r.cv[1] + vol_pair[1];
            if (((r.cv[0] & d_mask) + (vol_pair[0] & d_mask)) & f_val) carry++;
            r.cv[0] = new_low & (f_val - 1);
            r.cv[1] = carry;
            bar.volume = r.cv[0] + r.cv[1] * f_val;

            candles.push_back(bar);
        }
        return candles;
    }

    std::vector<std::string> k() {
        if (s > 1) return {};

        r.lk = 0;
        int count = -1;
        r.d = w({18})[0] - 1;
        int end_day = w({18})[0];

        std::vector<std::string> result;
        std::string first_day = x(1);

        while (r.d < end_day) {
            std::string today = x(1);
            if (count <= 0) {
                if (y()) r.lk += N();
                auto tmp = w(std::vector<int>{3 * r.lk}, {0});
                count = tmp[0] + 1;

                if (result.empty()) {
                    result.push_back(first_day);
                    count--;
                } else {
                    result.push_back(today);
                }
            } else {
                result.push_back(today);
            }
            count--;
            r.d++;
        }

        // 如果没推入任何数据，至少返回第一个
        if (result.empty()) {
            result.push_back(first_day);
        }

        return result;
    }

    std::vector<MultiField> _mi_run() {
        if (s >= 1) return {};
        r.f = w({6})[0];
        r.c = w({6})[0];
        r.dv.assign(r.f, 0);
        r.dl.assign(r.f, 0);
        std::vector<MultiField> res;
        int idx = 0;
        while (e < n) {
            MultiField mf;
            mf.values.resize(r.f);
            for (int i = 0; i < r.f; ++i) {
                if (y()) r.dl[i] += N();
                auto val = w({3 * r.dl[i]}, {1});
                r.dv[i] += val[0];
                mf.values[i] = r.dv[i];
            }
            res.push_back(mf);
            idx++;
            if (e >= n) break;
            if (e == n - 1 && ((r.c ^ idx) & 7) == 0) break;
        }
        return res;
    }

public:
    template<typename T>
    std::vector<T> decode(const std::string& input) {
        initialize(input);
        auto u = w({12, 6});
        u_header = u[0];
        s = 63 ^ u[1];

        std::cout << "u[0]=" << u_header << " 分支: ";
        std::string name;
        switch (u_header) {
            case 1479: name = "T"; break;
            case 136:  name = "_"; break;
            case 200:  name = "S"; break;
            case 139:  name = "k"; break;
            case 197:  name = "_mi_run"; break;
            default:   name = "unknown";
        }
        std::cout << name << "\n";

        if constexpr (std::is_same_v<T, std::string>) {
            if (u_header == 139) return k();  // k() → vector<string>
        } else if constexpr (std::is_same_v<T, DailyClose>) {
            if (u_header == 200) return S();  // S() → vector<DailyClose>
        } else if constexpr (std::is_same_v<T, Trade>) {
            if (u_header == 136) return _();
        } else if constexpr (std::is_same_v<T, Candle>) {
            if (u_header == 1479) return T();
        } else if constexpr (std::is_same_v<T, MultiField>) {
            if (u_header == 197) return _mi_run();
        }
        return {};
        return {};
    }
};

// Static definition
const std::string Decoder::c_alphabet =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// 测试主函数
TEST_CASE("c1", "[calendar]") {
    std::string encoded_data = "LC/AAAf8CXCw6mHbaPgkryxXv10eAJP1LW0SD39aT7+NV44Xba3PxCgTdrFc3FepphjnTBw1X4hmGu+ypVAcvFenpBXPqCc6F4ZmGueLFwbIN8QTDXPsCc1FepphjvOoCc8FepphjvcgFO3CP00wxXXWhrkUdZrIJpw9X3ThrlEp6hlGc88Kcem0VeFpZM46VV4MrTC2KScKc811U4aLXUdlzINc9lTrwFW3T52KPj0mDueVFuUR1RtiEoCXfdgFOOSGRXnUhrXWhb0kt6Rk2pU44JV4SrTyU9wSDHPwCnXdP1FuiUM44r7qwdKqcYrIZpw1DqgrlU5IrHRawxjrwBaqcbrIt9gr3UhDtOpyVNjEnCHPnC3royNWvi0gj/";

    Decoder dec;

    // Try decoding as SimpleDate (likely S function)
    auto dates = dec.decode<std::string>(encoded_data);
    std::cout << "\n--- Decoded Dates ---\n";
    for (const auto& d : dates) {
        std::cout << d << "\n";
    }

}
