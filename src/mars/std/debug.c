#include "mars/std/debug.h"
#include "mars/std/platform.h"

#include <stdlib.h>

#if defined(MARS_ENABLE_LOG)

#include <stdio.h>
#include <stdarg.h>

#if defined(MARS_OS_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN
#else
#include <execinfo.h>
#include "debug.h"
#endif

unsigned char _mars_g_debug_level_mask = MARS_DEBUG_LEVEL_ALL;

void _mars_dbg_log_write(const unsigned char level, const char* func, char* fmt, ...) {
	if ((_mars_g_debug_level_mask & level) != 0) {
		// Get variadic arguments
		va_list args;
		va_start(args, fmt);

		// Write to console
		char c = 'X';
		switch (level) {
			case (MARS_DEBUG_LEVEL_STATUS):
				printf_s("[*] (%s) ", func);
				vprintf_s(fmt, args);
				printf_s("\n");
			break;
			case (MARS_DEBUG_LEVEL_WARNING):
				c = '!';
			case (MARS_DEBUG_LEVEL_ERROR):
				fprintf_s(stderr, "[%c] (%s) ", c, func);
				vfprintf_s(stderr, fmt, args);
				fprintf_s(stderr, "\n");
			break;
		}
		va_end(args);
	}
}

#endif // defined(MARS_ENABLE_LOG)

#if defined(MARS_DEBUG)

int _mars_dbg_assert(const char* expr, const char* file, int line, const char* func, char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	fprintf_s(stderr, "[X] ASSERTION FAILED @%s:%d %s (%s) ", file, line, func, expr);
	vfprintf_s(stderr, fmt, args);
	fprintf_s(stderr, "\n");
	va_end(args);
	exit(MARS_ERROR_CODE_ASSERT);
	return 0;
}

#endif // defined(MARS_DEBUG)

unsigned int _mars_g_return_code = MARS_RETURN_CODE_OK;

unsigned char _mars_g_error_code_mask = MARS_ERROR_CODE_NONE;

void _mars_print_stack() {
#if defined(MARS_ENABLE_LOG)
	fprintf_s(stderr, "(Stack trace here TODO)\n");
#endif
}

void _mars_error_abort(unsigned int status) {
	if ((_mars_g_error_code_mask & status) != 0) {
		_mars_print_stack();
		exit(status);
	}
}