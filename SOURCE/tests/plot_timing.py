import csv
import sys

import matplotlib.pyplot as plt
import numpy as np

ALGO_NAMES = {"0": "brute-force", "1": "karmarkar-karp"}
ALGO_COLORS = {"0": "r", "1": "g"}
PARAM_LABELS = {
    "n": "Liczba liczb n",
    "l": "Długosc liczb l [cyfry]",
    "k": "Liczba podzbiorów k",
}

def load_results():
    with open(RESULTS_FILE, newline="") as f:
        return list(csv.DictReader(f))


def plot_series(rows, algo, param):
    points = [r for r in rows if r["algo"] == algo and r["param"] == param]
    if not points:
        return

    x = [int(r["value"]) for r in points]
    y = [float(r["time_us"])/1000 for r in points]
    fixed = ", ".join(f"{p}={points[0][p]}" for p in "nlk" if p != param)
    
    s = int(len(x)*0.1)
    h = int(len(x)/2)
    x_fit = np.array([x[s], x[h]])
    linear = np.polyfit(x_fit, [y[s], y[h]], 1)
    y_lin = np.polyval(linear, x)
    quadratic = np.polyfit(x_fit, [y[s], y[h]], 2)
    y_quad = np.polyval(quadratic, x)
    #nlogn = np.column_stack((x_fit * np.log(x_fit), np.ones(len(x))))
    #coeff_nlogn = np.linalg.lstsq(nlogn, y, rcond=None)[0]
    #y_nlogn = coeff_nlogn[0] * np.array(x) * np.log(np.array(x)) + coeff_nlogn[1]

    plt.figure()
    plt.plot(x, y, f"o-{ALGO_COLORS[algo]}", label=ALGO_NAMES[algo])
    if algo == "1":
        plt.plot(x, y_lin,   "b--", label="O(n)")
        #plt.plot(x, y_nlogn, "c--", label="O(nlogn)")
        plt.plot(x, y_quad,  "m--", label="O(n^2)")
    plt.title(f"{ALGO_NAMES[algo]}: czas działania w zależnosci od {param} ({fixed})")
    plt.xlabel(PARAM_LABELS[param])
    plt.ylabel("Czas [ms]")
    plt.grid(True)
    if algo == "0" and param == "n":
        plt.yscale("log")
    plt.legend()

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

