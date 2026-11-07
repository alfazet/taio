#!/usr/bin/env python
"""
tc_gen: testcase generator
"""

import random
import sys

DIGITS = "0123456789"


def rand_with_len(l):
    s = random.choice(DIGITS[1:])
    for _ in range(l - 1):
        s += random.choice(DIGITS)

    return s


def main():
    argc = len(sys.argv)
    if argc < 3:
        print(
            "./tc_gen.py <n> <l> [seed], where `n`: number of numbers in the set, `l`: length of each number in the set"
        )
        return

    if argc >= 4:
        random.seed(int(sys.argv[3]))
    n = int(sys.argv[1])
    l = int(sys.argv[2])
    numbers = [rand_with_len(l) for _ in range(n)]

    print(1)
    for x in numbers:
        print(x, end=" ")


if __name__ == "__main__":
    main()
