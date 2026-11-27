import csv
import sys

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np

from scipy.optimize import curve_fit

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
    
    # brute points
    if algo == "0":
        s = int(len(x)*0.1)
        h = int(len(x)*3/4)
        x_fit = np.array([x[s], x[h]])
        y_fit = np.array([y[s], y[h]])

    # karmarkar-karp points
    if algo == "1":
        s = int(len(x)*0.1)
        h = int(len(x)/2)
        x_fit = np.array([x[s], x[h]])
        y_fit = np.array([y[s], y[h]])
 
    def exponential(x, a, b):
        return a * np.exp(b * x)

    x_fun = np.linspace(x[0], x[len(x)-1], 100)

    linear = np.polyfit(x_fit, y_fit, 1)
    y_lin = np.polyval(linear, x_fun)
    quadratic = np.polyfit(x_fit, y_fit, 2)
    y_quad = np.polyval(quadratic, x_fun)
    nlogn = np.column_stack((x_fit * np.log(x_fit), np.ones(len(x_fit))))
    coeff_nlogn = np.linalg.lstsq(nlogn, y_fit, rcond=None)[0]
    y_nlogn = coeff_nlogn[0] * np.array(x_fun) * np.log(np.array(x_fun)) + coeff_nlogn[1]
    poly13 = np.polyfit(x_fit, y_fit, 13)
    y_p13 = np.polyval(poly13, x_fun)
    popt, _ = curve_fit(exponential, x_fit, y_fit, p0=(0, 0.1), maxfev=10000)
    y_exp = exponential(np.array(x_fun), *popt)

    if algo == "1":
        x = x[int(len(x)/2):]
        y = y[int(len(y)/2):]
        x_fun = x_fun[int(len(x_fun)/2):]
        y_lin = y_lin[int(len(y_lin)/2):]
        y_nlogn = y_nlogn[int(len(y_nlogn)/2):]
        y_quad = y_quad[int(len(y_quad)/2):]
        y_exp = y_exp[int(len(y_exp)/2):]

    plt.figure()
    plt.plot(x, y, f"o-{ALGO_COLORS[algo]}", label=ALGO_NAMES[algo])
    if algo == "0" and param == "n":
        plt.plot(x_fun, y_exp,   "y--", label=f"O(eⁿ)")
    if algo == "0" and param == "k":
        plt.plot(x_fun, y_p13,   "m--", label=f"O({param}¹³)")
    if algo == "1" or (algo == "0" and param == "l"):
        plt.plot(x_fun, y_lin,   "b--", label=f"O({param})")
    if algo == "1":
        plt.plot(x_fun, y_nlogn, "c--", label=f"O({param} log({param}))")
        plt.plot(x_fun, y_quad,  "m--", label=f"O({param}²)")
    plt.title(f"{ALGO_NAMES[algo]}: czas działania w zależnosci od {param} ({fixed})")
    plt.gca().xaxis.set_major_locator(ticker.MaxNLocator(integer=True))
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

