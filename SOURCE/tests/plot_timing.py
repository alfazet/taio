import csv
import sys

import matplotlib.pyplot as plt

ALGO_NAMES = {"0": "brute-force", "1": "karmarkar-karp"}
ALGO_COLORS = {"0": "r", "1": "g"}
PARAM_LABELS = {
    "n": "Liczba liczb n",
    "l": "Dlugosc liczb l [cyfry]",
    "k": "Liczba podzbiorow k",
}

def load_results():
    with open(RESULTS_FILE, newline="") as f:
        return list(csv.DictReader(f))


def plot_series(rows, algo, param):
    points = [r for r in rows if r["algo"] == algo and r["param"] == param]
    if not points:
        return

    x = [int(r["value"]) for r in points]
    y = [float(r["time_us"]) for r in points]
    fixed = ", ".join(f"{p}={points[0][p]}" for p in "nlk" if p != param)

    plt.figure()
    plt.plot(x, y, f"o-{ALGO_COLORS[algo]}")
    plt.title(f"{ALGO_NAMES[algo]}: czas dzialania w zaleznosci od {param} ({fixed})")
    plt.xlabel(PARAM_LABELS[param])
    plt.ylabel("Czas [µs]")
    plt.grid(True)
    if algo == "0" and param == "n":
        plt.yscale("log")

if len(sys.argv) < 2:
    print(f"Uzycie: python {sys.argv[0]} <plik_csv>")
    sys.exit(1)
RESULTS_FILE = sys.argv[1]

rows = load_results()
algos = sorted({r["algo"] for r in rows})
for algo in algos:
    for param in "nlk":
        plot_series(rows, algo, param)

plt.show()

