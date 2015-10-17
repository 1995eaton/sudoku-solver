#include <iostream>
#include <sstream>
#include <vector>
#include <omp.h>

#define MULTITHREADED

class Sudoku {
  private:
    int board[9][9], moves[9][9], hcnt[9], hloc[9];
    bool solved = false;

    int get_candidates(int x, int y) {
      int mask = 0;
      for (int i = 0; i < 9; i++)
        mask |= (1 << board[i][x]), mask |= (1 << board[y][i]);
      int *sp = *board + y / 3 * 27 + x / 3 * 3;
               mask |= (1 << *sp) | (1 << *(sp + 1)) | (1 << *(sp + 2));
      sp += 9; mask |= (1 << *sp) | (1 << *(sp + 1)) | (1 << *(sp + 2));
      sp += 9; mask |= (1 << *sp) | (1 << *(sp + 1)) | (1 << *(sp + 2));
      return 511 & ~(mask >> 1);
    }

    bool solve_hidden_block(int blx, int bly) {
      blx *= 3;
      bly *= 3;
      std::fill(hcnt, hcnt + 9, 0);
      for (int y = bly; y < bly + 3; y++) {
        for (int x = blx; x < blx + 3; x++) {
          if (!board[y][x]) {
            for (int n = 0; n < 9; n++) {
              if ((moves[y][x] & (1 << n)) && hcnt[n] < 2) {
                hcnt[n]++;
                hloc[n] = y * 9 + x;
              }
            }
          }
        }
      }
      for (int n = 0; n < 9; n++) {
        if (hcnt[n] == 1) {
          *(*board + hloc[n]) = n + 1;
          return true;
        }
      }
      return false;
    }

    inline void logic_nonhidden_matches() {
      bool match_found = false;
      do {
        match_found = false;
        for (int y = 0; y < 9; y++) {
          for (int x = 0; x < 9; x++) {
            if (board[y][x] == 0) {
              if (__builtin_popcount(moves[y][x] = get_candidates(x, y)) == 1) {
                // index of least significant bit + 1
                // e.g. 0b001100000 = 6
                board[y][x] = __builtin_ffs(moves[y][x]);
                match_found = true;
              }
            }
          }
        }
      } while (match_found);
    }

    inline bool logic_hidden_matches() {
      return   solve_hidden_block(0, 0)
             | solve_hidden_block(0, 1)
             | solve_hidden_block(0, 2)
             | solve_hidden_block(1, 0)
             | solve_hidden_block(1, 1)
             | solve_hidden_block(1, 2)
             | solve_hidden_block(2, 0)
             | solve_hidden_block(2, 1)
             | solve_hidden_block(2, 2);
    }

    void logic() {
      while (logic_nonhidden_matches(), logic_hidden_matches());
    }

    // get offset of first empty tile in the board
    int get_n() {
      int low = 10, n = -1;
      for (int i = 0; i < 81; i++) {
        if (*(*board + i) == 0) {
          // number of bits in a number
          int b = __builtin_popcount(*(*moves + i));
          if (b < low) {
            low = b;
            n = i;
          }
        }
      }
      return n;
    }

    void backtrack() {
      int n = get_n();
      if (n == -1) {
        solved = true;
        return;
      }
      int state[81];
      std::copy(*board, *board + 81, state);
      for (int mask = *(*moves + n); mask & 511; mask &= mask - 1) {
        *(*board + n) = __builtin_ffs(mask);
        logic();
        backtrack();
        if (solved)
          return;
        std::copy(state, state + 81, *board);
      }
    }

  public:
    void apply_board(const std::string& nums) {
      solved = false;
      int *bp = *board;
      for (const char letter: nums) {
        *bp = letter - '0';
        if (*bp > 9 || *bp < 0)
          *bp = 0;
        bp++;
      }
    }

    Sudoku(const std::string& nums) {
      apply_board(nums);
    }

    std::string str() {
      std::ostringstream res;
      for (int y = 0; y < 9; y++) {
        res << (y % 3 == 0 ?  "+-------+-------+-------+\n|" : "|");
        for (int x = 1; x < 10; x++) {
          res << ' ' << (board[y][x - 1] ? board[y][x - 1] : '.')
            << (x % 3 == 0 ? " |" : "");
        }
        res << "\n";
      }
      res << "+-------+-------+-------+\n";
      return res.str();
    }

    std::string lstr() {
      std::ostringstream res;
      for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
          res << board[y][x];
        }
      }
      return res.str();
    }

    void solve() {
      logic();
      backtrack();
    }
};

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
