#!/bin/bash
set -e

test_dir="tests"
OUT_DIR="$test_dir/add_files"

files=(
  "test1.txt"
  "test2.txt"
  "test3.txt"
  "test4.txt"
  "test5.txt"
  "test6.txt"
)

flags=(
  ""
  "-b"
  "-e"
  "-E"
  "-n"
  "-s"
  "-t"
  "-T"
  "--number-nonblank"
  "--number"
  "--squeeze-blank"
)

for file in "${files[@]}"
do
  input="$test_dir/$file"

  for flag in "${flags[@]}"
  do
    if diff <(./s21_cat $flag "$input") <(cat $flag "$input") > /dev/null # команда diff ничего не выводит
    then
      echo "[OK] $file $flag"
    else
      echo "[FAIL] $file $flag"
    fi
  done
done

# очищаем файлы для сравнения 
> "$OUT_DIR/my_cat.txt"
> "$OUT_DIR/cat.txt"

