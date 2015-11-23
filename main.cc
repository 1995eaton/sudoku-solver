#include <iostream>
#include <vector>
#include <omp.h>

#include "sudoku.hh"

#define MULTITHREADED

int main(int argc, char **argv) {
  bool silent = argc == 2 && std::string(argv[1]) == "-s";
  std::vector<std::string> boards;
  for (std::string board, line; std::getline(std::cin, line);) {
    if (line[0] != 'G') {
      board += line;
      if (board.length() == 81) {
        boards.push_back(board);
        board.clear();
      }
    }
  }
#ifdef MULTITHREADED
#pragma omp parallel for
#endif
  for (size_t i = 0; i < boards.size(); i++) {
    Sudoku s(boards[i]);
    s.solve();
    boards[i] = silent ? s.lstr() : s.str();
  }
  for (size_t i = 0; i < boards.size(); i++) {
    if (!silent) {
      std::cout << "--> Board " << i << std::endl
        << boards[i] << std::endl;
    } else {
      std::cout << boards[i] << std::endl;
    }
  }
}
