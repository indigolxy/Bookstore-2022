#!/bin/bash

g++ -o main main.cpp

i=0
while (( $? == 0 ))
do
  ((i++))
  echo "Case ${i}"
  ./clear.sh
  python3 makedata2.py
  echo "running std."
  ./main <test.txt >2.out
  echo "running test"
  ./test <test.txt >2.ans
  echo "comparing"
  diff 2.out 2.ans >/dev/null
done
