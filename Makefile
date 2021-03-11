flags = --std=gnu++11 -O3 -Werror -Wall -Wextra -Wpedantic
all: main

main: bin/ bin/main.o bin/huffman.o
	g++ bin/main.o bin/huffman.o -o main
test: bin/ bin/test.o bin/huffman_test.o bin/autotest.o bin/huffman.o
	g++ bin/test.o bin/huffman_test.o bin/autotest.o bin/huffman.o -o test
bin/test.o:
	g++ -c $(flags) test/test.cpp -o bin/test.o -I include/
bin/huffman_test.o:
	g++ -c $(flags) test/huffman_test.cpp -o bin/huffman_test.o -I include/
bin/autotest.o:
	g++ -c $(flags) test/autotest.cpp -o bin/autotest.o -I include/
bin/main.o:
	g++ -c $(flags) src/main.cpp -o bin/main.o -I include/
bin/huffman.o:
	g++ -c $(flags) src/huffman.cpp -o bin/huffman.o -I include/
bin/:
	mkdir bin
clean:
	rm bin/*.o main test -rf bin/
