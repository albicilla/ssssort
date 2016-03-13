FLAGS=-std=c++14

all: debug

debug:
	$(CXX) $(FLAGS) -O0 -ggdb -o sort sort.cpp

release:
	$(CXX) $(FLAGS) -O3 -DNDEBUG -o sort sort.cpp
