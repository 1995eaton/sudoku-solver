cd "$(dirname $0)"
make all

PUZZLES=(
./boards/hard.txt
./boards/hard-2.txt
./boards/project-euler-96.txt
)

for p in ${PUZZLES[*]}; do
  count=$(./sudoku -s < "$p" | sha1sum)
  re_count=$(qqwing --solve --csv < "$p" | grep -oE '[[:digit:]]+' | sha1sum)
  pass='\e[38;5;1mFAIL\e[0m'
  [ "$count" == "$re_count" ] && pass='\e[38;5;2mPASS\e[0m'
  echo -e "[$pass] $p => $count,$re_count" | sed 's/\s*-\s*//g'
done
