#!/bin/bash
set -e

test_dir="tests"

files=(
  "test1.txt"
  "test2.txt"
  "test3.txt"
  "test4.txt"
  "test5.txt"
  "test6.txt"
)

patterns=(
  "test"
  "123"
  "^[A-Za-z]"
  "[0-9]{2,}"
)

flags=(
  ""
  "-i"
  "-v"
  "-c"
  "-l"
  "-n"
)

for file in "${files[@]}"; do
  input="$test_dir/$file"

  for pattern in "${patterns[@]}"; do
    if diff <(./s21_grep "$pattern" "$input") <(grep -E "$pattern" "$input") >/dev/null; then
      echo "[OK] $file pattern: $pattern"
    else
      echo "[FAIL] $file pattern: $pattern"
    fi

    if diff <(./s21_grep -e "$pattern" "$input") <(grep -E -e "$pattern" "$input") >/dev/null; then
      echo "[OK] $file -e pattern: $pattern"
    else
      echo "[FAIL] $file -e pattern: $pattern"
    fi

    for flag in "${flags[@]}"; do
      if diff <(./s21_grep $flag "$pattern" "$input") <(grep -E $flag "$pattern" "$input") >/dev/null; then
        echo "[OK] $file $flag pattern: $pattern"
      else
        echo "[FAIL] $file $flag pattern: $pattern"
      fi
    done
  done
done
