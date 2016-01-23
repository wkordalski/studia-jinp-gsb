all:
	g++ -Wall -std=c++11 -o test gsb_example.cpp interstellarclock.cpp
clean:
	rm test
