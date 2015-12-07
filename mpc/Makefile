
CXX = g++
STND=-ansi
CFLAGS = $(STND) -pedantic -O3 -g -Werror -Wall -Wextra -Wformat=2 -Wshadow \
  -Wno-long-long -Wno-overlength-strings -Wno-format-nonliteral -Wcast-align \
  -Wwrite-strings -Wstrict-prototypes -Wold-style-definition -Wredundant-decls \
  -Wnested-externs -Wmissing-include-dirs -Wswitch-default

TESTS = $(wildcard tests/*.cpp)
EXAMPLES = $(wildcard examples/*.cpp)
EXAMPLESEXE = $(EXAMPLES:.c=)

all: $(EXAMPLESEXE) check 

check: $(TESTS) mpc.cpp
	$(CXX) $(filter-out -Werror, $(CFLAGS)) $^ -lm -o test
	./test

examples/%: examples/%.cpp mpc.cpp
	$(CXX) $(CFLAGS) $^ -lm -o $@
  
clean:
	rm -rf test examples/doge examples/lispy examples/maths examples/smallc
