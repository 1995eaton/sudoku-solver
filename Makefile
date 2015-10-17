CXX=g++
CXXFLAGS=-fopenmp -std=c++14 -march=native -Ofast -Wall -Wextra -pedantic
SRC=sudoku.cc
OBJ=$(SRC:.cc=.o)
EXE=sudoku
PREFIX=/usr/local

all: $(SRC) $(EXE)

$(EXE): $(OBJ)
	$(CXX) -fopenmp $(OBJ) -o $@

.c.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJ) $(EXE)

install: $(EXE)
	install -m 755 $(EXE) $(PREFIX)/bin
