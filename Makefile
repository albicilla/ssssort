FLAGS=-std=c++14 -Wall

all: debug

debug:
	$(CXX) $(FLAGS) -O0 -ggdb -o sort sort.cpp

release:
	$(CXX) $(FLAGS) -O3 -g -DNDEBUG -o sort sort.cpp

run:
	./sort
