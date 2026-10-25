#!/bin/python

import tempfile
import random
import numpy
import time
import sys
import os
import matplotlib.pyplot as plt

sys.set_int_max_str_digits(1000000)

def get_program_str(input_file):
    return f"../build/test_bigint/test_bigint {input_file} > /dev/null"

def program(s):
    os.system(s)

def rand(digits):
    return random.randint(10**(digits - 1), 10**digits - 1)

digits_start=1000
digits_finish=200000
steps = 30
digits = range(digits_start, digits_finish, (int)((digits_finish - digits_start) / steps))
times = []
result = []

for i in digits:
    a = rand(i)
    b = rand(i)
    
    # Create temporary file for bigint test program input
    fd, input_file = tempfile.mkstemp()
    with os.fdopen(fd, 'w') as inp:
        inp.write(f'+ {a} {b}')

    # Program string is generated outside time measure to not take it into account
    program_str = get_program_str(input_file)

    time_start = time.perf_counter()
    program(program_str)
    time_end = time.perf_counter()
    
    # Remove temporary input file
    os.remove(input_file);

    time_measure = time_end - time_start
    times.append(time_measure)
    result.append(a+b)

plt.plot(digits, times, 'g')
plt.title('Czas wykonywania dodawania w zależności od długości liczb')
plt.xlabel('Długość dodawanych liczb [liczba cyfr]')
plt.ylabel('Czas [s]')
plt.show()

#plt.plot(result, times, 'r')
#plt.title('Czas wykonywania dodawania w zależności od liczby wyniku')
#plt.show()

