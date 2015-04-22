cd "$(dirname $0)"
make all

PUZZLES=(
./boards/hard.txt
./boards/project-euler-96.txt
)

for p in ${PUZZLES[*]}; do
  count=$(
  ./sudoku -s < "$p" |
      awk '{ c += substr($0, 0, 3) } END { print c }'
  )
  re_count=$(
  qqwing --solve --csv < "$p" |
      awk '{ c += substr($0, 0, 3) } END { print c }'
  )
  pass=FAIL
  [ $count -eq $re_count ] && pass=PASS
  echo \[$pass\] "$p": got $count, expected $re_count
done
