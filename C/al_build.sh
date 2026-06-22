#!/usr/bin/env sh

set -e

gcc -Wall -Wextra -ggdb al_test.c -o al_test
./al_test
rm al_test
