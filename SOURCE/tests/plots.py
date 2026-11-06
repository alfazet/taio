#!/usr/bin/env python

import argparse
import os
import random
import re
import statistics
import subprocess
import sys
import tempfile

import matplotlib.pyplot as plt
from matplotlib.ticker import LogFormatterSciNotation

from common import DEFAULT_BIN

TC_GEN = "./tc_gen.py"

RETRIES = 20
TIMEOUT = 60.0
FIXED_K = 5
FIXED_L = 20
FIXED_N = 1000
TIME_REGEX = r"(\d+(?:\.\d+)?)"

ALGOS = {
    0: {
        "name": "brute-force",
        "color": "r",
        "n_values": list(range(2, 25, 2)),
        "l_values": [1, 2, 4, 8, 16, 32, 64, 128, 256],
        "k_values": list(range(2, 11)),
    },
    1: {
        "name": "karmarkar-karp",
        "color": "g",
        "n_values": list(range(100, 20001, 100)),
        "l_values": list(range(1, 2049, 8)),
        "k_values": list(range(2, 100)),
    },
}


def generate_testcase(path, n, l):
    seed = random.randrange(2**31)
    proc = subprocess.run(
        [sys.executable, TC_GEN, str(n), str(l), str(seed)],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    if proc.returncode != 0:
        raise RuntimeError(f"tc_gen zakończył się kodem {proc.returncode}: {proc.stderr.strip()}")
    with open(path, "w") as f:
        f.write(proc.stdout)
        f.write("\n")


def run_once(algo, k, n, l):
    fd, path = tempfile.mkstemp()
    os.close(fd)
    try:
        generate_testcase(path, n, l)
        proc = subprocess.run(
            [DEFAULT_BIN, "-i", path, "-a", str(algo), "-k", str(k), "-t"],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            timeout=TIMEOUT,
            text=True,
        )
        if proc.returncode != 0:
            raise RuntimeError(f"taio zakończył się kodem {proc.returncode}: {proc.stderr.strip()}")
        match = re.search(TIME_REGEX, proc.stdout)
        if not match:
            raise RuntimeError(f"Brak czasu w wyjściu: {proc.stdout!r}")
        return float(match.group(1))
    finally:
        if os.path.exists(path):
            os.remove(path)


def measure(algo, k, n, l):
    samples = []
    for _ in range(RETRIES):
        try:
            samples.append(run_once(algo, k, n, l))
        except subprocess.TimeoutExpired:
            return None
    return statistics.mean(samples)


def sweep(algo, param, values, fixed_n, fixed_l, fixed_k):
    xs, ys = [], []
    for v in values:
        n, l, k = fixed_n, fixed_l, fixed_k
        if param == "n":
            n = v
        elif param == "l":
            l = v
        else:
            k = v
        t = measure(algo, k, n, l)
        if t is None:
            print(f"[{ALGOS[algo]['name']}] n={n} l={l} k={k}: timeout, przerywam serię", file=sys.stderr)
            break
        xs.append(v)
        ys.append(t)
        print(f"[{ALGOS[algo]['name']}] n={n} l={l} k={k}: {t:.1f} us", file=sys.stderr)
    return xs, ys


def plot_sweep(results, param, fixed_n, fixed_l, fixed_k):
    plt.figure()
    for algo, (xs, ys) in results.items():
        if xs:
            plt.plot(xs, ys, ALGOS[algo]["color"], marker="o", label=ALGOS[algo]["name"])

    if param == "n":
        plt.title(f"Czas działania w zależności od n (l = {fixed_l}, k = {fixed_k})")
        plt.xlabel("Liczba liczb n")
    elif param == "l":
        plt.title(f"Czas działania w zależności od l (n = {fixed_n}, k = {fixed_k})")
        plt.xlabel("Długość liczb l [liczba cyfr]")
    else:
        plt.title(f"Czas działania w zależności od k (n = {fixed_n}, l = {fixed_l})")
        plt.xlabel("Liczba podzbiorów k")
    plt.ylabel("Czas [µs]")
    # plt.xscale("log")
    # plt.yscale("log")

    # ax = plt.gca()
    # ax.xaxis.set_major_formatter(LogFormatterSciNotation())
    # ax.yaxis.set_major_formatter(LogFormatterSciNotation())

    plt.legend()
    plt.grid(True, which="both")


p = argparse.ArgumentParser()
p.add_argument("-k", type=int, default=2)
p.add_argument("--seed", type=int, default=2137)
p.add_argument("--alg", type=int, default=1)
args = p.parse_args()

random.seed(args.seed)

results_n = {}
results_l = {}
results_k = {}

algo = args.alg
cfg = ALGOS[algo]

results_n[algo] = sweep(algo, "n", cfg["n_values"], FIXED_N, FIXED_L, args.k)
results_l[algo] = sweep(algo, "l", cfg["l_values"], FIXED_N, FIXED_L, args.k)
results_k[algo] = sweep(algo, "k", cfg["k_values"], FIXED_N, FIXED_L, args.k)

plot_sweep(results_n, "n", FIXED_N, FIXED_L, args.k)
plot_sweep(results_l, "l", FIXED_N, FIXED_L, args.k)
plot_sweep(results_k, "k", FIXED_N, FIXED_L, args.k)
plt.show()
