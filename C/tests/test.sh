#!/usr/bin/env sh

set -e

TESTS="*_test.c test*.c"

echo $TESTS | tr ' ' '\n' | while read TEST; do
  cc -Wall -Wextra -o test $TEST
  ./test
  rm ./test
done
