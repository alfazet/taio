#!/usr/bin/env python

import tempfile
import random
import numpy
import time
import sys
import os
import matplotlib.pyplot as plt

from common import DEFAULT_BIGINT_BIN

sys.set_int_max_str_digits(0)
random.seed(2137)
retries = 5

def rand(digits):
    return random.randint(10 ** (digits - 1), 10**digits - 1)

digits_start = 500000
digits_finish = 5000000
steps = 20
digits = range(
    digits_start, digits_finish, (int)((digits_finish - digits_start) / steps)
)
times = []
result = []

for i in digits:
    iter_time = 0
    for j in range(retries):
        a = rand(i)
        b = rand(i)

        # Create temporary file for bigint test program input
        fd, input_file = tempfile.mkstemp()
        with os.fdopen(fd, "w") as inp:
            inp.write(f"+ {a} {b}")

        # Execute test program
        os.system(f"{DEFAULT_BIGINT_BIN} {input_file} > /dev/null")

        # Get time measured by program
        output_file = f"{input_file}.out"
        with open(output_file, "r") as of:
            output_text = of.read()
        time_measure = output_text

        # Remove temporary input and test program output file
        os.remove(input_file)
        os.remove(output_file)

        # Time from program output is in nanoseconds; convert it to ms
        time_ms = float(time_measure) / 1000000.0
        iter_time += time_ms

    times.append(iter_time / retries)

plt.plot(digits, times, "g")
plt.title("Czas wykonywania dodawania w zależności od długości liczb")
plt.xlabel("Długość dodawanych liczb [liczba cyfr]")
plt.ylabel("Czas [ms]")
plt.show()
