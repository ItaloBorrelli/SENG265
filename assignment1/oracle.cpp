#include <iostream>
#include <string.h>

#include "oracle.h"

using namespace std;

namespace oracle {
Dynamic_array::Dynamic_array() {
	// set sizes
	size = 0;
	allocated_size = 0;

	// no dynamic memory at this point
	array = NULL;
}

Dynamic_array::Dynamic_array(Dynamic_array &a) {
	array = new int[a.allocated_size];

	// set sizes
	size = a.get_size();
	allocated_size = a.allocated_size;

	// copy a to array
	for (int i = 0; i < size; i++) {
		array[i] = a[i];
	}
}

Dynamic_array &Dynamic_array::operator=(Dynamic_array &a) {
	// get dynamic memory
	int *array0 = new int[a.allocated_size];

	// set sizes
	size = a.get_size();
	allocated_size = a.allocated_size;

	// copy array to array0
	for (int i = 0; i < size; i++) {
		array0[i] = a[i];
	}

	// switch dynamic memory
	delete [] array;
	array = array0;

	// return self-reference
	return *this;
}

Dynamic_array::~Dynamic_array() {
	delete [] array;
}

int Dynamic_array::get_size(void) {
	return size;
}

void Dynamic_array::print_state(void) {
	cout << "oracle::print_state: NOT IMPLEMENTED" << endl;
}

int& Dynamic_array::operator[](int i) {
	if (i >= size)
		throw Subscript_range_exception();

	return array[i];
}

void Dynamic_array::insert(int x, int i) {
	// check i for range
	if (i < 0 || i > size) {
		throw Subscript_range_exception();
	}

	// array has space available
	if (size < allocated_size) {
		// shift array[i..size-1] right by one position
		shift_right(i, size, 1);

		// copy x
		array[i] = x;

		// increment size
		size++;
	} else { // array does not have space available
		// allocate a larger array
		int new_allocated_size = allocated_size + BLOCK_SIZE;
		int *array0 = new int[new_allocated_size];
		allocated_size = new_allocated_size;

		// copy array[0..i-1]
		for (int j = 0; j < i; j++) {
			array0[j] = array[j];
		}

		// copy x
		array0[i] = x;

		// copy array[i..size-1]
		for (int j = i; j < size; j++) {
			array0[j+1] = array[j];
		}

		// increment size
		size++;

		// switch dynamic memory
		delete [] array;
		array = array0;
	}
}

void Dynamic_array::insert(Dynamic_array &a, int i) {
	// check i for range
	if (i < 0 || i > size) {
		throw Subscript_range_exception();
	}

	int new_size = size + a.get_size();

	if (new_size <= allocated_size) { // array has space available
		// shift array[i..size-1] right
		shift_right(i, size, a.get_size());

		// copy a
		for (int j = 0; j < a.get_size(); j++) {
			array[i+j] = a[j];
		}

		// increment size
		size += a.get_size();
	} else { // array does not have space available
		// allocate a larger array
		int new_allocated_size;
		if (new_size % BLOCK_SIZE == 0) {
			new_allocated_size = new_size;
		} else {
			int full_blocks = new_size / BLOCK_SIZE;
			new_allocated_size = (full_blocks+1) * BLOCK_SIZE;
		}
		int *array0 = new int[new_allocated_size];

		// copy array[0..i-1]
		for (int j = 0; j < i; j++) {
			array0[j] = array[j];
		}

		// copy a
		for (int j = 0; j < a.get_size(); j++) {
			array0[i+j] = a[j];
		}

		// copy array[i..size-1]
		for (int j = i; j < size; j++) {
			array0[j+a.get_size()] = array[j];
		}

		// update sizes
		allocated_size = new_allocated_size;
		size = new_size;

		// switch dynamic memory
		delete [] array;
		array = array0;
	}
}

void Dynamic_array::remove(int i) {
	// check i for range
	if (i < 0 || i >= size) {
		Subscript_range_exception();
	}

	int new_size = size - 1;
	if (new_size > allocated_size - BLOCK_SIZE) { // allocated_size unchanged
		// shift array[i..size-1] left by one position
		shift_left(i+1, size, 1);

		// update size
		size = new_size;
	} else { // allocated_size must be changed
		// allocate a smaller array
		int new_allocated_size = allocated_size - BLOCK_SIZE;
		int *array0 = new int[new_allocated_size];
		allocated_size = new_allocated_size;

		// copy array[0..i-1]
		for (int j = 0; j < i; j++) {
			array0[j] = array[j];
		}

		// copy array[i+1..size-1]
		for (int j = i+1; j < size; j++) {
			array0[j-1] = array[j];
		}

		// decrement size
		size--;

		// switch dynamic memory
		delete [] array;
		array = array0;
	}
}

void Dynamic_array::remove(int start, int end) {
	// check start, end
	if (start < 0 || start > end || end > size) {
		throw Subscript_range_exception();
	}

	int delta = end - start;
	int new_size = size - delta;

	if (new_size > allocated_size - BLOCK_SIZE) { // allocated_size unchanged
		// shift array[i..size-1] left
		shift_left(end, size, delta);

		// update size
		size = new_size;
	} else { // allocated_size must be changed
		// allocate a smaller array
		int new_allocated_size;
		if (new_size % BLOCK_SIZE == 0) {
			new_allocated_size = new_size;
		} else {
			int full_blocks = new_size / BLOCK_SIZE;
			new_allocated_size = (full_blocks+1) * BLOCK_SIZE;
		}
		int *array0 = new int[new_allocated_size];

		// copy array[0..start-1]
		for (int j = 0; j < start; j++) {
			array0[j] = array[j];
		}

		// copy array[end..size-1]
		for (int j = end; j < size; j++) {
			array0[j-delta] = array[j];
		}

		// update sizes
		allocated_size = new_allocated_size;
		size = new_size;

		// switch dynamic memory
		delete [] array;
		array = array0;
	}
}

void Dynamic_array::shift_left(int start, int end, int delta) {
	for (int i = start; i < end; i++) {
		array[i-delta] = array[i];
	}
}

void Dynamic_array::shift_right(int start, int end, int delta) {
	for (int i = end-1; i >= start; i--) {
		array[i+delta] = array[i];
	}
}
}
