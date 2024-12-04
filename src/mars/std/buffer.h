#ifndef MARS_STD_BUFFER_H
#define MARS_STD_BUFFER_H
/**
 * buffer.h
 * Dynamically resizing byte buffer.
*/
#include "mars/std/utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifndef BUFFER_DEFAULT_CAPACITY
#define BUFFER_DEFAULT_CAPACITY 1024ULL
#endif
#ifndef BUFFER_MAX_CAPACITY
#define BUFFER_MAX_CAPACITY SIZE_MAX - 1
#endif

typedef struct {
	size_t _length;
	size_t _capacity;
	uint8_t _buffer[];
} buffer_t;

buffer_t* _buffer_resize(buffer_t*, size_t);

/// @brief Create a new byte buffer.
/// @return Buffer pointer
buffer_t* buffer_create();

/// @brief Create a new byte buffer preallocated to a certain size.
/// @param _capacity Initial capacity
/// @return Buffer pointer
buffer_t* buffer_create_size(size_t);

/// @brief Read file contents into a byte buffer. Does not handle opening / verifying / closing the file handle.
/// @param _fp Open file pointer
/// @param _max_size Max buffer size
/// @return Buffer pointer
buffer_t* buffer_file_read(FILE*, size_t);

/// @brief Deallocate the byte buffer.
/// @param _buf Buffer pointer
void buffer_destroy(buffer_t*);

/// @brief Get one byte from the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @return Data (uint8_t)
uint8_t buffer_get_u8(buffer_t*, size_t);

/// @brief Get two bytes from the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @return Data (uint16_t)
uint16_t buffer_get_u16(buffer_t*, size_t);

/// @brief Get four bytes from the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @return Data (uint32_t)
uint32_t buffer_get_u32(buffer_t*, size_t);

/// @brief Get eight bytes from the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @return Data (uint64_t)
uint64_t buffer_get_u64(buffer_t*, size_t);

/// @brief Get a NULL-terminated string of characters from the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @param _max_size Maximum number of characters to read (will stop when a NULL-terminator is read)
/// @param _dest Preallocated destination character buffer (if NULL, calculate required size & immediately return)
/// @return Destination buffer size
size_t buffer_get_str(buffer_t*, size_t, size_t, char*);

/// @brief Overwrite one byte in the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @param _val Value
void buffer_set_u8(buffer_t*, size_t, uint8_t);

/// @brief Overwrite two bytes in the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @param _val Value
void buffer_set_u16(buffer_t*, size_t, uint16_t);

/// @brief Overwrite four bytes in the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @param _val Value
void buffer_set_u32(buffer_t*, size_t, uint32_t);

/// @brief Overwrite eight bytes in the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @param _val Value
void buffer_set_u64(buffer_t*, size_t, uint64_t);

/// @brief Overwrite a string of characters in the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @param _str NULL-terminated string
/// @param _max_size Max number of characters to write
void buffer_set_str(buffer_t*, size_t, char*, size_t);

/// @brief Append one byte to the buffer.
/// @param _buf Buffer double pointer
/// @param _val Value
void buffer_push_u8(buffer_t**, uint8_t);

/// @brief Append two bytes to the buffer.
/// @param _buf Buffer double pointer
/// @param _val Value
void buffer_push_u16(buffer_t**, uint16_t);

/// @brief Append four bytes to the buffer.
/// @param _buf Buffer double pointer
/// @param _val Value
void buffer_push_u32(buffer_t**, uint32_t);

/// @brief Append eight bytes to the buffer.
/// @param _buf Buffer double pointer
/// @param _val Value
void buffer_push_u64(buffer_t**, uint64_t);

/// @brief Append a string of characters to the buffer.
/// @param _buf Buffer double pointer
/// @param _str NULL-terminatede string
/// @param _max_size Max number of characters to write
void buffer_push_str(buffer_t**, char*, size_t);

#endif MARS_STD_BUFFER_H