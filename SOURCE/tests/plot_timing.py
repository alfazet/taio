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
    y_fit = np.array([y[s], y[h]])
    
    linear = np.polyfit(x_fit, y_fit, 1)
    y_lin = np.polyval(linear, x)
    quadratic = np.polyfit(x_fit, y_fit, 2)
    y_quad = np.polyval(quadratic, x)
    nlogn = np.column_stack((x_fit * np.log(x_fit), np.ones(len(x_fit))))
    coeff_nlogn = np.linalg.lstsq(nlogn, y_fit, rcond=None)[0]
    y_nlogn = coeff_nlogn[0] * np.array(x) * np.log(np.array(x)) + coeff_nlogn[1]

    plt.figure()
    plt.plot(x, y, f"o-{ALGO_COLORS[algo]}", label=ALGO_NAMES[algo])
    if algo == "1":
        plt.plot(x, y_lin,   "b--",
            label=f"{linear[0]:.3f}{param} {"-" if linear[1] < 0 else "+"} {abs(linear[1]):.2f}")
        plt.plot(x, y_nlogn, "c--", label=f"{coeff_nlogn[0]:.6f}{param} log({param}) {"-" if coeff_nlogn[1] < 0 else "+"} {abs(coeff_nlogn[1]):.2f}")
        plt.plot(x, y_quad,  "m--", 
            label=f"{quadratic[0]:.6f}{param}² {"-" if quadratic[1] < 0 else "+"} {abs(quadratic[1]):.2f}{param} {"-" if quadratic[2] < 0 else "+"} {abs(quadratic[2]):.2f}")
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

