#!/usr/bin/env sh

set -e

gcc -Wall -Wextra -ggdb tu_test.c -o tu_test
./tu_test
rm tu_test
