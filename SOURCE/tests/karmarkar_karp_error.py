#!/bin/python

import random
import sys
import os

from common import DEFAULT_BIN

random.seed(911)

def rand_max(max_val):
    return random.randint(1, max_val)

def rand_digits(digits):
    return random.randint(10 ** (digits - 1), 10**digits - 1)

def generate_set(digits, n, k):
    rand_set = []
    
    # Get set total sum
    total = rand_digits(digits)
    # Adjust total so it's divisible by k
    total += k - total % k

    # Fill set
    average = total // n
    if total == 0 or average == 0:
        print(f't={total}, a={average}')
    for i in range(n):
        rand_set.append(average)
    # Adjust last element for integral division error
    rand_set[n - 1] += total - average * n

    # Add some randomization so that set elements' differ
    difference = 0
    for i in range(n - 1):
        # Generate random difference
        rand_diff = rand_max(average) - average // 2
        # Subtract difference from value in set
        rand_set[i] += rand_diff
        # Add difference
        difference -= rand_diff

    # Add remaining difference to the last element
    rand_set[n - 1] += difference

    # Account for uncommon case when difference value exceeded last element
    if (rand_set[n - 1] < 0):
        return generate_set(digits, n, k)

    return rand_set

numbers = [20, 30, 40, 50, 67, 100, 200, 300, 1000, 2000]
subsets = [2, 3, 4, 5, 6, 7, 10, 15]
digits  = [3, 4, 5, 6, 7, 10, 20, 30, 40, 50, 67, 100, 200, 420, 911, 2137]

retries = 5
ratio_count = 0
total_ratio = 0

for n in numbers:
    print(f'Testy dla n={n}')
    for k in subsets:
        for d in digits:
            # Skip if generated total might be less than number count in set
            if 10**(d-1) < n:
                continue
            # Generate input file with 'retries' count of sets
            input_file = '.karmarkar_karp_error_input.txt'
            with open(input_file, "w") as inp:
                inp.write(f'{retries}\n')
                for r in range(retries):
                    generated_set = generate_set(d, n, k)
                    #print(f'n={n:3}; k={k:2}; d={d:2}; r={r}; set: {generated_set}')
                    for num in generated_set:
                        inp.write(f'{num} ')
                    inp.write('\n')

            # Run program with Karmarkar algorithm
            os.system(f'{DEFAULT_BIN} -i {input_file} -a 1 -k {k}')

            output_file = f'{input_file.removesuffix('.txt')}_output.txt'
            #print(f'\n\n\n ===> file for n={n}, k={k}, d={d}:')
            #os.system(f'cat {output_file}')

            ratio = 0
            with open(output_file, "r") as outp:
                # Skip first line
                outp.readline()
                for r in range(retries):
                    # Skip empty line
                    outp.readline()
                    # Get total of divided set
                    line = outp.readline()
                    total = (int)(line.split(' ', 1)[0])
                    # Get subsets totals
                    set_sums = []
                    for i in range(k):
                        line = outp.readline()
                        set_sums.append((int)(line.split(' ', 1)[0]))
                    # Get max difference between sets
                    line = outp.readline()
                    maxdiff = (int)(line.split(' ', 1)[0])

                    # Compute maximum ratio between subset sum and target subset sum
                    target = total // k
                    maxsum = max(set_sums)
                    maxratio = maxsum / target
                    #print(f't={total}; sums:{set_sums}; d={maxdiff}; r={maxraito}')
                    # Add ratio to cummulated one
                    ratio += maxratio

            # Average subsets ratios
            ratio /= retries
#            print(f'Średni stosunek sumy o najwiękej różnicy względem docelowej wartości sumy a docelową wartością dla parametrów n={n}, k={k}, d={d}: {ratio}')
            total_ratio += ratio
            ratio_count += 1

            os.remove(input_file)
            os.remove(output_file)

total_ratio /= ratio_count
print(f'Łączny średni stosunek: {total_ratio}')

