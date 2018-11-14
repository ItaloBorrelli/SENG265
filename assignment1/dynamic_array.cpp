#include <iostream>										//-
#include <string.h>										//-
#include "dynamic_array.h"									//-
												//-
using namespace std;										//-
												//-
// ********** public functions **********							//-
												//-
Dynamic_array::Dynamic_array() {								//-
	head_p = NULL;										//-
	size = 0;										//-
}												//-
												//-
Dynamic_array::Dynamic_array(Dynamic_array & d) {						//-
	head_p = copy_blocks(d.head_p);

	size = d.size;
}												//-
												//-
Dynamic_array &Dynamic_array::operator=(Dynamic_array & d) {					//-
	// delete original contents
	remove(0, size);

	// copy d contents and size
	head_p = copy_blocks(d.head_p);
	size = d.size;

	// return object reference
	return *this;										//-
}												//-
												//-
Dynamic_array::~Dynamic_array() {								//-
	remove(0, size);
}												//-
												//-
void Dynamic_array::print_state(void) {								//-
	cout << "size:" << size << endl;							//-
	for (Block * p = head_p; p != NULL; p = p->next_p) {					//-
		cout << "\tsize:" << p->size << " elements:";					//-
		for (int i = 0; i < p->size; i++) {						//-
			cout << p->a[i] << ' ';							//-
		}										//-
		cout << endl;									//-
	}											//-
}												//-
												//-
int Dynamic_array::get_size(void) {								//-
	return size;
}												//-
												//-
int& Dynamic_array::operator[](int i) {								//-
	// check i for range error								//-
	if (i < 0 || i >= size) {								//-
		throw Subscript_range_exception();						//-
	}											//-
												//-
	// find target block and index								//-
	Block_position position = find_block(i);						//-
												//-
	// return element at index i								//-
	return position.block_p->a[position.i];							//-
}												//-
												//-
void Dynamic_array::insert(int x, int i) {							//-
	// case 1: range error									//-
	if (i < 0 || i > size) {								//-
		throw Subscript_range_exception();						//-
	}											//-
												//-
	// case 2: empty array									//-
	if (size == 0) {									//-
		// create linked list consisting of a single new block				//-
		Block * new_block_p = new Block;						//-
		new_block_p->size = 1;								//-
		new_block_p->a[0] = x;								//-
		new_block_p->next_p = NULL;							//-
												//-
		// insert new block								//-
		insert_blocks(NULL, new_block_p);						//-
												//-
		// update total size								//-
		size++;										//-
												//-
		return;										//-
	}											//-
												//-
	// find target block and index								//-
	Block_position position = find_block(i);						//-
												//-
	// case 3: non-empty array; new block not needed					//-
	if (position.block_p->size < BLOCK_SIZE) {						//-
		// shift block array right							//-
		for (int j = position.block_p->size; j > position.i; j--) {			//-
			position.block_p->a[j] = position.block_p->a[j-1];			//-
		}										//-
												//-
		// assign x									//-
		position.block_p->a[position.i] = x;						//-
												//-
		// update array and block size							//-
		size++;										//-
		position.block_p->size++;							//-
												//-
	// case 4: non-empty array; new block needed						//-
	} else {										//-
		// create new block								//-
		Block * new_block_p = new Block;						//-
		new_block_p->size = 1;								//-
		new_block_p->next_p = NULL;							//-
												//-
		// case 4.a: insert x into old block						//-
		if (position.i < BLOCK_SIZE) {							//-
			// copy last array element to new block					//-
			new_block_p->a[0] = position.block_p->a[BLOCK_SIZE-1];			//-
												//-
			// shift old block array right						//-
			for (int j = position.block_p->size-1; j > position.i; j--) {		//-
				position.block_p->a[j] = position.block_p->a[j-1];		//-
			}									//-
												//-
			// assign x into old block						//-
			position.block_p->a[position.i] = x;					//-
												//-
		// case 4.b: insert x into new block						//-
		} else {									//-
			new_block_p->a[0] = x;							//-
		}										//-
												//-
		// update total size								//-
		size++;										//-
												//-
		// insert new block into linked list						//-
		insert_blocks(position.block_p, new_block_p);					//-
	}											//-
}												//-
												//-
void Dynamic_array::insert(Dynamic_array &p, int i) {						//-
	// case 1: range error									//-
	if (i < 0 || i > size) {
		// cout << "DB. case 1" << endl;
		throw Subscript_range_exception();
	}

	// case 2: parameter array empty							//-
	if (p.size == 0) {
		// cout << "DB. case 2" << endl;
		return;
	}

	// case 3: array empty									//-
	if (size == 0) {
		// cout << "DB. case 3" << endl;
		// copy blocks from p
		Block * copy_p = copy_blocks(p.head_p);

		// insert copied blocks
		insert_blocks(head_p, copy_p);

		// update size
		size += p.size;

		// return
		return;
	}

	// find target block and index								//-
	Block_position position = find_block(i);						//-

	// case 4: array non-empty; new blocks not needed					//-
	if (position.block_p->size + p.size <= BLOCK_SIZE) {
		// cout << "DB. case 4" << endl;
		// shift block array to the right
		for (int j = position.block_p->size + p.size - 1; j > position.i; j--) {
			position.block_p->a[j] = position.block_p->a[j-p.size];
		}

		// copy p to new position
		for (int j = 0; j < p.size; j++) {
			position.block_p->a[position.i+j] = p[j];
		}

		// update sizes
		position.block_p->size += p.size;
		size += p.size;

	// case 5: array non-empty; new blocks needed						//-
	} else {
		// copy p									//-
		Block * copy_p = copy_blocks(p.head_p);						//-

		// case 5.a: insert position at start of block					//-
		if (position.i == 0) {
			// cout << "DB. case 5a" << endl;
			insert_blocks(position.pre_block_p, copy_p);

		// case 5.b: insert position at middle of block					//-
		} else if (position.i < position.block_p->size) {
			// cout << "DB. case 5b" << endl;
			// allocate and initialize new block
			Block * new_block_p = new Block;
			new_block_p->size = position.block_p->size - position.i;
			new_block_p->next_p = NULL;

			// copy elements to new block
			int k = 0;
			for (int j = position.i; j < position.block_p->size; j++) {
				new_block_p->a[k] = position.block_p->a[position.i+k];
				k++;
			}

			// insert new block
			insert_blocks(position.block_p, new_block_p);

			// insert blocks copied from parameter array
			insert_blocks(position.block_p, copy_p);

			// update block size
			position.block_p->size = position.i;

		// case 5.c: insert position after end of block					//-
		} else {
			// cout << "DB. case 5c" << endl;
			insert_blocks(position.block_p, copy_p);
		}

		// update total size								//-
		size += p.size;
	}
}												//-
												//-
void Dynamic_array::remove(int i) {								//-
	// case 1: range error									//-
	if (i < 0 || i > size-1) {
		throw Subscript_range_exception();
	}

	// find target block and index								//-
	Block_position position = find_block(i);

	// case 2: block size == 1								//-
	if (position.block_p->size == 1) {
		remove_blocks(position.pre_block_p, position.block_p, position.block_p);

	// case 3: block size > 1								//-
	} else {
		for (int j = i; j < position.block_p->size-1; j++) {
			position.block_p->a[j] = position.block_p->a[j+1];
		}

		position.block_p->size--;
	}

	// update total size									//-
	size--;
}												//-
												//-
void Dynamic_array::remove(int start, int end) {						//-
	// case 1: range error
	if (start < 0 || start > end || end > size) {
		throw Subscript_range_exception();
	}

	// case 2: empty range
	if (start == end) {
		return;
	}

	// update total size
	size -= end - start;

	// locate block for start element
	Block_position start_position = find_block(start);

	// locate block for the element before end element
	Block_position end_position = find_block(end-1);

	// case 3: start block same as end block
	if (start_position.block_p == end_position.block_p) {
		// case 3.a: block deleted
		if (start_position.i == 0 && end_position.i == start_position.block_p->size-1) {

			remove_blocks(start_position.pre_block_p,
			start_position.block_p, end_position.block_p);

		// case 3.b: block retained
		} else {
			// use a loop to shift block elements left
			int delta = end - start;
			for (int i = start_position.i;
			i < start_position.block_p->size - delta; i++) {
				start_position.block_p->a[i] =
				start_position.block_p->a[i + delta];
			}

			// update block size
			start_position.block_p->size -= delta;
		}

	// case 4: start block not same as end block
	} else {
		// case 4.a: start block deleted
		if (start_position.i == 0) {
			// case 4.a.1: end block deleted					
			if (end_position.i == end_position.block_p->size-1) {
				remove_blocks(start_position.pre_block_p,
				start_position.block_p, end_position.block_p);

			// case 4.a.2: end block retained					
			} else {
				remove_blocks(start_position.pre_block_p,
				start_position.block_p, end_position.pre_block_p);

				// update end block
				int delta = end_position.i + 1;
				for (int i = 0; i < end_position.block_p->size-delta; i++) {
					end_position.block_p->a[i] = end_position.block_p->a[i+delta];
				}
				end_position.block_p->size -= end_position.i + 1;
			}

		// case 4.b: start block retained
		} else {
			// case 4.b.1: end block deleted					
			if (end_position.i == end_position.block_p->size-1) {
				remove_blocks(start_position.block_p,
				start_position.block_p->next_p, end_position.block_p);

				// update start block
				start_position.block_p->size = start_position.i;

			// case 4.b.2: end block retained					
			} else {
				if (start_position.block_p->next_p != end_position.block_p) {
					remove_blocks(start_position.block_p,
					start_position.block_p->next_p, end_position.pre_block_p);
				}

				// update start block
				start_position.block_p->size = start_position.i;

				// update end block
				int delta = end_position.i + 1;
				for (int i = 0; i < end_position.block_p->size-delta; i++) {
					end_position.block_p->a[i] = end_position.block_p->a[i+delta];
				}
				end_position.block_p->size -= end_position.i + 1;
			}
		}
	}

}												//-
												//-
// ********** private functions **********							//-
												//-
// purpose											//-
//	return b where										//-
//		if i < size									//-
//			b.block_p->a[b.i] is the ith element overall				//-
//		else										//-
//			b.block_p->a[b.i-1] is the i-1st element overall			//-
//												//-
//		if b.block_p == head_p								//-
//			b.pre_block_p is NULL							//-
//		else										//-
//			b.pre_block_p points to block preceding b.block_p			//-
// preconditions										//-
//	i in [1..size]										//-
Dynamic_array::Block_position Dynamic_array::find_block(int i) {				//-
	Block_position position;								//-
												//-
	// scan Block list									//-
	position.i = i;										//-
	position.pre_block_p = NULL;								//-
	for (position.block_p = head_p;								//-
	position.block_p != NULL;								//-
	position.block_p = position.block_p->next_p) {						//-
		// found in current block							//-
		if (position.i < position.block_p->size) {					//-
			break;									//-
		}										//-
												//-
		// special case: insert just after last element					//-
		if (i == size && position.block_p->next_p == NULL) {				//-
			break;									//-
		}										//-
												//-
		// not found yet: advance							//-
		position.pre_block_p = position.block_p;					//-
		position.i -= position.block_p->size;						//-
	}											//-
												//-
	return position;									//-
}												//-
												//-
// purpose											//-
//	create a new linked list which is a copy of the list pointed to p			//-
//	return a pointer to the head of the new linked list					//-
// preconditions										//-
//	p is the head of a possibly empty linked list of blocks					//-
Dynamic_array::Block * Dynamic_array::copy_blocks(Block * p) {					//-
	Block * new_head_p = NULL;								//-
	Block * new_p;										//-
	while (p != NULL) {									//-
		// allocate and link in new block						//-
		if (new_head_p == NULL) {							//-
			new_p = new Block;							//-
			new_head_p = new_p;							//-
		} else {									//-
			new_p->next_p = new Block;						//-
			new_p = new_p->next_p;							//-
		}										//-
												//-
		// copy the elements								//-
		new_p->size = p->size;								//-
		for (int i = 0; i < p->size; i++) {						//-
			new_p->a[i] = p->a[i];							//-
		}										//-
												//-
		// advance									//-
		p = p->next_p;									//-
	}											//-
												//-
	// terminate new list									//-
	if (new_head_p != NULL) {								//-
		new_p->next_p = NULL;								//-
	}											//-
												//-
	return new_head_p;									//-
}												//-
												//-
// purpose											//-
//	insert the list headed by src_p into the list headed by head_p				//-
//	if dst_p is NULL									//-
//		insert the list at the start of the list headed by head_p			//-
//	else											//-
//		insert the list just after block dst_p						//-
// preconditions										//-
//	list headed by src_p is non-empty							//-
//	list headed by src_p has no blocks in common with the list headed by head_p		//-
void Dynamic_array::insert_blocks(Block * dst_p, Block * src_p) {				//-
	// find the last block in the list headed by src_p					//-
	Block * p = src_p;									//-
	while (p->next_p != NULL) {								//-
		p = p->next_p;									//-
	}											//-
												//-
	// insert at head									//-
	if (dst_p == NULL) { // insert at head							//-
		p->next_p = head_p;								//-
		head_p = src_p;									//-
												//-
	// insert after dst_p									//-
	} else { // insert after dst_p								//-
		p->next_p = dst_p->next_p;							//-
		dst_p->next_p = src_p;								//-
	}											//-
}												//-
												//-
// purpose											//-
//	remove the blocks pointed to by start_p and end_p, and all the blocks between		//-
// preconditions										//-
//	start_p and end_p point to blocks in the list headed by head_p				//-
//	end_p points to either the same block as start_p or a block to its right		//-
//	if start_p == head_p									//-
//		pre_start_p == NULL								//-
//	else											//-
//		pre_start_p points to the block immediately preceding start_p			//-
//												//-
void Dynamic_array::remove_blocks(Block * pre_start_p, Block * start_p, Block * end_p) {	//-
	// release the blocks									//-
	Block * p;										//-
	while (1) {										//-
		// release start_p								//-
		p = start_p->next_p;								//-
		delete start_p;									//-
												//-
		// advance									//-
		if (start_p == end_p) {								//-
			break;									//-
		} else {									//-
			start_p = p;								//-
		}										//-
	}											//-
												//-
	// link left and right sublists								//-
	if (pre_start_p == NULL) {								//-
		head_p = p;									//-
	} else {										//-
		pre_start_p->next_p = p;							//-
	}											//-
}												//-
