lib_options = -I./src -I../lib-test-framework/src -L../lib-test-framework/build -L./build -Wl,--no-as-needed,-rpath=../lib-test-framework/build -Wl,--no-as-needed,-rpath=./build -ltest_framework -lhash_table

all: ./build/libhash_table.so ./bin/tester.bin

./bin/tester.bin: ./test/test_hash_table.c
	gcc ./test/test_hash_table.c $(lib_options) -g -Wall -o ./bin/tester.bin

./build/libhash_table.so: ./build/hash_table.o
	gcc -shared ./build/hash_table.o -o ./build/libhash_table.so

./build/hash_table.o: ./src/hash_table.c
	gcc -fPIC -c -g -Wall ./src/hash_table.c -o ./build/hash_table.o

clean:
	rm -f ./build/*.o
	rm -f ./build/*.so
	rm -f ./bin/*.bin