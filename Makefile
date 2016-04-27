FLAGS=-std=c++14 -Wall -Wextra -Werror
# if you want to be pedantic, uncomment this line (for CXX=clang++):
#FLAGS+=-Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-shadow -Wno-global-constructors -Wno-padded

all: release

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
