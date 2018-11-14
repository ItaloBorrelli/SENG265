#include <iostream>
#include <stdlib.h>

#include "dynamic_array.h"
#include "oracle.h"

using namespace std;

void generate_cut(Dynamic_array &cut, int start, int delta, int count) {
	for (int i = 0; i < count; i++) {
		cut.insert(start, i);
		start += delta;
	}
}

void generate_oracle(oracle::Dynamic_array &orc, int start, int delta, int count) {
	for (int i = 0; i < count; i++) {
		orc.insert(start, i);
		start += delta;
	}
}

void print_state(Dynamic_array &cut, oracle::Dynamic_array &orc) {
	cout << "***** cut" << endl;
	cout << "size: " << cut.get_size() << endl;
	for (int i = 0; i < cut.get_size(); i++) {
		cout << cut[i] << " ";
		if (i > 50) { // avoid lengthy output
			cout << " ...";
			break;
		}
	}
	cout << endl;

	cout << "***** oracle" << endl;
	cout << "size: " << orc.get_size() << endl;
	for (int i = 0; i < orc.get_size(); i++) {
		cout << orc[i] << " ";
		if (i > 50) { // avoid lengthy output
			cout << " ...";
			break;
		}
	}
	cout << endl;
}

int compare_exceptions(Dynamic_array &cut, oracle::Dynamic_array &orc) {
	{
	// ********** operator[]
	int indexes[] = {orc.get_size(), orc.get_size()+1000};
	int N = sizeof(indexes)/sizeof(int);
	for (int i = 0; i < N; i++) {
		int caught = 0;
		try {
			cut[indexes[i]];
		} catch (Dynamic_array::Subscript_range_exception) {
			caught = 1;
		}
		if (!caught) {
			cout << "operator[]: uncaught Subscript_range_exception at: ";
			cout << indexes[i] << endl;
			return 0;
		}
	}
	}

	{
	// ********** insert(int,int)
	int indexes[] = {-1000, -1, orc.get_size()+1, orc.get_size()+1000};
	int N = sizeof(indexes)/sizeof(int);
	for (int i = 0; i < N; i++) {
		int caught = 0;
		try {
			cut.insert(0, indexes[i]);
		} catch (Dynamic_array::Subscript_range_exception) {
			caught = 1;
		}
		if (!caught) {
			cout << "insert(int,int): uncaught Subscript_range_exception at: ";
			cout << indexes[i] << endl;
			return 0;
		}
	}
	}

	{
	// ********** insert(Dynamic_array&,int)
	int indexes[] = {-1000, -1, orc.get_size()+1, orc.get_size()+1000};
	int N = sizeof(indexes)/sizeof(int);
	Dynamic_array a;
	for (int i = 0; i < N; i++) {
		int caught = 0;
		try {
			cut.insert(a, indexes[i]);
		} catch (Dynamic_array::Subscript_range_exception) {
			caught = 1;
		}
		if (!caught) {
			cout << "insert(Dynamic_array&,int): uncaught Subscript_range_exception at: ";
			cout << indexes[i] << endl;
			return 0;
		}
	}
	}

	{
	// ********** remove(int)
	int indexes[] = {-1000, -1, orc.get_size(), orc.get_size()+1000};
	int N = sizeof(indexes)/sizeof(int);
	for (int i = 0; i < N; i++) {
		int caught = 0;
		try {
			cut.remove(indexes[i]);
		} catch (Dynamic_array::Subscript_range_exception) {
			caught = 1;
		}
		if (!caught) {
			cout << "remove(int): uncaught Subscript_range_exception at: ";
			cout << indexes[i] << endl;
			return 0;
		}
	}
	}

	{
	// ********** remove(int,int)
	// start out of range
	int start_indexes[] = {-1000, -1, orc.get_size()+1, orc.get_size()+1000};
	int N = sizeof(start_indexes)/sizeof(int);
	for (int i = 0; i < N; i++) {
		int caught = 0;
		try {
			cut.remove(start_indexes[i], orc.get_size());
		} catch (Dynamic_array::Subscript_range_exception) {
			caught = 1;
		}
		if (!caught) {
			cout << "remove(int,int): uncaught Subscript_range_exception at: ";
			cout << start_indexes[i] << "," << orc.get_size() << endl;
			return 0;
		}
	}

	// end out of range
	int end_indexes[] = {orc.get_size()+1, orc.get_size()+1000};
	N = sizeof(end_indexes)/sizeof(int);
	for (int i = 0; i < N; i++) {
		int caught = 0;
		try {
			cut.remove(0, end_indexes[i]);
		} catch (Dynamic_array::Subscript_range_exception) {
			caught = 1;
		}
		if (!caught) {
			cout << "remove(int,int): uncaught Subscript_range_exception at: ";
			cout << end_indexes[i] << "," << orc.get_size() << endl;
			return 0;
		}
	}

	// special case: 0 <= end < start < size
	int caught = 0;
	try {
		cut.remove(1, 0);
	} catch (Dynamic_array::Subscript_range_exception) {
		caught = 1;
	}
	if (!caught) {
		cout << "remove(int,int): uncaught Subscript_range_exception at: 1,0" << endl;
		return 0;
	}
	}

	return 1; // no failures detected
}

int compare_content(Dynamic_array &cut, oracle::Dynamic_array &orc) {
	// check size
	if (cut.get_size() != orc.get_size()) {
		cout << "ERROR. ";
		cout << "size. cut: " << cut.get_size();
		cout << " orc:" << orc.get_size() << endl;

		print_state(cut, orc);
		return 0;
	}

	// check operator[]
	for (int i = 0; i < orc.get_size(); i++) {
		if (cut[i] != orc[i]) {
			cout << "ERROR. ";
			cout << "cut[" << i << "]:" << cut[i];
			cout << " orc[" << i << "]:" << orc[i] << endl;

			print_state(cut, orc);
			return 0;
		}
	}

	return 1;
}

void run_tests(Dynamic_array &cut, oracle::Dynamic_array &orc) {
cout << "DB 0 run_tests" << endl;
	{
	// ***** copy constructor, operator=
	// copy constructor
	Dynamic_array cut0(cut);
	oracle::Dynamic_array orc0(orc);

	cut0.insert(1, 0);
	orc0.insert(1, 0);

	compare_content(cut, orc);
	compare_exceptions(cut, orc);
	compare_content(cut0, orc0);
	compare_exceptions(cut0, orc0);

	// operator=
	Dynamic_array cut1;
	oracle::Dynamic_array orc1;

	cut1 = cut0;
	orc1 = orc0;

	cut1.remove(0);
	orc1.remove(0);

	compare_content(cut0, orc0);
	compare_exceptions(cut0, orc0);
	compare_content(cut1, orc1);
	compare_exceptions(cut1, orc1);
	} // array size: unchanged

cout << "DB 1 run_tests" << endl;
	{
	// ***** operator[]
	if (orc.get_size() > 0) {
		int size = cut.get_size();

		cut[0] = -cut[0];
		cut[size/2] = -cut[size/2];
		cut[size-1] = -cut[size-1];

		orc[0] = -orc[0];
		orc[size/2] = -orc[size/2];
		orc[size-1] = -orc[size-1];

		compare_content(cut, orc);
		compare_exceptions(cut, orc);
	}
	} // array size: unchanged

cout << "DB 2 run_tests" << endl;
	{
	// ***** insert(int,int)
	int size = cut.get_size();

	cut.insert(1,size);
	cut.insert(3,1);
	cut.insert(5,0);

	orc.insert(1,size);
	orc.insert(3,1);
	orc.insert(5,0);

	compare_content(cut, orc);
	compare_exceptions(cut, orc);
	} // array size: larger by 3

cout << "DB 3 run_tests" << endl;
	{
	// ***** remove(int)
	int size = cut.get_size();

	cut.remove(size-1);
	cut.remove(1);
	cut.remove(0);

	orc.remove(size-1);
	orc.remove(1);
	orc.remove(0);

	compare_content(cut, orc);
	compare_exceptions(cut, orc);
	} // array size: smaller by 3; same as original

cout << "DB 4 run_tests" << endl;
	{
	// ***** insert(Dynamic_array&,int)
	int size = orc.get_size();

	// cut
	Dynamic_array cut0;
	generate_cut(cut0, 1, 2, 2); // size: 2
	cut.insert(cut0,size);

	cut0.remove(0);
	cut.insert(cut0,1);

	cut0.remove(0);
	cut.insert(cut0,0);

	// orc
	oracle::Dynamic_array orc0;
	generate_oracle(orc0, 1, 2, 2); // size: 2
	orc.insert(orc0,size);

	orc0.remove(0);
	orc.insert(orc0,1);

	orc0.remove(0);
	orc.insert(orc0,0);

	compare_content(cut, orc);
	compare_exceptions(cut, orc);
	} // array size: larger by 3

cout << "DB 5 run_tests" << endl;
	{
	// ***** remove(int,int)
	int size = orc.get_size();

	cut.remove(0,size/2);
	orc.remove(0,size/2);
	compare_content(cut, orc);
	compare_exceptions(cut, orc);

	size = orc.get_size();
	cut.remove(size/2,size-1);
	orc.remove(size/2,size-1);
	compare_content(cut, orc);
	compare_exceptions(cut, orc);
	}
}

class triple {
public:
	int start, delta, count;
};

triple triples[] = {
	{0,	1,	0},	// BUG
	{0,	1,	1},
	{0,	2,	2},
	{0,	3,	3},
	{0,	4,	4},
	{0,	5,	5},
	{0,	5,	1000},
	{0,	5,	10000}, // < 1s
	// {0,	5,	100000}, // > 60s
	{-1,	-1,	-1},	// sentinel
};

int main() {
	for (int i = 0; triples[i].start != -1; i++) {
		cout << "********** " << triples[i].start << " " << triples[i].delta << " ";
		cout << triples[i].count << endl;

		Dynamic_array cut;
		generate_cut(cut, triples[i].start, triples[i].delta, triples[i].count);

		oracle::Dynamic_array orc;
		generate_oracle(orc, triples[i].start, triples[i].delta, triples[i].count);

		run_tests(cut, orc);
	}
}
