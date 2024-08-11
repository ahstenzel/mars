#ifndef MARS_STD_PLATFORM_H
#define MARS_STD_PLATFORM_H
/**
 * platform.h
 * Platform-specific preprocessor definitions.
*/

// Detect compiler
#if defined(__clang__)
#define MARS_CMP_CLANG
#elif (defined(__GNUC__) || defined(__GNUG__)) && !defined(__INTEL_COMPILER)
#define MARS_CMP_GCC
#elif defined(_MSC_VER)
#define MARS_CMP_MSVC
#elif
#define MARS_CMP_UNKNOWN
#endif

// Detect architecture
#if defined(__arm__) || defined(__thumb__) ||\
	defined(_M_ARM)  || defined(_M_ARMT)
#define MARS_ARCH_ARM
#elif defined(__aarch64__) || defined (_M_ARM64)
#define MARS_ARCH_ARM64
#elif defined(i386) || defined(__i386) || defined(__i386__) ||\
	defined(i486) || defined(__i486) || defined(__i486__) ||\
	defined(i586) || defined(__i586) || defined(__i586__) ||\
	defined(i686) || defined(__i686) || defined(__i686__) ||\
	defined(_M_IX86) || defined(_M_X86) || defined(_X86_)
#define MARS_ARCH_X86
#elif defined(__x86_64) || defined(__x86_64__) ||\
	defined(__amd64) || defined(__amd64__) ||\
	defined(_M_X64) || defined(_M_AMD64)
#define MARS_ARCH_X64
#else
#define MARS_ARCH_UNKNOWN
#endif

// Detect OS
#if defined(_WIN64) || defined(_WIN32)
#define MARS_OS_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
#define MARS_OS_MAC
#elif defined(__linux__)
#define MARS_OS_LINUX
#else
#define MARS_OS_UNKNOWN
#endif

// Warning Suppression Definitions
#if defined(MARS_CMP_MSVC)
	#define MARS_DO_PRAGMA(X) _Pragma(#X)
	#define MARS_DISABLE_ALL_WARNINGS_PUSH				MARS_DO_PRAGMA(warning(push, 0))
	#define MARS_DISABLE_WARNING_PUSH					MARS_DO_PRAGMA(warning(push))
	#define MARS_DISABLE_WARNING_POP					MARS_DO_PRAGMA(warning(pop))
	#define MARS_DISABLE_WARNING(X)						MARS_DO_PRAGMA(warning(disable : X))

	#define MARS_DISABLE_WARNING_UNSECURE_FUNCTION		MARS_DISABLE_WARNING(4996)
	#define MARS_DISABLE_WARNING_UNUSED_RESULT

#elif defined(MARS_CMP_GCC) || defined(MARS_CMP_CLANG)
	#define MARS_DO_PRAGMA(X) _Pragma(#X)
	#define MARS_DISABLE_ALL_WARNINGS_PUSH
	#define MARS_DISABLE_WARNING_PUSH					MARS_DO_PRAGMA(GCC diagnostic push)
	#define MARS_DISABLE_WARNING_POP					MARS_DO_PRAGMA(GCC diagnostic pop)
	#define MARS_DISABLE_WARNING(id)					MARS_DO_PRAGMA(GCC diagnostic ignored #id)

	#define MARS_DISABLE_WARNING_UNSECURE_FUNCTION
	#define MARS_DISABLE_WARNING_UNUSED_RESULT			MARS_DISABLE_WARNING(-Wunused-result)
#else
	#define MARS_DO_PRAGMA(X)
	#define MARS_DISABLE_WARNING_PUSH
	#define MARS_DISABLE_WARNING_POP
	#define MARS_DISABLE_WARNING(id)

	#define MARS_DISABLE_WARNING_UNSECURE_FUNCTION
	#define MARS_DISABLE_WARNING_UNUSED_RESULT
#endif

// POSIX function name resolution
#if !defined(MARS_OS_WINDOWS) && defined(MARS_CMP_GCC)
	#define printf_s printf
	#define vprintf_s vprintf
	#define fprintf_s fprintf
	#define vfprintf_s vfprintf
	#define memcpy_s(dest, dest_size, src, src_size) memcpy(dest, src, src_size)
	#define memmove_s(dest, dest_size, src, src_size) memmove(dest, src, src_size)
	#define strcat_s(dest, dest_size, src) strcat(dest, src)
	#define strcpy_s(dest, dest_size, src) strcpy(dest, src)
#endif

// Function name refrencing
#if !defined(MARS_STR_FUNC)
	#if defined(MARS_CMP_MSVC)
		#define MARS_STR_FUNC __FUNCTION__
	#elif defined(MARS_CMP_GCC) || defined(MARS_CMP_CLANG)
		#define MARS_STR_FUNC __func__
	#else
		#define MARS_STR_FUNC "N/A"
	#endif
#endif

// Shared library interfacing
#if defined(MARS_BUILD_SHARED)
	#if defined(MARS_OS_WINDOWS) && (defined(MARS_CMP_MSVC) || defined(MARS_CMP_GCC))
		#if defined(MARS_SHARED_EXPORT)
			#define MARS_API __declspec(dllexport)
		#else
			#define MARS_API __declspec(dllimport)
		#endif
	#elif defined(MARS_CMP_CLANG)
		#if defined(MARS_SHARED_EXPORT)
			#define MARS_API __attribute__((dllexport))
		#else
			#define MARS_API __attribute__((dllimport))
		#endif
	#else
		#define MARS_API
	#endif
#else
	#define MARS_API
#endif

// Inline specifier
#if !defined(MARS_DISABLE_INLINE) && !defined(MARS_CMP_MSVC)
	#define MARS_INLINE inline
#else
	#define MARS_INLINE
#endif

#endif // MARS_STD_PLATFORM_H