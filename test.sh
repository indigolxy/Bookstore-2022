#!/bin/bash

g++ -o a.out main.cpp

i=0
while (( $? == 0 ))
do
  ((i++))
  echo "Case ${i}"
  ./clear.sh
  python3 makedata.py
  echo "running std."
  ./a.out <test.txt >2.out
  echo "running test"
  ./test <test.txt >2.ans
  echo "comparing"
  diff 2.out 2.ans >/dev/null
done
