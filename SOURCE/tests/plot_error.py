import csv
import sys

import matplotlib.pyplot as plt

def load_results():
    with open(RESULTS_FILE, newline="") as f:
        return list(csv.DictReader(f))

def lower_bound(k):
    # 4/3 - 1/(3(k-1)), undefined for k = 1
    return float("nan") if k <= 1 else 4 / 3 - 1 / (3 * (k - 1))

def upper_bound(k):
    # 4/3 - 1/(3k)
    return 4 / 3 - 1 / (3 * k)

def plot_series(rows, param):
    points = rows
    if not points:
        return

    x = [int(r["k"]) for r in points]
    y = [float(r["error"]) for r in points]
    fixed = ", ".join(f"{p}={points[0][p]}" for p in "nlk" if p != param)

    lower = [lower_bound(k) for k in x]
    upper = [upper_bound(k) for k in x]

    plt.figure()
    plt.plot(x, y, "o-b")
    plt.plot(x, lower, "--r", label=r"Dolne ograniczenie: $\frac{4}{3}-\frac{1}{3(k-1)}$")
    plt.plot(x, upper, "--g", label=r"Gorne ograniczenie: $\frac{4}{3}-\frac{1}{3k}$")
    plt.title(
        "Stosunek maksymalnej roznicy podzbiorow wzgledem sumy zbioru\n"
        f"wejsciowego w zaleznosci od {param} ({fixed})"
    )
    plt.xlabel("Liczba podziałów zbioru")
    plt.ylabel("Wzgledna różnica wyniku")
    plt.grid(True)

if len(sys.argv) < 2:
    print(f"Uzycie: python {sys.argv[0]} <plik_csv>")
    sys.exit(1)
RESULTS_FILE = sys.argv[1]

rows = load_results()
for param in "k":
    plot_series(rows, param)
plt.show()

