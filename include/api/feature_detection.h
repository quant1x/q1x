#pragma once
#ifndef API_FEATURE_DETECTION_H
#define API_FEATURE_DETECTION_H 1

#include <cstddef>

// ==================================================================
// == C++ 标准检测 ==
// ==================================================================

#define CPP_STD_CXX98     199711L
#define CPP_STD_CXX11     201103L
#define CPP_STD_CXX14     201402L
#define CPP_STD_CXX17     201703L
#define CPP_STD_CXX20     202002L
#define CPP_STD_CXX23     202302L

#if defined(_MSVC_LANG)
#define TARGET_CPP (__cplusplus ? __cplusplus : _MSVC_LANG)
#else
#define TARGET_CPP __cplusplus
#endif

#define TARGET_CPP_AT_LEAST(n) (TARGET_CPP >= CPP_STD_##n)
#define TARGET_CPP_IS(n)      (TARGET_CPP == CPP_STD_##n)

// ==================================================================
// == 编译器类型判断 ==
// ==================================================================

#define COMPILER_UNKNOWN     0
#define COMPILER_MSVC        1
#define COMPILER_GCC         2
#define COMPILER_CLANG       3
#define COMPILER_ICC         4

#define CURRENT_COMPILER \
    (defined(_MSC_VER) ? COMPILER_MSVC : \
     defined(__INTEL_COMPILER) ? COMPILER_ICC : \
     defined(__clang__) ? COMPILER_CLANG : \
     defined(__GNUC__) ? COMPILER_GCC : \
     COMPILER_UNKNOWN)

// 使用 TARGET_COMPILER_IS_* 命名风格（适合大型项目）
#define TARGET_COMPILER_IS_MSVC     (CURRENT_COMPILER == COMPILER_MSVC)
#define TARGET_COMPILER_IS_GCC      (CURRENT_COMPILER == COMPILER_GCC)
#define TARGET_COMPILER_IS_CLANG    (CURRENT_COMPILER == COMPILER_CLANG)
#define TARGET_COMPILER_IS_ICC      (CURRENT_COMPILER == COMPILER_ICC)

// 编译器版本检测（可选）
#if TARGET_COMPILER_IS_MSVC
#define COMPILER_VERSION (_MSC_VER)
#elif TARGET_COMPILER_IS_GCC
#define COMPILER_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#elif TARGET_COMPILER_IS_CLANG
#define COMPILER_VERSION (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#elif TARGET_COMPILER_IS_ICC
    #define COMPILER_VERSION (__INTEL_COMPILER)
#else
    #define COMPILER_VERSION 0
#endif

// 强制内联宏定义（示例）
#if TARGET_COMPILER_IS_MSVC
#define FORCE_INLINE __forceinline
#elif TARGET_COMPILER_IS_GCC || TARGET_COMPILER_IS_CLANG
#define FORCE_INLINE inline __attribute__((always_inline))
#else
#define FORCE_INLINE inline
#endif

// ==================================================================
// == 操作系统检测 ==
// ==================================================================

#define OS_IS_WINDOWS   (defined(_WIN32) || defined(_WIN64))
#define OS_IS_LINUX     (defined(__linux__))
#define OS_IS_APPLE     (defined(__APPLE__) && defined(__MACH__))
#define OS_IS_MACOS     (OS_IS_APPLE && defined(TARGET_OS_MAC) && TARGET_OS_MAC)
#define OS_IS_IOS       (OS_IS_APPLE && defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE)
#define OS_IS_ANDROID   (defined(__ANDROID__))
#define OS_IS_UNIX      (OS_IS_LINUX || OS_IS_MACOS || OS_IS_IOS || OS_IS_ANDROID)

// ==================================================================
// == CPU 架构检测 ==
// ==================================================================

#define CPU_ARCH_UNKNOWN      0
#define CPU_ARCH_X86_32     1
#define CPU_ARCH_X86_64     2
#define CPU_ARCH_ARM_32     3
#define CPU_ARCH_ARM_64     4
#define CPU_ARCH_MIPS       5
#define CPU_ARCH_POWERPC    6
#define CPU_ARCH_RISCV      7

#define CURRENT_CPU_ARCH \
    (defined(__x86_64__) || defined(_M_X64) ? CPU_ARCH_X86_64 : \
     defined(__i386__) || defined(_M_I386) ? CPU_ARCH_X86_32 : \
     defined(__aarch64__) || defined(_M_ARM64) ? CPU_ARCH_ARM_64 : \
     defined(__arm__) || defined(_M_ARM) ? CPU_ARCH_ARM_32 : \
     defined(__mips__) ? CPU_ARCH_MIPS : \
     defined(__powerpc__) || defined(__ppc__) ? CPU_ARCH_POWERPC : \
     defined(__riscv) ? CPU_ARCH_RISCV : \
     CPU_ARCH_UNKNOWN)

// 是否是 x86 架构
#define TARGET_CPU_HAS_X86_32     (CURRENT_CPU_ARCH == CPU_ARCH_X86_32)
#define TARGET_CPU_HAS_X86_64     (CURRENT_CPU_ARCH == CPU_ARCH_X86_64)

// 是否是 ARM 架构
#define TARGET_CPU_HAS_ARM_32     (CURRENT_CPU_ARCH == CPU_ARCH_ARM_32)
#define TARGET_CPU_HAS_ARM_64     (CURRENT_CPU_ARCH == CPU_ARCH_ARM_64)

// 是否是 64 位架构
#define TARGET_CPU_BITS_64 (TARGET_CPU_HAS_X86_64 || TARGET_CPU_HAS_ARM_64)
// 是否是 32 位架构
#define TARGET_CPU_BITS_32 (!TARGET_CPU_BITS_64 && CURRENT_CPU_ARCH != CPU_ARCH_UNKNOWN)

// 判断是否支持 inline 关键字（所有现代编译器都支持）
#define TARGET_COMPILER_HAS_INLINE_KEYWORD 1

// 判断是否支持 C++17 的 inline 变量
#if defined(TARGET_CPP_AT_LEAST) && TARGET_CPP_AT_LEAST(17)
#define CPP_HAS_INLINE_VARIABLES 1
#else
#define CPP_HAS_INLINE_VARIABLES 0
#endif

// 判断是否支持函数模板内联优化（一般都支持）
#define CPP_HAS_INLINE_TEMPLATES 1

// 判断是否支持内联汇编（取决于编译器和平台）
#if TARGET_COMPILER_IS_MSVC && (defined(_M_IX86) || defined(_M_X64))
#define TARGET_HAS_INLINE_ASM 1
#elif (IS_GCC || IS_CLANG) && !defined(__arm__) && !defined(__aarch64__)
#define TARGET_HAS_INLINE_ASM 1
#else
#define TARGET_HAS_INLINE_ASM 0
#endif

#endif // API_FEATURE_DETECTION_H
