#pragma once
#ifndef API_H
#define API_H 1

/* So that we can use inline on some critical functions, and use
 * GNUC attributes (such as to get -Wall warnings for printf-like
 * functions).  Both __inline__ and __attribute__ exist for gcc >= 2.7,
 * other !__GNUC__ compilers may provide __attribute__ still.
 *
 * We've since discovered that the gcc shipped with NeXT systems
 * as "cc" is completely broken.  It claims to be __GNUC__ and so
 * on, but it doesn't implement half of the things that __GNUC__
 * means.  In particular it's missing inline and the __attribute__
 * stuff.  So we hack around it.  PR#1613. -djg
 */
#if defined(__GNUC__) &&                                                       \
    (__GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7)) && !defined(NEXT)
#define API_INLINE __inline__
#define API_HAS_INLINE 1
#else /* !__GNUC__ */
#if defined(_MSC_VER)
#define API_INLINE __inline
#define API_HAS_INLINE 1
#else /* !_MSC_VER */
#define API_INLINE
#define API_HAS_INLINE 0
#endif /* !_MSC_VER */
/* __has_attribute should always be a pre-defined macro, but not
 * necessarily __attribute__ (e.g. builtin), so check for both to
 * avoid overriding __attribute__.
 */
#if !(defined(__attribute__) || defined(__has_attribute))
#define __attribute__(__x)
#endif
#endif /* !__GNUC__ */
#ifndef __has_attribute
#define __has_attribute(__x) 0
#endif

#if defined(DOXYGEN) || !defined(WIN32)

/**
 * The public API functions are declared with API_DECLARE(), so they may
 * use the most appropriate calling convention.  Public API functions with
 * variable arguments must use API_DECLARE_NONSTD().
 *
 * @remark Both the declaration and implementations must use the same macro.
 *
 * <PRE>
 * API_DECLARE(rettype) api_func(args)
 * </PRE>
 * @see API_DECLARE_NONSTD @see API_DECLARE_DATA
 * @remark Note that when API compiles the library itself, it passes the
 * symbol -DAPI_DECLARE_EXPORT to the compiler on some platforms (e.g. Win32)
 * to export public symbols from the dynamic library build.\n
 * The user must define the API_DECLARE_STATIC when compiling to target
 * the static API library on some platforms (e.g. Win32.)  The public symbols
 * are neither exported nor imported when API_DECLARE_STATIC is defined.\n
 * By default, compiling an application and including the API public
 * headers, without defining API_DECLARE_STATIC, will prepare the code to be
 * linked to the dynamic library.
 */
#define API_DECLARE(type) type

/**
 * The public API functions using variable arguments are declared with
 * API_DECLARE_NONSTD(), as they must follow the C language calling convention.
 * @see API_DECLARE @see API_DECLARE_DATA
 * @remark Both the declaration and implementations must use the same macro.
 * <PRE>
 *
 * API_DECLARE_NONSTD(rettype) api_func(args, ...);
 *
 * </PRE>
 */
#define API_DECLARE_NONSTD(type) type

/**
 * The public API variables are declared with API_DECLARE_DATA.
 * This assures the appropriate indirection is invoked at compile time.
 * @see API_DECLARE @see API_DECLARE_NONSTD
 * @remark Note that the declaration and implementations use different forms,
 * but both must include the macro.
 *
 * <PRE>
 *
 * extern API_DECLARE_DATA type api_variable;\n
 * API_DECLARE_DATA type api_variable = value;
 *
 * </PRE>
 */
#define API_DECLARE_DATA

#elif defined(API_DECLARE_STATIC)
#define API_DECLARE(type) type __stdcall
#define API_DECLARE_NONSTD(type) type __cdecl
#define API_DECLARE_DATA
#elif defined(API_DECLARE_EXPORT)
#define API_DECLARE(type) __declspec(dllexport) type __stdcall
#define API_DECLARE_NONSTD(type) __declspec(dllexport) type __cdecl
#define API_DECLARE_DATA __declspec(dllexport)
#else
#define API_DECLARE(type) __declspec(dllimport) type __stdcall
#define API_DECLARE_NONSTD(type) __declspec(dllimport) type __cdecl
#define API_DECLARE_DATA __declspec(dllimport)
#endif

#if !defined(WIN32) || defined(APU_MODULE_DECLARE_STATIC)
/**
 * Declare a dso module's exported module structure as API_MODULE_DECLARE_DATA.
 *
 * Unless API_MODULE_DECLARE_STATIC is defined at compile time, symbols
 * declared with API_MODULE_DECLARE_DATA are always exported.
 * @code
 * module API_MODULE_DECLARE_DATA mod_tag
 * @endcode
 */
#define API_MODULE_DECLARE_DATA
#else
#define API_MODULE_DECLARE_DATA __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

API_DECLARE(int) add(int a, int b);

#ifdef __cplusplus
}
#endif

#endif // API_H