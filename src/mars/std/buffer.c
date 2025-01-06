#include "mars/std/buffer.h"
#include "mars/std/debug.h"

buffer_t* _buffer_resize(buffer_t* _buf, size_t _new_capacity) {
	if (_new_capacity == 0) {
		size_t c = MARS_NEXT_POW2(_buf->_capacity + 1);
		_new_capacity = umin(c, BUFFER_MAX_CAPACITY);
	}
	buffer_t* new_buffer = buffer_create_size(_new_capacity);
	memcpy_s(new_buffer->_buffer, _new_capacity, _buf->_buffer, _buf->_length);
	new_buffer->_length = _buf->_length;
	MARS_FREE(_buf);
	return new_buffer;
}

buffer_t* buffer_create() {
	return buffer_create_size(BUFFER_DEFAULT_CAPACITY);
}

buffer_t* buffer_create_size(size_t _capacity) {
	size_t buffer_size = offsetof(buffer_t, _buffer) + _capacity;
	buffer_t* buf = MARS_CALLOC(1, buffer_size);
	if (!buf) { 
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate buffer_t buffer!");
		return NULL; 
	}
	buf->_capacity = _capacity;
	return buf;
}

buffer_t* buffer_file_read(FILE* _fp, size_t _max_size) {
	if (!_fp) { return NULL; }

	// Calculate buffer size
	fseek(_fp, 0L, SEEK_END);
	long sz = ftell(_fp);
	if (sz == -1) { return NULL; }
	fseek(_fp, 0L, SEEK_SET);
	size_t buffer_size = 0;
	if (_max_size == 0) { buffer_size = (size_t)(sz); }
	else { buffer_size = umin(_max_size, (size_t)(sz)); }

	// Read buffer
	buffer_t* buf = buffer_create_size(buffer_size);
	fread(&buf->_buffer[0], 1, buffer_size, _fp);
	buf->_length = buffer_size;
	return buf;
}

void buffer_destroy(buffer_t* _buf) {
	MARS_FREE(_buf);
}

uint8_t buffer_get_u8(buffer_t* _buf, size_t _idx) {
	if (!_buf || _idx >= _buf->_length) { return 0; }
	return _buf->_buffer[_idx];
}

uint16_t buffer_get_u16(buffer_t* _buf, size_t _idx) {
	if (!_buf || _idx >= (_buf->_length - 1)) { return 0; }
	uint16_t val = 0;
	uint8_t* ptr = &_buf->_buffer[_idx];
	val |= ((uint16_t)*(ptr++));
	val |= ((uint16_t)*(ptr++) << 8);
	return val;
}

uint32_t buffer_get_u32(buffer_t* _buf, size_t _idx) {
	if (!_buf || _idx >= (_buf->_length - 3)) { return 0; }
	uint32_t val = 0;
	uint8_t* ptr = &_buf->_buffer[_idx];
	val |= ((uint32_t)*(ptr++));
	val |= ((uint32_t)*(ptr++) << 8);
	val |= ((uint32_t)*(ptr++) << 16);
	val |= ((uint32_t)*(ptr++) << 24);
	return val;
}

uint64_t buffer_get_u64(buffer_t* _buf, size_t _idx) {
	if (!_buf || _idx >= (_buf->_length - 7)) { return 0; }
	uint64_t val = 0;
	uint8_t* ptr = &_buf->_buffer[_idx];
	val |= ((uint64_t)*(ptr++));
	val |= ((uint64_t)*(ptr++) << 8);
	val |= ((uint64_t)*(ptr++) << 16);
	val |= ((uint64_t)*(ptr++) << 24);
	val |= ((uint64_t)*(ptr++) << 32);
	val |= ((uint64_t)*(ptr++) << 40);
	val |= ((uint64_t)*(ptr++) << 48);
	val |= ((uint64_t)*(ptr++) << 56);
	return val;
}

size_t buffer_get_str(buffer_t* _buf, size_t _idx, size_t _max_size, char* _dest) {
	if (!_buf || _idx >= _buf->_length) { return 0; }

	// Calculate destination size
	uint8_t* ptr = &_buf->_buffer[_idx];
	size_t sz = 0;
	while(sz < _max_size && _idx + sz < _buf->_length) {
		if (*(char*)(ptr++) == '\0') { break; }
		sz++;
	}

	// Copy to destination buffer
	if (!_dest) { return sz + 1; }
	ptr = &_buf->_buffer[_idx];
	memcpy_s(_dest, _max_size, (void*)ptr, sz);
	_dest[sz] = '\0';
	return sz + 1;
}

void buffer_set_u8(buffer_t* _buf, size_t _idx, uint8_t _val) {
	if (!_buf || _idx >= _buf->_length) { return; }
	_buf->_buffer[_idx] = _val;
}

void buffer_set_u16(buffer_t* _buf, size_t _idx, uint16_t _val) {
	if (!_buf || _idx >= (_buf->_length - 1)) { return; }
	_buf->_buffer[_idx] = (uint8_t)(_val & 0xFF);
	_buf->_buffer[_idx + 1] = (uint8_t)((_val >> 8) & 0xFF);
}

void buffer_set_u32(buffer_t* _buf, size_t _idx, uint32_t _val) {
	if (!_buf || _idx >= (_buf->_length - 3)) { return; }
	_buf->_buffer[_idx] = (uint8_t)(_val & 0xFF);
	_buf->_buffer[_idx + 1] = (uint8_t)((_val >> 8) & 0xFF);
	_buf->_buffer[_idx + 2] = (uint8_t)((_val >> 16) & 0xFF);
	_buf->_buffer[_idx + 3] = (uint8_t)((_val >> 24) & 0xFF);
}

void buffer_set_u64(buffer_t* _buf, size_t _idx, uint64_t _val) {
	if (!_buf || _idx >= (_buf->_length - 7)) { return; }
	_buf->_buffer[_idx] = (uint8_t)(_val & 0xFF);
	_buf->_buffer[_idx + 1] = (uint8_t)((_val >> 8) & 0xFF);
	_buf->_buffer[_idx + 2] = (uint8_t)((_val >> 16) & 0xFF);
	_buf->_buffer[_idx + 3] = (uint8_t)((_val >> 24) & 0xFF);
	_buf->_buffer[_idx + 4] = (uint8_t)((_val >> 32) & 0xFF);
	_buf->_buffer[_idx + 5] = (uint8_t)((_val >> 40) & 0xFF);
	_buf->_buffer[_idx + 6] = (uint8_t)((_val >> 48) & 0xFF);
	_buf->_buffer[_idx + 7] = (uint8_t)((_val >> 56) & 0xFF);
}

void buffer_set_str(buffer_t* _buf, size_t _idx, char* _str, size_t _max_size) {
	if (!_buf || _idx >= (_buf->_length - _max_size - 1)) { return; }
	memcpy_s(&_buf->_buffer[_idx], _max_size, _str, strlen(_str));
}

void buffer_push_u8(buffer_t** _buf, uint8_t _val) {
	if (!_buf || !(*_buf)) { return; }
	buffer_t* buf = *_buf;

	// Resize buffer
	while (buf->_length > (buf->_capacity - 1)) {
		buffer_t* temp = _buffer_resize(buf, 0);
		if (!temp) { return; }
		(*_buf) = temp;
		buf = temp;
	}

	buf->_buffer[buf->_length++] = _val;
	buf->_length += 1;
}

void buffer_push_u16(buffer_t** _buf, uint16_t _val) {
	if (!_buf || !(*_buf)) { return; }
	buffer_t* buf = *_buf;

	// Resize buffer
	while (buf->_length > (buf->_capacity - 2)) {
		buffer_t* temp = _buffer_resize(buf, 0);
		if (!temp) { return; }
		(*_buf) = temp;
		buf = temp;
	}
	
	buf->_buffer[buf->_length++] = (uint8_t)(_val & 0xFF);
	buf->_buffer[buf->_length++] = (uint8_t)((_val >> 8) & 0xFF);
	buf->_length += 2;
}

void buffer_push_u32(buffer_t** _buf, uint32_t _val) {
	if (!_buf || !(*_buf)) { return; }
	buffer_t* buf = *_buf;

	// Resize buffer
	while (buf->_length > (buf->_capacity - 4)) {
		buffer_t* temp = _buffer_resize(buf, 0);
		if (!temp) { return; }
		(*_buf) = temp;
		buf = temp;
	}
	
	buf->_buffer[buf->_length++] = (uint8_t)(_val & 0xFF);
	buf->_buffer[buf->_length++] = (uint8_t)((_val >> 8) & 0xFF);
	buf->_buffer[buf->_length++] = (uint8_t)((_val >> 16) & 0xFF);
	buf->_buffer[buf->_length++] = (uint8_t)((_val >> 24) & 0xFF);
	buf->_length += 4;
}

void buffer_push_u64(buffer_t** _buf, uint64_t _val) {
	if (!_buf || !(*_buf)) { return; }
	buffer_t* buf = *_buf;

	// Resize buffer
	while (buf->_length > (buf->_capacity - 8)) {
		buffer_t* temp = _buffer_resize(buf, 0);
		if (!temp) { return; }
		(*_buf) = temp;
		buf = temp;
	}
	
	buf->_buffer[buf->_length++] = (uint8_t)(_val & 0xFF);
	buf->_buffer[buf->_length++] = (uint8_t)((_val >> 8) & 0xFF);
	buf->_buffer[buf->_length++] = (uint8_t)((_val >> 16) & 0xFF);
	buf->_buffer[buf->_length++] = (uint8_t)((_val >> 24) & 0xFF);
	buf->_buffer[buf->_length++] = (uint8_t)((_val >> 32) & 0xFF);
	buf->_buffer[buf->_length++] = (uint8_t)((_val >> 40) & 0xFF);
	buf->_buffer[buf->_length++] = (uint8_t)((_val >> 48) & 0xFF);
	buf->_buffer[buf->_length++] = (uint8_t)((_val >> 56) & 0xFF);
	buf->_length += 8;
}

void buffer_push_str(buffer_t** _buf, char* _str, size_t _max_size) {
	if (!_buf || !(*_buf)) { return; }
	buffer_t* buf = *_buf;

	// Resize buffer
	while (buf->_length > (buf->_capacity - _max_size)) {
		buffer_t* temp = _buffer_resize(buf, 0);
		if (!temp) { return; }
		(*_buf) = temp;
		buf = temp;
	}

	size_t str_size = strlen(_str);
	memcpy_s(&buf->_buffer[buf->_length], _max_size, _str, str_size);
	buf->_length += str_size;
}