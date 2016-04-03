FLAGS=-std=c++14 -Wall -Wextra -Werror

all: debug string_debug

debug:
	$(CXX) $(FLAGS) -O0 -ggdb -o sort sort.cpp

release:
	$(CXX) $(FLAGS) -O3 -g -DNDEBUG -o sort sort.cpp

string_debug:
	$(CXX) $(FLAGS) -O0 -ggdb -o strings strings.cpp

string_release:
	$(CXX) $(FLAGS) -O3 -g -DNDEBUG -o strings strings.cpp

run:
	./sort
