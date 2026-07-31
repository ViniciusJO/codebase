#!/usr/bin/env sh

set -e

gcc -Wall -Wextra -ggdb aal_test.c -o aal_test
./aal_test
rm aal_test
