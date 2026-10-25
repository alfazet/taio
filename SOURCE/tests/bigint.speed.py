#!/bin/python

import random
import numpy
import time
import sys
import os
import matplotlib.pyplot as plt

sys.set_int_max_str_digits(1000000)

def get_program_str(a, b):
    return f"../build/test_bigint/taio + {a} {b} > /dev/null"

def program(s):
    os.system(s)

def rand(digits):
    return random.randint(10**(digits - 1), 10**digits - 1)

digits_start=10000
digits_finish=100000
steps = 100
digits = range(digits_start, digits_finish, (int)((digits_finish - digits_start) / steps))
times = []
result = []

for i in digits:
    a = rand(i)
    b = rand(i)
    
    # Program string is generated outside time measure to not take it into account
    program_str = get_program_str(a, b)
    time_start = time.perf_counter()
    program(program_str)
    time_end = time.perf_counter()

    time_measure = time_end - time_start
    times.append(time_measure)
    result.append(a+b)

plt.plot(digits, times, 'g')
plt.title('Czas wykonywania dodawania w zależności od długości liczb')
plt.show()

#plt.plot(result, times, 'r')
#plt.title('Czas wykonywania dodawania w zależności od liczby wyniku')
#plt.show()

