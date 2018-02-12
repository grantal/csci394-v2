
CXX=clang++-5.0   # Can switch to g++ if desired
CXXFLAGS=-g -std=c++17 -Wall -pedantic -Wextra -Werror
LDFLAGS=$(CXXFLAGS)
LIBS=

INC=ptrtree.hh vectree.hh tree.hh catch.hpp
SRC=vectree.cc ptrtree.cc test_tree.cc test_ptrtree.cc
OBJ=$(SRC:.cc=.o)

all: test_ptrtree

test_tree: test_ptrtree

%.o.cc: %.cc %.hh
	$(CXX) $(CFLAGS) -c -o $@ $<

test_vectree: $(OBJ)
	$(CXX) $(LDFLAGS) $(LIBS) -o $@ test_tree.o vectree.o

test_ptrtree: $(OBJ)
	$(CXX) $(LDFLAGS) $(LIBS) -o $@ test_ptrtree.o ptrtree.o

test: test_ptrtree
	./test_ptrtree

clean:
	rm -f $(OBJ)
