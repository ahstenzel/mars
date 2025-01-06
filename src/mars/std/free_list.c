#include "mars/std/free_list.h"
#include "mars/std/debug.h"

size_t _free_list_buffer_size(size_t element_size, size_t capacity) {
	size_t c = element_size * capacity;
	if (c / capacity != element_size) { return 0; }
	size_t o = ((capacity/8)+1);
	return umax(sizeof(free_list_t), offsetof(free_list_t, _buffer) + c + o);
}

free_list_t* _free_list_factory(size_t element_size, size_t capacity) {
	size_t buffer_size = _free_list_buffer_size(element_size, capacity);
	if (buffer_size == 0) { return NULL; }
	free_list_t* list = MARS_CALLOC(buffer_size, 1);
	if (!list) { 
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate free_list buffer!");
		return NULL; 
	}
	list->_capacity = capacity;
	list->_element_size = element_size;
	return list;
}

free_list_t* _free_list_resize(free_list_t* list, size_t new_capacity) {
	// Calculate new capacity
	if (new_capacity == 0) {
		size_t c = MARS_NEXT_POW2(list->_capacity + 1);
		new_capacity = umin(c, FREE_LIST_MAX_CAPACITY);
	}
	if (new_capacity > FREE_LIST_MAX_CAPACITY) { return NULL; }

	// Create a new list & copy data over
	free_list_t* new_list = _free_list_factory(list->_element_size, new_capacity);
	if (!new_list) { return NULL; }

	size_t bit_dest_size = (list->_capacity / 8) + 1;
	memcpy_s(&new_list->_buffer[0], bit_dest_size, &list->_buffer[0], bit_dest_size);
	size_t data_dest_size = list->_capacity * list->_element_size;
	memcpy_s(_free_list_pos(new_list, 0), data_dest_size, _free_list_pos(list, 0), data_dest_size);

	//size_t dest_size = list->_length * list->_element_size;
	//memcpy_s(new_list->_buffer, dest_size, list->_buffer, dest_size);

	new_list->_length = list->_length;
	new_list->_next_free = list->_next_free;
	MARS_FREE(list);
	return new_list;
}

void* _free_list_insert(free_list_t** list, size_t* index, void* data) {
	// Error check
	if (!list || !(*list)) { goto free_list_insert_fail; }
	free_list_t* _list = *list;
	if (_free_list_bit_get(_list, _list->_next_free)) { goto free_list_insert_fail; }

	// Resize container
	if (_list->_length >= _list->_capacity) {
		free_list_t* temp = _free_list_resize(*list, 0);
		if (!temp) { goto free_list_insert_fail; }
		(*list) = temp;
		_list = temp;
	}

	// Add to empty slot
	uint8_t* dest = _free_list_pos(_list, _list->_next_free);
	size_t dest_size = _list->_element_size;
	memcpy_s(dest, dest_size, data, dest_size);
	_free_list_bit_set(_list, _list->_next_free);
	_list->_length++;
	*index = _list->_next_free;

	// Find the next free slot
	for(size_t i=_list->_next_free; i<_list->_capacity; ++i) {
		if (_free_list_bit_get(_list, i) == 0) {
			_list->_next_free = i;
			break;
		}
	}

	return (void*)dest;
free_list_insert_fail:
	index = NULL;
	return NULL;
}

void _free_list_remove(free_list_t* list, size_t index, size_t count) {
	// Error check
	if (!list) { return; }
	if ((index + count) > list->_length) { return; }

	// Flag spot as free
	list->_next_free = umin(list->_next_free, index);
	for(size_t i=0; i<count; ++i) {
		_free_list_bit_clr(list, index+i);
	}

	// Decrement length
	list->_length -= count;
}

free_list_it_t* _free_list_it(free_list_t* list) {
	// Error check
	if (!list || list->_length == 0) { return NULL; }

	// Construct iterator
	size_t buffer_size = sizeof(free_list_it_t);
	free_list_it_t* it = MARS_CALLOC(buffer_size, 1);
	if (!it) { 
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate buffer_t iterator!");
		return NULL; 
	}
	it->index = SIZE_MAX;
	it->_list = list;

	// Find first valid entry in list
	it = _free_list_it_next(it);
	return it;
}

free_list_it_t* _free_list_it_next(free_list_it_t* it) {
	// Error check
	if (!it) { return NULL; }

	// Find the next valid position in the buffer
	free_list_t* _list = it->_list;
	do {
		// Increment index
		it->index++;

		// Reached the end of the array
		if (it->index >= _list->_capacity) {
			break;
		}

		// Evaluate position
		if (_free_list_bit_get(_list, it->index)) {
			it->data = (void*)(_free_list_pos(_list, it->index));
			return it;
		}
	} while(1);

	MARS_FREE(it);
	return NULL;
}