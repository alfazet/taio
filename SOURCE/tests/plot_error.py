import csv
import sys

import matplotlib.pyplot as plt
import numpy as np

def load_results():
    with open(RESULTS_FILE, newline="") as f:
        return list(csv.DictReader(f))

def bound(k):
    return 4 / 3 - 1 / (3 * k)

def plot_series(rows, param):
    points = rows
    if not points:
        return

    n = [int(r["n"]) for r in points][0]
    l = [int(r["l"]) for r in points][0]

    x = [int(r["k"]) for r in points]
    y = [float(r["error"]) for r in points]

    x_bound = np.linspace(x[0], x[len(x)-1], num=50)
    y_bound = [bound(k) for k in x_bound]

    plt.figure()
    plt.plot(x, y, "o-b", label="Obliczony stosunek")
    plt.plot(x_bound, y_bound, "--g", label=r"Ograniczenie: $\frac{4}{3}-\frac{1}{3k}$")
    plt.title(
        "Stosunek maksymalnej różnicy podzbiorów względem sumy zbioru\n"
        f"wejściowego w zależnosci od k (n={n}, l={l})"
    )
    plt.xlabel("Liczba podziałów zbioru")
    plt.ylabel("Stosunek podzbiorów o największych sumach")
    plt.grid(True)
    plt.legend()

if len(sys.argv) < 2:
    print(f"Uzycie: python {sys.argv[0]} <plik_csv>")
    sys.exit(1)
RESULTS_FILE = sys.argv[1]

rows = load_results()
for param in "k":
    plot_series(rows, param)
plt.show()

