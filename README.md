# TAiO

## Notation
- `n` be the number of non-negative integers in the input set `x_1, x_2, ... x_n`
- `M = max(x_1, x_2, ..., x_n)`
- `K = x_1 + x_2 + ... + x_n`

## Constraints
- For the brute force `O(2^n)` solution: `1 <= n <= 32`, arbitrary `K`.
- For the meet-in-the-middle `O(2^(n/2))` solution: `1 <= n <= 64`, arbitrary `K`.
- For the pseudo-poly `O(nK)` solution: `1 <= nK, nM <= 2^31 - 1` (so that everything fits inside a 32 bit `int`).

## Input format
In the first line `n` (number of test cases), then `n` test cases, one per line.
Each test case is a list of integers separated by spaces.
