using namespace std;

namespace oracle {
class Dynamic_array {
public:
	Dynamic_array();
	Dynamic_array(Dynamic_array &a);
	Dynamic_array &operator=(Dynamic_array &a);
	~Dynamic_array();

	void print_state(void);
	int get_size(void);

	int& operator[](int i);

	void insert(int x, int i);
	void insert(Dynamic_array &a, int i);

	void remove(int i);
	void remove(int start, int end);

	class Subscript_range_exception {
	};
private:
	enum {
		BLOCK_SIZE = 5,
	};

	void shift_left(int start, int end, int delta);
	void shift_right(int start, int end, int delta);

	int *array;
	int allocated_size;
	int size;
};
}
