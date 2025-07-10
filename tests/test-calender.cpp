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
//#include <stdio.h>
//#include <time.h>
//#include <duktape.h>
//#include <q1x/exchange.h>
//
//// 可靠判断 Date 对象的方法
//int is_date_object(duk_context *ctx, duk_idx_t idx) {
//    // 调用 Object.prototype.toString 方法
//    duk_get_global_string(ctx, "Object");
//    duk_get_prop_string(ctx, -1, "prototype");
//    duk_get_prop_string(ctx, -1, "toString");
//    duk_dup(ctx, idx < 0 ? idx-3 : idx);  // 复制待检测对象
//
//    if (duk_pcall(ctx, 1) != 0) {         // 执行 toString.call(obj)
//        printf("类型检测失败: %s\n", duk_safe_to_string(ctx, -1));
//        duk_pop(ctx); // 弹出错误
//        return 0;
//    }
//
//    const char *str = duk_get_string(ctx, -1);
//    int ret = (strcmp(str, "[object Date]") == 0);
//
//    duk_pop_n(ctx, 3); // 弹出 Object/prototype/toString 结果
//    return ret;
//}
//
//// 安全获取时间戳（兼容不同引擎）
//double get_date_timestamp(duk_context *ctx, duk_idx_t idx) {
//    duk_get_prop_string(ctx, idx, "getTime"); // 1. 获取方法
//    duk_dup(ctx, idx);                        // 2. 设置 this 绑定
//
//    if (duk_pcall(ctx, 1) != 0) {             // 3. 调用 obj.getTime()
//        printf("调用失败: %s\n", duk_safe_to_string(ctx, -1));
//        duk_pop(ctx);
//        return -1;
//    }
//
//    double timestamp = duk_get_number(ctx, -1);
//    duk_pop(ctx); // 弹出结果
//    return timestamp;
//}
//
//int main_0() {
//    duk_context *ctx = duk_create_heap_default();
//
//    // 创建测试数组（包含 Date 对象和干扰项）
//    duk_eval_string(ctx,
//                    "[ new Date('1990-12-19T00:00:00Z'), "
//                    "{ fakeDate: true }, "
//                    "new Date('1990-12-20T00:00:00Z') ]"
//    );
//
//    duk_size_t len = duk_get_length(ctx, -1);
//    for (duk_uarridx_t i = 0; i < len; i++) {
//        if (duk_get_prop_index(ctx, -1, i)) { // 获取元素
//            if (is_date_object(ctx, -1)) {
//                double ts = get_date_timestamp(ctx, -1);
//                if (ts >= 0) {
//                    time_t t = (time_t)(ts / 1000);
//                    struct tm *tm = gmtime(&t);
//
//                    printf("[%d] 合法日期对象\n", i);
//                    printf("  时间戳: %.0f ms\n", ts);
//                    printf("  格式化: %04d-%02d-%02d\n",
//                           tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday);
//                }
//            } else {
//                printf("[%d] 非日期对象: ", i);
//                if (duk_is_object(ctx, -1)) {
//                    printf("普通对象\n");
//                } else {
//                    printf("%s\n", duk_safe_to_string(ctx, -1));
//                }
//            }
//            duk_pop(ctx); // 弹出元素
//        }
//    }
//
//    duk_destroy_heap(ctx);
//    return 0;
//}
//
//#include <stdio.h>
//#include <duktape.h>
//
//// 错误处理函数
//void handle_error(duk_context *ctx, int rc) {
//    if (rc) {
//        printf("Error: %s\n", duk_safe_to_string(ctx, -1));
//        duk_pop(ctx); // 弹出错误信息
//    }
//}
//
//int main_1() {
//    // 创建 Duktape 上下文
//    duk_context *ctx = duk_create_heap_default();
//    if (!ctx) {
//        printf("Failed to create Duktape context\n");
//        return 1;
//    }
//
//    // 定义返回数组的 JavaScript 函数
//    const char *js_code =
//            "function getSampleArray() {\n"
//            "    return [42, 'Hello', true, {x: 10}];\n"
//            "}";
//
//    // 将 JavaScript 代码推入栈并执行
//    if (duk_peval_string(ctx, js_code) != 0) {
//        handle_error(ctx, 1);
//        duk_destroy_heap(ctx);
//        return 1;
//    }
//    duk_pop(ctx); // 弹出 eval 结果（undefined）
//
//    // 调用 JavaScript 函数
//    duk_get_global_string(ctx, "getSampleArray");  // 将函数推入栈顶
//    if (duk_pcall(ctx, 0) != 0) {                  // 调用函数（0个参数）
//        handle_error(ctx, 1);
//        duk_destroy_heap(ctx);
//        return 1;
//    }
//
//    // 检查返回值是否为数组
//    if (!duk_is_array(ctx, -1)) {
//        printf("Return value is not an array\n");
//        duk_pop(ctx);
//        duk_destroy_heap(ctx);
//        return 1;
//    }
//
//    // 获取数组长度
//    duk_size_t len = duk_get_length(ctx, -1);
//    printf("Array length: %d\n", (int)len);
//
//    // 遍历数组元素
//    for (duk_uarridx_t i = 0; i < len; i++) {
//        duk_get_prop_index(ctx, -1, i);  // 将元素推入栈顶
//
//        printf("Element %d: ", (int)i);
//
//        if (duk_is_number(ctx, -1)) {
//            printf("Number: %g\n", duk_get_number(ctx, -1));
//        } else if (duk_is_string(ctx, -1)) {
//            printf("String: %s\n", duk_get_string(ctx, -1));
//        } else if (duk_is_boolean(ctx, -1)) {
//            printf("Boolean: %s\n", duk_get_boolean(ctx, -1) ? "true" : "false");
//        } else if (duk_is_object(ctx, -1)) {
//            printf("Object: ");
//            duk_get_prop_string(ctx, -1, "x");
//            printf("{x: %d}\n", duk_get_int(ctx, -1));
//            duk_pop(ctx); // 弹出 x 的值
//        }
//
//        duk_pop(ctx); // 弹出当前元素
//    }
//
//    // 清理栈（弹出数组）
//    duk_pop(ctx);
//
//    // 销毁上下文
//    duk_destroy_heap(ctx);
//    return 0;
//}
//
//#include <duktape.h>
//#include <string>
//#include <algorithm>
//#include <vector>
//#include <iostream>
//
//const char * const sinaJsDecoder = R"(function d(t) {
//    var e, i, n, r, a, o, s, l = (arguments,
//            864e5), u = 7657, c = [], h = [], d = ~(3 << 30), f = 1 << 30,
//        p = [0, 3, 5, 6, 9, 10, 12, 15, 17, 18, 20, 23, 24, 27, 29, 30], m = Math, g = function () {
//            var l, u;
//            for (l = 0; 64 > l; l++)
//                h[l] = m.pow(2, l),
//                26 > l && (c[l] = v(l + 65),
//                    c[l + 26] = v(l + 97),
//                10 > l && (c[l + 52] = v(l + 48)));
//            for (c.push("+", "/"),
//                     c = c.join(""),
//                     i = t.split(""),
//                     n = i.length,
//                     l = 0; n > l; l++)
//                i[l] = c.indexOf(i[l]);
//            return r = {},
//                e = o = 0,
//                a = {},
//                u = w([12, 6]),
//                s = 63 ^ u[1],
//            {
//                _1479: T,
//                _136: _,
//                _200: S,
//                _139: k,
//                _197: _mi_run
//            }["_" + u[0]] || function () {
//                return []
//            }
//        }, v = String.fromCharCode, b = function (t) {
//            return t === {}._
//        }, N = function () {
//            var t, e;
//            for (t = y(),
//                     e = 1; ;) {
//                if (!y())
//                    return e * (2 * t - 1);
//                e++
//            }
//        }, y = function () {
//            var t;
//            return e >= n ? 0 : (t = i[e] & 1 << o,
//                o++,
//            o >= 6 && (o -= 6,
//                e++),
//                !!t)
//        }, w = function (t, r, a) {
//            var s, l, u, c, d;
//            for (l = [],
//                     u = 0,
//                 r || (r = []),
//                 a || (a = []),
//                     s = 0; s < t.length; s++)
//                if (c = t[s],
//                    u = 0,
//                    c) {
//                    if (e >= n)
//                        return l;
//                    if (t[s] <= 0)
//                        u = 0;
//                    else if (t[s] <= 30) {
//                        for (; d = 6 - o,
//                                   d = c > d ? d : c,
//                                   u |= (i[e] >> o & (1 << d) - 1) << t[s] - c,
//                                   o += d,
//                               o >= 6 && (o -= 6,
//                                   e++),
//                                   c -= d,
//                                   !(0 >= c);)
//                            ;
//                        r[s] && u >= h[t[s] - 1] && (u -= h[t[s]])
//                    } else
//                        u = w([30, t[s] - 30], [0, r[s]]),
//                        a[s] || (u = u[0] + u[1] * h[30]);
//                    l[s] = u
//                } else
//                    l[s] = 0;
//            return l
//        }, x = function (t) {
//            var e, i, n;
//            for (t > 1 && (e = 0),
//                     e = 0; t > e; e++)
//                r.d++,
//                    n = r.d % 7,
//                (3 == n || 4 == n) && (r.d += 5 - n);
//            return i = new Date,
//                i.setTime((u + r.d) * l),
//                i
//        }, S = function () {
//            var t, i, a, o, l;
//            if (s >= 1)
//                return [];
//            for (r.d = w([18], [1])[0] - 1,
//                     a = w([3, 3, 30, 6]),
//                     r.p = a[0],
//                     r.ld = a[1],
//                     r.cd = a[2],
//                     r.c = a[3],
//                     r.m = m.pow(10, r.p),
//                     r.pc = r.cd / r.m,
//                     i = [],
//                     t = 0; o = {
//                d: 1
//            },
//                 y() && (a = w([3])[0],
//                     0 == a ? o.d = w([6])[0] : 1 == a ? (r.d = w([18])[0],
//                         o.d = 0) : o.d = a),
//                     l = {
//                         day: x(o.d)
//                     },
//                 y() && (r.ld += N()),
//                     a = w([3 * r.ld], [1]),
//                     r.cd += a[0],
//                     l.close = r.cd / r.m,
//                     i.push(l),
//                 !(e >= n) && (e != n - 1 || 63 & (r.c ^ t + 1)); t++)
//                ;
//            return i[0].prevclose = r.pc,
//                i
//        }, _ = function () {
//            var t, i, a, o, l, u, c, h, d, f, p;
//            if (s > 2)
//                return [];
//            for (c = [],
//                     d = {
//                         v: "volume",
//                         p: "price",
//                         a: "avg_price"
//                     },
//                     r.d = w([18], [1])[0] - 1,
//                     h = {
//                         day: x(1)
//                     },
//                     a = w(1 > s ? [3, 3, 4, 1, 1, 1, 5] : [4, 4, 4, 1, 1, 1, 3]),
//                     t = 0; 7 > t; t++)
//                r[["la", "lp", "lv", "tv", "rv", "zv", "pp"][t]] = a[t];
//            for (r.m = m.pow(10, r.pp),
//                     s >= 1 ? (a = w([3, 3]),
//                         r.c = a[0],
//                         a = a[1]) : (a = 5,
//                         r.c = 2),
//                     r.pc = w([6 * a])[0],
//                     h.pc = r.pc / r.m,
//                     r.cp = r.pc,
//                     r.da = 0,
//                     r.sa = r.sv = 0,
//                     t = 0; !(e >= n) && (e != n - 1 || 7 & (r.c ^ t)); t++) {
//                for (l = {},
//                         o = {},
//                         f = r.tv ? y() : 1,
//                         i = 0; 3 > i; i++)
//                    if (p = ["v", "p", "a"][i],
//                    (f ? y() : 0) && (a = N(),
//                        r["l" + p] += a),
//                        u = "v" == p && r.rv ? y() : 1,
//                        a = w([3 * r["l" + p] + ("v" == p ? 7 * u : 0)], [!!i])[0] * (u ? 1 : 100),
//                        o[p] = a,
//                    "v" == p) {
//                        if (!(l[d[p]] = a) && (s > 1 || 241 > t) && (r.zv ? !y() : 1)) {
//                            o.p = 0;
//                            break
//                        }
//                    } else
//                        "a" == p && (r.da = (1 > s ? 0 : r.da) + o.a);
//                r.sv += o.v,
//                    l[d.p] = (r.cp += o.p) / r.m,
//                    r.sa += o.v * r.cp,
//                    l[d.a] = b(o.a) ? t ? c[t - 1][d.a] : l[d.p] : r.sv ? ((m.floor((r.sa * (2e3 / r.m) + r.sv) / r.sv) >> 1) + r.da) / 1e3 : l[d.p] + r.da / 1e3,
//                    c.push(l)
//            }
//            return c[0].date = h.day,
//                c[0].prevclose = h.pc,
//                c
//        }, T = function () {
//            var t, e, i, n, a, o, l;
//            if (s >= 1)
//                return [];
//            for (r.lv = 0,
//                     r.ld = 0,
//                     r.cd = 0,
//                     r.cv = [0, 0],
//                     r.p = w([6])[0],
//                     r.d = w([18], [1])[0] - 1,
//                     r.m = m.pow(10, r.p),
//                     a = w([3, 3]),
//                     r.md = a[0],
//                     r.mv = a[1],
//                     t = []; a = w([6]),
//                     a.length;) {
//                if (i = {
//                    c: a[0]
//                },
//                    n = {},
//                    i.d = 1,
//                32 & i.c)
//                    for (; ;) {
//                        if (a = w([6])[0],
//                        63 == (16 | a)) {
//                            l = 16 & a ? "x" : "u",
//                                a = w([3, 3]),
//                                i[l + "_d"] = a[0] + r.md,
//                                i[l + "_v"] = a[1] + r.mv;
//                            break
//                        }
//                        if (32 & a) {
//                            o = 8 & a ? "d" : "v",
//                                l = 16 & a ? "x" : "u",
//                                i[l + "_" + o] = (7 & a) + r["m" + o];
//                            break
//                        }
//                        if (o = 15 & a,
//                            0 == o ? i.d = w([6])[0] : 1 == o ? (r.d = o = w([18])[0],
//                                i.d = 0) : i.d = o,
//                            !(16 & a))
//                            break
//                    }
//                n.date = x(i.d);
//                for (o in {
//                    v: 0,
//                    d: 0
//                })
//                    b(i["x_" + o]) || (r["l" + o] = i["x_" + o]),
//                    b(i["u_" + o]) && (i["u_" + o] = r["l" + o]);
//                for (i.l_l = [i.u_d, i.u_d, i.u_d, i.u_d, i.u_v],
//                         l = p[15 & i.c],
//                     1 & i.u_v && (l = 31 - l),
//                     16 & i.c && (i.l_l[4] += 2),
//                         e = 0; 5 > e; e++)
//                    l & 1 << 4 - e && i.l_l[e]++,
//                        i.l_l[e] *= 3;
//                i.d_v = w(i.l_l, [1, 0, 0, 1, 1], [0, 0, 0, 0, 1]),
//                    o = r.cd + i.d_v[0],
//                    n.open = o / r.m,
//                    n.high = (o + i.d_v[1]) / r.m,
//                    n.low = (o - i.d_v[2]) / r.m,
//                    n.close = (o + i.d_v[3]) / r.m,
//                    a = i.d_v[4],
//                "number" == typeof a && (a = [a, a >= 0 ? 0 : -1]),
//                    r.cd = o + i.d_v[3],
//                    l = r.cv[0] + a[0],
//                    r.cv = [l & d, r.cv[1] + a[1] + !!((r.cv[0] & d) + (a[0] & d) & f)],
//                    n.volume = (r.cv[0] & f - 1) + r.cv[1] * f,
//                    t.push(n)
//            }
//            return t
//        }, k = function () {
//            var t, e, i, n;
//            if (s > 1)
//                return [];
//            for (r.l = 0,
//                     n = -1,
//                     r.d = w([18])[0] - 1,
//                     i = w([18])[0]; r.d < i;)
//                e = x(1),
//                    0 >= n ? (y() && (r.l += N()),
//                        n = w([3 * r.l], [0])[0] + 1,
//                    t || (t = [e],
//                        n--)) : t.push(e),
//                    n--;
//            return t
//        };
//    return _mi_run = function () {
//        var t, i, a, o;
//        if (s >= 1)
//            return [];
//        for (r.f = w([6])[0],
//                 r.c = w([6])[0],
//                 a = [],
//                 r.dv = [],
//                 r.dl = [],
//                 t = 0; t < r.f; t++)
//            r.dv[t] = 0,
//                r.dl[t] = 0;
//        for (t = 0; !(e >= n) && (e != n - 1 || 7 & (r.c ^ t)); t++) {
//            for (o = [],
//                     i = 0; i < r.f; i++)
//                y() && (r.dl[i] += N()),
//                    r.dv[i] += w([3 * r.dl[i]], [1])[0],
//                    o[i] = r.dv[i];
//            a.push(o)
//        }
//        return a
//    }
//        ,
//        g()()
//})";
//
//std::string preprocess(const std::string& text) {
//    std::string processed = text;
//    size_t eqPos = processed.find('=');
//    if (eqPos != std::string::npos) {
//        processed = processed.substr(eqPos + 1);
//    }
//    size_t semiPos = processed.find(';');
//    if (semiPos != std::string::npos) {
//        processed = processed.substr(0, semiPos);
//    }
//    processed.erase(std::remove(processed.begin(), processed.end(), '"'), processed.end());
//    return processed;
//}
//
//
//int main() {
//    std::string text = exchange::update_calendar();
//
//    duk_context *ctx = duk_create_heap_default();
//
//    // 定义JS函数
//    //duk_eval_string(ctx, sinaJsDecoder);
//    if (duk_peval_string(ctx,sinaJsDecoder) != DUK_EXEC_SUCCESS) {
//        std::cerr << "Error in JavaScript code: " << duk_safe_to_string(ctx, -1) << std::endl;
//        duk_destroy_heap(ctx);
//        return 1;
//    }
//
//    // 调用函数
//    auto a1 = duk_get_global_string(ctx, "d");
//    std::cout << a1 << std::endl;
//    std::string input = R"(var datelist="LC/AAAf8CXCw6mHbaPgkryxXv10eAJP1LW0SD39aT7+NV44Xba3PxCgTdrp5BkYVAc11hWvg0c/19UAc7jNtHQyWBAu2xmGuZI1NVAc3FepphjnTBw1X4hmGu+ypVAcvFenpBXPqCc6F4ZmGueLFwbIN8QTDXPsCc1FepphjvOoCc8FepphjvcgFO3CP00wxXXWhrkUdZrIJpw9X3ThrlEp6hlGc88Kcem0VeFpZM46VV4MrTC2KScKc811U4aLXUdlzINc9lTrwFW3T52KPj0mDueVFuUR1RtiEoCXfdgFOOSGRXnUhrXWhb0kt6Rk2pU44JV4SrTyU9wSDHPwCnXdP1FuiUM44r7qwdKqcYrIZpw1DqgrlU5IrHRawxjrwBaqcbrIt9gr3UhDtOpyVNjEnCHPnC3royNWvi0gj/";var KLC_TD_SH=datelist;)";
//    input = preprocess(text);
//    auto a2 = duk_push_string(ctx, input.c_str());
//    std::cout << a2 << std::endl;
//    //duk_call(ctx, 1);
//    if (duk_pcall(ctx, 1) != DUK_EXEC_SUCCESS) {
//        std::cerr << "Error: " << duk_safe_to_string(ctx, -1) << std::endl;
//        duk_destroy_heap(ctx);
//        return 1;
//    }
//
//    // 打印返回值的类型和内容
//    //std::cout << "Return value type: " << duk_get_type(ctx, -1) << std::endl;
//    //std::cout << "Return value: " << duk_safe_to_string(ctx, -1) << std::endl;
//
//    // 检查返回值是否为数组
//    if (!duk_is_array(ctx, -1)) {
//        std::cerr << "Return value is not an array!" << std::endl;
//        duk_destroy_heap(ctx);
//        return 1;
//    }
//
//    // 获取数组长度
//    auto a3 = duk_get_length(ctx, -1);
//    std::cout << a3 << std::endl;
//    //duk_size_t len = duk_get_uint(ctx, -1);
//    //std::cout << len << std::endl;
//    //duk_pop(ctx);  // 弹出长度值
//    size_t len = a3;
//    std::cout << "Array length: " << len << std::endl;
//
//    // 遍历数组元素
//    for (duk_uarridx_t i = 0; i < len; i++) {
//        duk_get_prop_index(ctx, -1, i);
//        std::cout << "Element(" << duk_get_type(ctx, -1) <<")" << i << ": ";
//        if (duk_is_number(ctx, -1)) {
//            std::cout << "number: " << duk_get_number(ctx, -1);
//        } else if (duk_is_string(ctx, -1)) {
//            std::cout << "string: " << duk_get_string(ctx, -1);
//        } else if (duk_is_boolean(ctx, -1)) {
//            std::cout << "boolean: " << duk_get_boolean(ctx, -1);
//        } else if (duk_is_object(ctx, -1)) {
//            const char *date_str = duk_safe_to_string(ctx, -1);
//            std::cout << "object: " << date_str << std::endl;
//
////            auto pb = duk_get_prop_string(ctx, -1, "getTime");
////            std::cout << "get method: " << pb << std::endl;
////            printf("{getTime: %d}\n", duk_get_int(ctx, -1));
////            duk_pop(ctx); // 弹出 x 的值
//
//
//            //auto date_num = duk_get_number(ctx, -1);
//            //std::cout << "object: " << date_num;
////            duk_get_prop_string(ctx, -1, "toISOString");
////            if (duk_is_callable(ctx, -1)) {
////                duk_dup(ctx, -1);
////                if (duk_pcall_method(ctx, 0) == 0) {
////                    double ts = duk_get_number(ctx, -1);
////                    std::cout << "object: " << ts;
////                    duk_pop(ctx); // 弹出时间戳
////                } else {
////                    std::cerr << "Error: " << duk_safe_to_string(ctx, -1) << std::endl;
////                }
////            }
////            duk_pop(ctx); // 弹出getTime
//        } else {
//            std::cout << "[unknown type]";
//        }
//        std::cout << std::endl;
//
//        duk_pop(ctx);  // 弹出当前元素
//    }
//
//    duk_destroy_heap(ctx);
//    return 0;
//}
//
////#include <iostream>
////#include <string>
////#include <cstdlib>
////#include "mujs.h"
////
////using namespace std;
////
////
////int main() {
////    // 初始化 mujs 环境
////    js_State *J = js_newstate(nullptr, nullptr, JS_STRICT);
////
////    // 加载 JavaScript 文件
////    string jsFilePath = "D:/projects/quant1x/q2x/cmake-build-debug/js_decode.js";
////    //jsFilePath = "js_decode.js";
////    cerr << "Loading JavaScript file: " << jsFilePath << endl;
////    if (js_dofile(J, jsFilePath.c_str())) {
////        cerr << "Error loading JavaScript file: " << jsFilePath << endl;
////        js_freestate(J);
////        return 1;
////    }
////    cerr << "JavaScript file loaded successfully." << endl;
//////    js_getglobal(J, "d");
//////    int num = js_tonumber(J, -1);
//////    std::cout << num << std::endl;
//////    js_pop(J, 1);
////    // 检查函数 d 是否存在
////    js_getglobal(J, "d");
////    if (js_type(J, -1) != JS_ISFUNCTION) {
////        cerr << "Error: 'd' is not a function." << endl;
////        js_pop(J, 1);
////        js_freestate(J);
////        return 1;
////    }
////    js_pop(J, 1);
////
////    // 输入编码内容
////    string input = "LC/AAAf8CXCw6mHbaPgkryxXv10eAJP1LW0SD39aT7+NV44Xba3PxCgTdrFc3FepphjnTBw1X4hmGu+ypVAcvFenpBXPqCc6F4ZmGueLFwbIN8QTDXPsCc1FepphjvOoCc8FepphjvcgFO3CP00wxXXWhrkUdZrIJpw9X3ThrlEp6hlGc88Kcem0VeFpZM46VV4MrTC2KScKc811U4aLXUdlzINc9lTrwFW3T52KPj0mDueVFuUR1RtiEoCXfdgFOOSGRXnUhrXWhb0kt6Rk2pU44JV4SrTyU9wSDHPwCnXdP1FuiUM44r7qwdKqcYrIZpw1DqgrlU5IrHRawxjrwBaqcbrIt9gr3UhDtOpyVNjEnCHPnC3royNWvi0gj/";
////
////    // 设置输入参数
////    js_pushundefined(J); // this 对象
////    js_pushstring(J, input.c_str());
////
////    // 调用函数 d
////    js_getglobal(J, "d");
////    if (js_pcall(J, 1)) { // 调用函数，传入 1 个参数
////        cerr << "Error calling function 'd': " << js_trystring(J, -1, "unknown error") << endl;
////        js_pop(J, 1);
////        js_freestate(J);
////        return 1;
////    }
////
////    // 获取返回值
////    if (js_isarray(J, -1)) {
////        int length = js_getlength(J, -1);
////        cout << "Decoded Results:" << endl;
////        for (int i = 0; i < length; ++i) {
////            js_getindex(J, -1, i);
////            cout << js_tostring(J, -1) << endl;
////            js_pop(J, 1);
////        }
////    } else {
////        cerr << "Unexpected return type from 'd' function." << endl;
////    }
////
////    // 清理 mujs 环境
////    js_freestate(J);
////    return 0;
////}

TEST_CASE("date-range-1", "[calendar]") {
    exchange::timestamp begin("2025-05-29");
    exchange::timestamp end("2025-05-30");
    auto list = exchange::date_range(begin, end);
    std::cout<< list << std::endl;
}

TEST_CASE("date-range-2", "[calendar]") {
    exchange::timestamp begin("2025-05-25");
    exchange::timestamp end("2025-05-31");
    auto list = exchange::date_range(begin, end);
    std::cout<< list << std::endl;
}