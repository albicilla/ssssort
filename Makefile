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


_sqlplot:
	sp-process speed.plot

_gnuplot:
	gnuplot speed.plot

_fixup:
# fixup standard deviation lines
	sed -i 's/title "algo=ssssort,a=\(-\)*1"\( notitle ls 3\)* with lines\(points\)*/notitle ls 3 with lines/' speed.plot
	sed -i 's/title "algo=stdsort,a=\(-\)*1"\( notitle ls 5\)* with lines\(points\)*/notitle ls 5 with lines/' speed.plot
# set average line style
	sed -i 's/title "algo=ssssort,a=0"\( ls 4\)*/title "algo=ssssort" ls 4/' speed.plot
	sed -i 's/title "algo=stdsort,a=0"\( ls 6\)*/title "algo=stdsort" ls 6/' speed.plot

plot: _sqlplot _fixup _gnuplot
