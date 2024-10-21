#ifndef MARS_STD_BASE64_H
#define MARS_STD_BASE64_H
/**
 * base64.h
 * Functions for base64 encoding/decoding.
*/
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/// @brief Encode a stream of data into a null-terminated base64 string. If result is NULL, return the calculated size of the output buffer.
/// @param data Input data stream
/// @param len Input data length
/// @param result Output buffer (or NULL)
/// @param maxCount Max number of characters to write to output
/// @param urlSafe Use '-' and '_' in place of '+' and '/'
/// @return Output buffer size (including null-terminator), or 0 on error
size_t base64Encode(unsigned char* data, size_t len, char* result, size_t maxCount, bool urlSafe);

/// @brief Decode a base64 null-terminated string into a data stream. If result is NULL, return the calculated size of the output buffer.
/// @param data Input string
/// @param result Output buffer (or NULL)
/// @param maxCount Max number of characters to write to output
/// @return Output buffer size, or 0 on error
size_t base64Decode(char* data, unsigned char* result, size_t maxCount);

#endif