#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>

class Sudoku {
    private:
        int board[9][9], moves[9][9];
        int hcnt[9], hloc[9];
        bool solved = false;

        int get_candidates(int, int) const;
        bool solve_hidden_block(int, int);
        inline void logic_nonhidden_matches();
        inline bool logic_hidden_matches();
        void logic();
        int get_n() const;
        void backtrack();

    public:
        Sudoku(const std::string& nums);
        void apply_board(const std::string& nums);
        std::string str() const;
        std::string lstr() const;
        void solve();
};

#endif
