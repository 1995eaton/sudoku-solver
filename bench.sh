#!/usr/bin/env zsh

BENCHFILE=./boards/hard-bench.txt
cd "$(dirname $0)"
make

time ./sudoku -s < "$BENCHFILE" > /dev/null
time qqwing --solve --csv < "$BENCHFILE" > /dev/null
