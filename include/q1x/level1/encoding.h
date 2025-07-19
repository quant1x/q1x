#pragma once
#ifndef QUANT1X_LEVEL1_ENCODING_H
#define QUANT1X_LEVEL1_ENCODING_H 1

#include <q1x/std/feature_detection.h>
// 手动处理字节序检测
#if OS_IS_WINDOWS
#define CISTA_LITTLE_ENDIAN 1
#else
#if OS_IS_APPLE
#include <machine/endian.h>
#else
#include <endian.h>
#endif
#define IS_LITTLE_ENDIAN (__BYTE_ORDER == __LITTLE_ENDIAN)
#endif

#include <cista/serialization.h>
#include <q1x/std/api.h>
#include <zlib.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

namespace level1 {

    /// 编解码
    namespace encoding {

        // 获取日期和时间
        // @param category 数据类别
        // @param zipday 压缩的日期
        // @param tminutes 压缩的时间（分钟）
        // @return 返回一个包含年、月、日、小时和分钟的元组
        inline std::tuple<int, int, int, int, int> GetDatetimeFromUint32(int category, uint32_t zipday, uint16_t tminutes) {
            int year = 0, month = 0, day = 0, hour = 15, minute = 0;

            if (category < 4 || category == 7 || category == 8) {
                year = static_cast<int>((zipday >> 11) + 2004);
                month = static_cast<int>((zipday % 2048) / 100);
                day = static_cast<int>((zipday % 2048) % 100);
                hour = static_cast<int>(tminutes / 60);
                minute = static_cast<int>(tminutes % 60);
            } else {
                year = static_cast<int>(zipday / 10000);
                month = static_cast<int>((zipday % 10000) / 100);
                day = static_cast<int>(zipday % 100);
            }

            return {year, month, day, hour, minute};
        }

        // 编码变长整数（VLQ编码），采用类似UTF-8的方案存储有符号整数
        // 编码规则：
        // - 每个字节的低6位（0x3F）存储数据
        // - 最高位（0x80）为连续标志：1=还有后续字节，0=终止
        // - 首字节的0x40位（第六位）为符号位：1=负数，0=正数
        // 位掩码说明：
        // +---------+-----------+-----------------------+
        // | 掩码值  | 二进制    | 作用                  |
        // +---------+-----------+-----------------------+
        // | 0x80    | 10000000  | 连续标志位            |
        // | 0x7F    | 01111111  | 首字节数据掩码（7位） |
        // | 0x40    | 01000000  | 符号位                |
        // | 0x3F    | 00111111  | 后续字节数据掩码（6位）|
        // +---------+-----------+-----------------------+
        // 编码流程：
        // 1. 处理符号位：
        //    - 如果是负数，设置符号位（0x40）
        //    - 取绝对值
        // 2. 处理第一个6位块：
        //    - 取低6位（&0x3F）作为首字节数据
        //    - 检查是否有后续字节（绝对值是否大于63）
        // 3. 处理后续7位块：
        //    - 循环读取剩余的7位块
        //    - 每个字节的低7位（&0x7F）存储数据
        //    - 最高位（0x80）为连续标志：1=还有后续字节，0=终止
        // 4. 返回编码后的字节数
        //  @param value 输入的有符号整数
        //  @param buffer 输出的字节缓冲区
        //  @param pos 输入/输出参数：输入为起始位置，输出为编码后位置
        //  @return int 编码后的字节数
        inline int varint_encode(int64_t value, uint8_t *buffer, int *pos) {
            bool sign = value < 0;
            uint64_t abs_value = sign ? -(uint64_t)value : (uint64_t)value;

            // 处理第一个6位块
            uint8_t first_byte = abs_value & 0x3F;
            abs_value >>= 6;
            first_byte |= (sign << 6) | ((abs_value != 0) << 7);
            buffer[(*pos)++] = first_byte;
            int num_bytes = 1;

            // 处理后续7位块
            while (abs_value != 0) {
                uint8_t byte = abs_value & 0x7F;
                abs_value >>= 7;
                buffer[(*pos)++] = byte | ((abs_value != 0) << 7);
                num_bytes++;
            }

            return num_bytes;
        }

        /**
         * @brief 解码变长整数（VLQ编码），采用类似UTF-8的方案存储有符号整数
         *
         * 编码规则：
         * - 每个字节的低7位（0x7F）存储数据
         * - 最高位（0x80）为连续标志：1=还有后续字节，0=终止
         * - 首字节的0x40位（第六位）为符号位：1=负数，0=正数
         *
         * 位掩码说明：
         * +---------+-----------+-----------------------+
         * | 掩码值  | 二进制    | 作用                  |
         * +---------+-----------+-----------------------+
         * | 0x80    | 10000000  | 连续标志位            |
         * | 0x7F    | 01111111  | 首字节数据掩码（7位） |
         * | 0x40    | 01000000  | 符号位                |
         * | 0x3F    | 00111111  | 后续字节数据掩码（6位）|
         * +---------+-----------+-----------------------+
         *
         * 解码流程：
         * 1. 读取首字节：
         *    - 取低7位（&0x7F）作为初始值
         *    - 检查0x40位确定符号
         * 2. 循环读取后续字节（直到最高位为0）：
         *    - 取低6位（&0x3F）并左移累加
         * 3. 应用符号位
         *
         * @param b     输入字节数组（编码数据）
         * @param pos   输入/输出参数：
         *              - 输入：起始解码位置
         *              - 输出：解码后位置（pos+已读字节数）
         * @return int  解码后的有符号整数值
         */
        inline int64_t varint_decode(const uint8_t *b, int *pos) {
            uint8_t byte = b[(*pos)++];
            bool sign = (byte & 0x40) != 0;
            int64_t data = byte & 0x3F;
            int shift = 6;

            while (byte & 0x80) {
                byte = b[(*pos)++];
                data |= (int64_t)(byte & 0x7F) << shift;
                shift += 7;
            }

            return sign ? -data : data;
        }

        const int TM_H_WIDTH = 1000000;  // 小时部分基数（前4位）
        const int TM_M_WIDTH = 10000;    // 分钟部分基数（中间2位）
        const int TM_T_WIDTH = 1000;     // 毫秒位基数

        /// 转换快照中的时间戳, HH:mm:ss.SSS
        /// @param stamp 时间戳
        /// @return 返回格式化的时间字符串
        inline std::string format_time(int64_t stamp) {
            char buffer[32] = {0};
            int h, tmp1, m1, tmp2, tmp3, m;
            //int s, t;
            double st;

            // 解析小时部分
            h = stamp / TM_H_WIDTH;         // 提取前4位作为小时
            tmp1 = stamp % TM_H_WIDTH;      // 剩余部分

            // 解析分钟部分
            m1 = tmp1 / TM_M_WIDTH;         // 中间2位作为初始分钟
            tmp2 = tmp1 % TM_M_WIDTH;       // 最后2位作为秒基数

            if (h > 100) {
                h /= 10;
            }
            if (m1 < 60) {
                // 正常分支：直接使用分钟
                m = m1;
                tmp3 = tmp2 * 60;           // 转换为秒基数
//                s = tmp3 / TM_M_WIDTH;      // 整数秒部分
//                t = tmp3 % TM_M_WIDTH;      // 毫秒基数
//                t /= TM_T_WIDTH;            // 转换为3位毫秒
                st = (double)tmp3 / TM_M_WIDTH; // 合并秒与毫秒
            } else {
                // 分钟溢出分支：进位处理
                h += 1;                    // 小时进位
                tmp3 = tmp1;                     // 使用整个剩余部分
                m = tmp3 / TM_H_WIDTH;           // 重新计算分钟
                tmp3 = (tmp3 % TM_H_WIDTH) * 60; // 转换为秒基数
//                s = tmp3 / TM_H_WIDTH;           // 整数秒部分
//                t = tmp3 % TM_H_WIDTH;           // 毫秒基数
//                t /= TM_T_WIDTH;                 // 转换为3位毫秒
                st = (double)tmp3 / TM_H_WIDTH;
            }

            // 格式化输出（自动补零）
            int n = std::snprintf(buffer, sizeof(buffer), "%02d:%02d:%06.3f", h, m, st);
            return std::string(buffer, 0, n);
        }

        // 将整数转换为 float64
        // 该函数将整数转换为 float64，模拟 Go 语言中的位操作逻辑
        // 该函数假设输入为 32 位无符号整数，并将其分解为 4 个字节部分，计算出对应的 float64 值
        // 注意：此函数仅适用于整数类型，且不处理负数或非整数输入
        template<typename T>
        inline f64 IntToFloat64(T integer) {
            // 确保输入为整数类型
            static_assert(std::is_integral_v<T>, "T must be an integer type");

            // 将输入视为 32 位无符号整数（兼容原 Go 代码的位操作逻辑）
            auto uinteger = static_cast<u32>(integer);

            // 分解 4 个字节部分
            int32_t logPoint = u8(uinteger >> 24) & 0xFF;        // 最高 8 位 (对应 Go 的 >>(8*3))
            int hleax = u8(uinteger >> 16) & 0xFF;        // 次高 8 位
            int lheax = u8(uinteger >> 8) & 0xFF;        // 次低 8 位
            int lleax = u8(uinteger) & 0xFF;                // 最低 8 位

            // 计算指数偏移
            int dwEcx = logPoint * 2 - 0x7F;
            int dwEdx = logPoint * 2 - 0x86;
            int dwEsi = logPoint * 2 - 0x8E;
            int dwEax = logPoint * 2 - 0x96;

            // 计算 dblXmm6
            f64 dblXmm6 = 0.0;
            int tmpEax = (dwEcx < 0) ? -dwEcx : dwEcx;
            dblXmm6 = std::pow(2.0, tmpEax);
            if (dwEcx < 0) {
                dblXmm6 = 1.0 / dblXmm6;
            }

            // 计算 dblXmm4
            f64 dblXmm4 = 0.0;
            if (hleax > 0x80) {
                int dwtmpeax = dwEdx + 1;
                f64 tmpdblXmm3 = std::pow(2.0, dwtmpeax);
                f64 dblXmm0 = std::pow(2.0, dwEdx) * 128.0;
                dblXmm0 += (hleax & 0x7F) * tmpdblXmm3;
                dblXmm4 = dblXmm0;
            } else {
                if (dwEdx >= 0) {
                    dblXmm4 = std::pow(2.0, dwEdx) * hleax;
                } else {
                    dblXmm4 = (1.0 / std::pow(2.0, -dwEdx)) * hleax;
                }
            }

            // 计算 dblXmm3 和 dblXmm1
            f64 dblXmm3 = std::pow(2.0, dwEsi) * lheax;
            f64 dblXmm1 = std::pow(2.0, dwEax) * lleax;

            // 如果 hleax 最高位为 1，额外乘 2
            if ((hleax & 0x80) != 0) {
                dblXmm3 *= 2.0;
                dblXmm1 *= 2.0;
            }

            // 返回累加结果
            return dblXmm6 + dblXmm4 + dblXmm3 + dblXmm1;
        }

        // 判断 float64 是否为 NaN 或 Inf
        inline bool Float64IsNaN(double f) {
            return std::isnan(f) || std::isinf(f);
        }

        // 模板函数：将数值类型转换为 double
        template <typename T>
        inline double NumberToFloat64(T v) {
            // 确保输入类型是 uint16_t, uint32_t 或 float
            static_assert(std::is_same_v<T, uint16_t> ||
                          std::is_same_v<T, uint32_t> ||
                          std::is_same_v<T, float>,
                          "Unsupported type for NumberToFloat64");

            return static_cast<double>(v);
        }
    } // namespace encoding
} // namespace level1

#endif //QUANT1X_LEVEL1_ENCODING_H
