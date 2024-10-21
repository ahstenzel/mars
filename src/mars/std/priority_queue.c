#include "mars/std/priority_queue.h"

size_t _priority_queue_size(size_t element_size, size_t capacity) {
	size_t c = element_size * capacity;
	if (c / capacity != element_size) { return 0; }
	size_t o = sizeof(priority_queue_value_t) * capacity;
	if (c > SIZE_MAX - o) { return 0; }
	return umax(sizeof(priority_queue_t), offsetof(priority_queue_t, _buffer) + o + c);
}

priority_queue_t* _priority_queue_factory(size_t element_size, size_t capacity) {
	size_t buffer_size = _priority_queue_size(element_size, capacity);
	if (buffer_size == 0) { return NULL; }
	priority_queue_t* qu = MARS_CALLOC(1, buffer_size);
	if (!qu) { return NULL; }
	qu->_capacity = capacity;
	qu->_element_size = element_size;
	return qu;
}

priority_queue_t* _priority_queue_resize(priority_queue_t* qu, size_t new_capacity) {
	// Calculate new capacity
	if (new_capacity == 0) {
		size_t c = MARS_NEXT_POW2(qu->_capacity + 1);
		new_capacity = umin(c, PRIORITY_QUEUE_MAX_CAPACITY);
	}
	if (new_capacity > PRIORITY_QUEUE_MAX_CAPACITY || new_capacity < qu->_length) { return NULL; }

	// Create new priority queue & copy data to it
	priority_queue_t* new_qu = _priority_queue_factory(qu->_element_size, new_capacity);
	if (!new_qu) { return NULL; }
	size_t value_dest_size = qu->_capacity * sizeof(priority_queue_value_t);
	memcpy_s(_priority_queue_value_pos(new_qu, 0), value_dest_size, _priority_queue_value_pos(qu, 0), value_dest_size);
	size_t data_dest_size = qu->_capacity * qu->_element_size;
	memcpy_s(_priority_queue_data_pos(new_qu, 0), data_dest_size, _priority_queue_data_pos(qu, 0), data_dest_size);

	new_qu->_length = qu->_length;
	MARS_FREE(qu);
	_priority_queue_sort(new_qu);
	return new_qu;
}

void* _priority_queue_insert(priority_queue_t** qu, priority_queue_value_t value, void* data) {
	// Error check
	if (!qu || !(*qu)) { return NULL; }
	priority_queue_t* _qu = *qu;

	// Resize container
	if (_qu->_length >= _qu->_capacity) {
		priority_queue_t* temp = _priority_queue_resize(_qu, 0);
		if (!temp) { return NULL; }
		(*qu) = temp;
		_qu = temp;
	}

	// Copy value to end
	void* value_dest = (void*)(_priority_queue_value_pos(_qu, _qu->_length));
	size_t value_dest_size = sizeof(priority_queue_value_t);
	memcpy_s(value_dest, value_dest_size, &value, value_dest_size);

	// Copy data to end
	void* data_dest = (void*)(_priority_queue_data_pos(_qu, _qu->_length));
	size_t data_dest_size = _qu->_element_size;
	memcpy_s(data_dest, data_dest_size, data, data_dest_size);
	_qu->_length++;

	// Sort list
	_priority_queue_sort(_qu);
	return _priority_queue_find(_qu, value, data);
}

void _priority_queue_remove(priority_queue_t* qu, size_t count) {
	// Error check
	if (!qu || qu->_length < count) { return; }

	// Decrement length
	qu->_length -= count;
}

void _priority_queue_remove_value(priority_queue_t* qu, priority_queue_value_t value, void* data) {
	// Error check
	if (!qu || qu->_length == 0) { return; }

	size_t it = _priority_queue_find_index(qu, value, data);
	if (it < qu->_capacity) {
		// Shift value block
		void* dest = (void*)(_priority_queue_value_pos(qu, it));
		void* src = (void*)(_priority_queue_value_pos(qu, it + 1));
		size_t move_size = ((qu->_capacity - it - 1) * sizeof(priority_queue_value_t)) + (qu->_capacity * qu->_element_size);
		memmove_s(dest, move_size, src, move_size);

		// Shift data block
		dest = (void*)(_priority_queue_data_pos(qu, it));
		src = (void*)(_priority_queue_data_pos(qu, it + 1));
		move_size = (qu->_capacity - it - 1) * qu->_element_size;
		memmove_s(dest, move_size, src, move_size);

		qu->_length--;
	}
}

void _priority_queue_sort(priority_queue_t* qu) {
	// Insertion sort
	size_t dest_elem_size = qu->_element_size;
	size_t dest_value_size = sizeof(priority_queue_value_t);
	void* tmp_data = MARS_MALLOC(qu->_element_size);
	priority_queue_value_t tmp_value = 0;
	for(size_t i=1; i<qu->_length; ++i) {
		// Copy element to temp buffer
		memcpy_s(tmp_data, dest_elem_size, _priority_queue_data_pos(qu, i), dest_elem_size);
		tmp_value = _priority_queue_value(qu, i);

		// Shift elements
		size_t j=i;
		for(; j > 0 && _priority_queue_value(qu, j-1) > tmp_value; --j) {
			memcpy_s(_priority_queue_data_pos(qu, j), dest_elem_size, _priority_queue_data_pos(qu, j-1), dest_elem_size);
			memcpy_s(_priority_queue_value_pos(qu, j), dest_value_size, _priority_queue_value_pos(qu, j-1), dest_value_size);
		}

		// Re-insert temp
		memcpy_s(_priority_queue_data_pos(qu, j), dest_elem_size, tmp_data, dest_elem_size);
		memcpy_s(_priority_queue_value_pos(qu, j), dest_value_size, &tmp_value, dest_value_size);
	}
	// Free temp buffer
	MARS_FREE(tmp_data);
}

size_t _priority_queue_find_index(priority_queue_t* qu, priority_queue_value_t value, void* data) {
	// Binary search
	size_t hi = qu->_length - 1;
	size_t lo = 0;
	size_t md = 0;
	priority_queue_value_t hi_val = 0;
	priority_queue_value_t lo_val = 0;
	priority_queue_value_t md_val = 0;
	size_t r = qu->_capacity;
	do {
		md = lo + (hi - lo) / 2;
		hi_val = _priority_queue_value(qu, hi);
		lo_val = _priority_queue_value(qu, lo);
		md_val = _priority_queue_value(qu, md);
		if (md_val == value) {
			// Value found
			r = md;
			break;
		}
		else if (md_val < value) {
			// Midpoint too low, move window up
			lo = md + 1;
		}
		else {
			// Midpoint too high, move window down
			hi = md - 1;
		}
	}
	while (hi != lo);

	// Move to the start of a sequential run
	while(r < (qu->_capacity - 1) && _priority_queue_value(qu, r+1) == _priority_queue_value(qu, r)) {
		r++;
	}

	// Check the run for matching data
	if (data) {
		do {
			if (memcmp(_priority_queue_data_pos(qu, r), data, qu->_element_size) == 0) {
				break;
			}
			if (r == 0 || _priority_queue_value(qu, r) != value) {
				return qu->_capacity;
			}
			r--;
		} while(1);
	}
	return r;
}

void* _priority_queue_find(priority_queue_t* qu, priority_queue_value_t value, void* data) {
	// Error check
	if (!qu || qu->_length == 0) { return NULL; }

	// Binary search
	size_t it = _priority_queue_find_index(qu, value, data);
	return (it < qu->_capacity) ? _priority_queue_data_pos(qu, it) : NULL;
}

priority_queue_it_t* _priority_queue_it(priority_queue_t* qu, bool begin) {
	// Error check
	if (!qu || qu->_length == 0) { return NULL; }

	// Construct iterator
	size_t buffer_size = sizeof(priority_queue_it_t);
	priority_queue_it_t* it = MARS_CALLOC(1, buffer_size);
	if (!it) { return NULL; }

	// Find first valid entry in map
	it->_qu = qu;
	if (begin) { it->_index = (qu->_length - 1); }
	else { it->_index = 0; }
	it->data = _priority_queue_data_pos(qu, it->_index);
	it->value = _priority_queue_value(qu, it->_index);

	return it;
}

priority_queue_it_t* _priority_queue_it_value(priority_queue_t* qu, priority_queue_value_t value) {
	// Error check
	if (!qu || qu->_length == 0) { return NULL; }

	// Construct iterator
	size_t buffer_size = sizeof(priority_queue_it_t);
	priority_queue_it_t* it = MARS_CALLOC(1, buffer_size);
	if (!it) { return NULL; }
	it->_index = _priority_queue_find_index(qu, value, NULL);
	if (it->_index == qu->_capacity) {
		MARS_FREE(it);
		return NULL;
	}
	it->_qu = qu;
	return it;
}

priority_queue_it_t* _priority_queue_it_next(priority_queue_it_t* it) {
	// Error check
	if (!it) { return NULL; }

	// Find the next valid position in the buffer
	priority_queue_t* _qu = it->_qu;
	if (it->_index > 0) {
		// Record next positions data
		it->_index--;
		it->data = _priority_queue_data_pos(_qu, it->_index);
		it->value = _priority_queue_value(_qu, it->_index);
		return it;
	}
	else {
		// End reached, invalidate iterator
		MARS_FREE(it);
		return NULL;
	}
}

priority_queue_it_t* _priority_queue_it_prev(priority_queue_it_t* it) {
	// Error check
	if (!it) { return NULL; }

	// Find the next valid position in the buffer
	priority_queue_t* _qu = it->_qu;
	if (it->_index < _qu->_length) {
		// Record next positions data
		it->_index++;
		it->data = _priority_queue_data_pos(_qu, it->_index);
		it->value = _priority_queue_value(_qu, it->_index);
		return it;
	}
	else {
		// End reached, invalidate iterator
		MARS_FREE(it);
		return NULL;
	}
}