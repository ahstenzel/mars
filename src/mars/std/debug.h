#ifndef MARS_DEBUG_H
#define MARS_DEBUG_H
/**
 * debug.h
 * Debug logging & tracing definitions.
*/
#include "mars/std/platform.h"

// Debug marker
#if !defined(NDEBUG)
#define MARS_DEBUG
#endif

// Logging marker
#if defined(MARS_DEBUG) || defined(MARS_FORCE_LOG)
#define MARS_ENABLE_LOG
#endif


//----------------------------------------------------------------------------------
// Debug logging
//----------------------------------------------------------------------------------

#if defined(MARS_ENABLE_LOG)

// Debug level flags
#define MARS_DEBUG_LEVEL_NONE			0x00		// Mask for no message levels.
#define MARS_DEBUG_LEVEL_ALL			0xFF		// Mask for all message levels.
#define MARS_DEBUG_LEVEL_STATUS			0x01		// Low-level status message.
#define MARS_DEBUG_LEVEL_WARNING		0x02		// Mid-level warning message - ignoring may lead to issues.
#define MARS_DEBUG_LEVEL_ERROR			0x04		// High-level error message - catastrophic failure.

// Global variables
extern unsigned char _mars_g_debug_level_mask;  // Mask for disabling certain debug messages.

/// @brief Write a string to the debug output.
/// @param level Message severity level
/// @param fmt Format string
/// @param ... Format arguments
void _mars_dbg_log_write(const unsigned char level, const char* func, char* fmt, ...);

/// @brief Write a debug info message.
/// @param s Format string
/// @param ... Format arguments
#define MARS_DEBUG_LOG(s, ...) _mars_dbg_log_write(MARS_DEBUG_LEVEL_STATUS, MARS_STR_FUNC, s, ##__VA_ARGS__)

/// @brief Write a debug warning message.
/// @param s Format string
/// @param ... Format arguments
#define MARS_DEBUG_WARN(s, ...) _mars_dbg_log_write(MARS_DEBUG_LEVEL_WARNING, MARS_STR_FUNC, s, ##__VA_ARGS__)

/// @brief Write a debug error message.
/// @param s Format string
/// @param ... Format arguments
#define MARS_DEBUG_ERROR(s, ...) _mars_dbg_log_write(MARS_DEBUG_LEVEL_ERROR, MARS_STR_FUNC, s, ##__VA_ARGS__)

/// @brief Set the bitmask that defines what debug messages are shown.
#define MARS_DEBUG_SET_SEVERITY(x) _mars_g_debug_level_mask = x

#else

#define MARS_DEBUG_LOG(s, ...) ((void)0)
#define MARS_DEBUG_WARN(s, ...) ((void)0)
#define MARS_DEBUG_ERROR(s, ...) ((void)0)
#define MARS_DEBUG_SET_SEVERITY(x) ((void)0)

#endif // defined(MARS_ENABLE_LOG)


//----------------------------------------------------------------------------------
// Return codes
//----------------------------------------------------------------------------------

// Code definitions
#define MARS_RETURN_OK						0		// Return code for successful execution.
#define MARS_RETURN_CRITICAL_ERROR			1		// Return code for an unhandled critical error.
#define MARS_RETURN_GENERIC_ERROR			2		// Return code for an unspecified error.
#define MARS_RETURN_INVALID_REFERENCE		3		// Return code for an invalid pointer.
#define MARS_RETURN_INVALID_ID				4		// Return code for an invalid unique ID.
#define MARS_RETURN_INVALID_PARAMETER		5		// Return code for an improperly formatted function parameter.
#define MARS_RETURN_CONTAINER_FAILURE		6		// Return code for an internal failure in data containers.
#define MARS_RETURN_RESOURCE_FAILURE		7		// Return code for an internal failure in the resource management system.
#define MARS_RETURN_VULKAN_FAILURE			8		// Return code for an error in a vulkan internal function.
#define MARS_RETURN_FILESYSTEM_FAILURE		9		// Return code for a filesystem related error.

// Global variables
extern unsigned int _mars_g_return_code;	// Last recorded function return code.

/// @brief Get the last recorded function return code.
#define MARS_RETURN _mars_g_return_code

/// @brief Clear the last recorded function return code.
#define MARS_RETURN_CLEAR _mars_g_return_code = MARS_RETURN_OK

/// @brief Record the current function return code.
#define MARS_RETURN_SET(x) _mars_g_return_code = x


//----------------------------------------------------------------------------------
// Error raising
//----------------------------------------------------------------------------------

// Code definitions
#define MARS_ERROR_STATUS_NONE			0x00		// Mask for no errors.
#define MARS_ERROR_STATUS_ALL			0xFF		// Mask for all errors.
#define MARS_ERROR_STATUS_GENERIC		0x01		// Mask for uncategorized general error.
#define MARS_ERROR_STATUS_BAD_ALLOC		0x02		// Mask for failed memory allocation.
#define MARS_ERROR_STATUS_FILESYS		0x04		// Mask for filesystem error.
#define MARS_ERROR_STATUS_RENDERER		0x08		// Mask for rendering error.

// Global variables
extern unsigned char _mars_g_error_status_mask;  	// Mask for disabling certain error types

/// @brief Raise the given error status and abort the program if they are not masked off.
/// @param status Status code
void _mars_error_abort(unsigned int status);

/// @brief Prints the call stack to stderr.
void _mars_print_stack();

/// @brief Set the bitmask that defines which error codes should exit the program
#define MARS_ERROR_SET_SEVERITY(x) _mars_g_error_status_mask = x

/// @brief Print an error message, then either exit the program (if the error type hasn't been masked), or set
///	@brief the global return code to indicate a critical unhandled error and continue execution.
/// @param x Error status code
/// @param s Format string
/// @param ... Format arguments
#define MARS_ABORT(x, s, ...) { \
	MARS_DEBUG_ERROR(s, ##__VA_ARGS__); \
	_mars_error_abort(x); \
	MARS_RETURN_SET(MARS_RETURN_CRITICAL_ERROR); \
}

#endif // MARS_DEBUG_H