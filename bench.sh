#!/usr/bin/env zsh

cd "$(dirname $0)"
make

time ./sudoku -s < ./boards/50k.txt > /dev/null
time qqwing --solve --csv < ./boards/50k.txt > /dev/null
