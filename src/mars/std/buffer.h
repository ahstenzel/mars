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
buffer_t* buffer_create_size(size_t _capacity);

/// @brief Read file contents into a byte buffer. Does not handle opening / verifying / closing the file handle.
/// @param _fp Open file pointer
/// @param _max_size Max buffer size
/// @return Buffer pointer
buffer_t* buffer_file_read(FILE* _fp, size_t _max_size);

/// @brief Deallocate the byte buffer.
/// @param _buf Buffer pointer
void buffer_destroy(buffer_t* _buf);

/// @brief Get one byte from the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @return Data (uint8_t)
uint8_t buffer_get_u8(buffer_t* _buf, size_t _idx);

/// @brief Get two bytes from the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @return Data (uint16_t)
uint16_t buffer_get_u16(buffer_t* _buf, size_t _idx);

/// @brief Get four bytes from the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @return Data (uint32_t)
uint32_t buffer_get_u32(buffer_t* _buf, size_t _idx);

/// @brief Get eight bytes from the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @return Data (uint64_t)
uint64_t buffer_get_u64(buffer_t* _buf, size_t _idx);

/// @brief Get a NULL-terminated string of characters from the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @param _halt Stop reading when a NULL-terminator is encountered
/// @param _max_size Maximum number of characters to read
/// @param _dest Preallocated destination character buffer (if NULL, calculate required size & immediately return)
/// @return Destination buffer size
size_t buffer_get_str(buffer_t* _buf, size_t _idx, size_t _max_size, bool _halt, char* _dest);

/// @brief Overwrite one byte in the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @param _val Value
void buffer_set_u8(buffer_t* _buf, size_t _idx, uint8_t _val);

/// @brief Overwrite two bytes in the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @param _val Value
void buffer_set_u16(buffer_t* _buf, size_t _idx, uint16_t _val);

/// @brief Overwrite four bytes in the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @param _val Value
void buffer_set_u32(buffer_t* _buf, size_t _idx, uint32_t _val);

/// @brief Overwrite eight bytes in the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @param _val Value
void buffer_set_u64(buffer_t* _buf, size_t _idx, uint64_t _val);

/// @brief Overwrite a string of characters in the buffer.
/// @param _buf Buffer pointer
/// @param _idx Index
/// @param _str NULL-terminated string
/// @param _max_size Max number of characters to write
void buffer_set_str(buffer_t* _buf, size_t _idx, char* _str, size_t _max_size);

/// @brief Append one byte to the buffer.
/// @param _buf Buffer double pointer
/// @param _val Value
void buffer_push_u8(buffer_t** _buf, uint8_t _val);

/// @brief Append two bytes to the buffer.
/// @param _buf Buffer double pointer
/// @param _val Value
void buffer_push_u16(buffer_t** _buf, uint16_t _val);

/// @brief Append four bytes to the buffer.
/// @param _buf Buffer double pointer
/// @param _val Value
void buffer_push_u32(buffer_t** _buf, uint32_t _val);

/// @brief Append eight bytes to the buffer.
/// @param _buf Buffer double pointer
/// @param _val Value
void buffer_push_u64(buffer_t** _buf, uint64_t _val);

/// @brief Append a string of characters to the buffer.
/// @param _buf Buffer double pointer
/// @param _str NULL-terminatede string
/// @param _max_size Max number of characters to write
void buffer_push_str(buffer_t** _buf, char* _str, size_t _max_size);

#endif MARS_STD_BUFFER_H