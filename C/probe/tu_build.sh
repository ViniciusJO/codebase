#!/usr/bin/env sh

set -e

gcc -Wall -Wextra -ggdb ./test_taged_union.c -o tu_test
./tu_test
rm tu_test
