CXX=g++
CXXFLAGS=-std=c++14 -msse4.1 -march=native -Ofast -Wall -Werror -Wextra -pedantic
SRC=sudoku.cc
OBJ=$(SRC:.cc=.o)
EXE=sudoku
PREFIX=/usr/local

all: $(SRC) $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(OBJ) -o $@

.c.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJ) $(EXE)

install: $(EXE)
	install -m 755 $(EXE) $(PREFIX)/bin
