all:
	g++ -std=c++11 -o test test.cc interstellarclock.cpp
clean:
	rm test
