.phony all:
all: test_final

test_final: test_final.cpp
	g++ -Wall -o test_final test_final.cpp dynamic_array.cpp oracle.cpp

.PHONY clean:
clean:
	-rm -rf *.o *.exe
