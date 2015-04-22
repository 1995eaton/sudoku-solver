#include <iostream>
#include <sstream>
#include <fstream>

static int board[9][9], moves[9][9];
static bool solved = false;

static int get_candidates(int x, int y) {
  int mask = 0;
  for (int i = 0; i < 9; i++) {
    mask |= (1 << (board[i][x] - 1));
    mask |= (1 << (board[y][i] - 1));
    mask |= (1 << (board[y / 3 * 3 + i / 3][x / 3 * 3 + i % 3] - 1));
  }
  return 511 & ~mask;
}

static bool solve_hidden_block(const int blx, const int bly) {
  static int hcnt[9], hloc[9];
  std::fill(hcnt, hcnt + 9, 0);
  for (int _y = bly * 3; _y < bly * 3 + 3; _y++) {
    for (int _x = blx * 3; _x < blx *3 + 3; _x++) {
      if (!board[_y][_x]) {
        for (int n = 0; n < 9; n++) {
          if (hcnt[n] < 2 && moves[_y][_x] & (1 << n)) {
            hcnt[n]++;
            hloc[n] = _y * 9 + _x;
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

static inline void logic_nonhidden_matches() {
  bool match_found = false;
  do {
    match_found = false;
    for (int y = 0; y < 9; y++) {
      for (int x = 0; x < 9; x++) {
        if (board[y][x] == 0) {
          if (__builtin_popcount(moves[y][x] = get_candidates(x, y)) == 1) {
            board[y][x] = __builtin_ffs(moves[y][x]);
            match_found = true;
          }
        }
      }
    }
  } while (match_found);
}

static inline bool logic_hidden_matches() {
  bool match_found = false;
  for (int y = 0; y < 3; y++) {
    for (int x = 0; x < 3; x++) {
      match_found |= solve_hidden_block(x, y);
    }
  }
  return match_found;
}

static void logic() {
  do logic_nonhidden_matches();
    while (logic_hidden_matches());
}

static bool check_pos(const int n, const int x, const int y) {
  for (int i = 0; i < 9; i++) {
    if (board[i][x] == n || board[y][i] == n ||
        board[y / 3 * 3 + i / 3][x / 3 * 3 + i % 3] == n) {
      return false;
    }
  }
  return true;
}

// find the square with the least amount of valid moves
static int get_n() {
  int low = 10, lown = -1;
  for (int n = 0; n < 81; n++) {
    if (*(*board + n) == 0) {
      int b = __builtin_popcount(*(*moves + n));
      if (b < low) {
        low = b;
        lown = n;
      }
    }
  }
  return lown;
}

static inline void backtrack() {
  int n = get_n();
  if (n == -1) {
    solved = true;
    return;
  }
  int x = n % 9,
      y = n / 9;
  int state[81];
  std::copy(*board, *board + 81, state);
  for (int i = 0; i < 9; i++) {
    if ((moves[y][x] & (1 << i)) && check_pos(i + 1, x, y)) {
      board[y][x] = i + 1;
      logic();
      backtrack();
      if (solved)
        return;
      std::copy(state, state + 81, *board);
    }
  }
}

static std::string str() {
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

static std::string lstr() {
  std::ostringstream res;
  for (int y = 0; y < 9; y++) {
    for (int x = 0; x < 9; x++) {
      res << board[y][x];
    }
  }
  return res.str();
}

static void apply_board(const std::string& nums) {
  solved = false;
  int *bp = *board;
  for (const char letter: nums) {
    *bp++ = letter - '0';
  }
  logic();
}

int main(int argc, char **argv) {
  int board_num = 1;
  bool silent = argc == 2 && std::string(argv[1]) == "-s";
  for (std::string board, line; std::getline(std::cin, line);) {
    if (line[0] != 'G') {
      board += line;
      if (board.length() == 81) {
        apply_board(board);
        backtrack();
        if (!silent) {
          std::cout << "--> Board " << board_num++ << std::endl
                    << str() << std::endl;
        } else {
          std::cout << lstr() << std::endl;
        }
        board.clear();
      }
    }
  }
}
