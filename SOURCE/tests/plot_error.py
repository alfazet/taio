import csv
import sys

import matplotlib.pyplot as plt

def load_results():
    with open(RESULTS_FILE, newline="") as f:
        return list(csv.DictReader(f))

def plot_series(rows, param):
    points = rows
    if not points:
        return

    x = [int(r["k"]) for r in points]
    y = [float(r["error"]) for r in points]
    fixed = ", ".join(f"{p}={points[0][p]}" for p in "nlk" if p != param)

    plt.figure()
    plt.plot(x, y, "o-b")
    plt.title(
        "Stosunek maksymalnej roznicy podzbiorow wzgledem sumy zbioru\n"
        f"wejsciowego w zaleznosci od {param} ({fixed})"
    )
    plt.xlabel("Liczba podziałów zbioru")
    plt.ylabel("Wzgledna roznica wyniku")
    plt.grid(True)

if len(sys.argv) < 2:
    print(f"Uzycie: python {sys.argv[0]} <plik_csv>")
    sys.exit(1)
RESULTS_FILE = sys.argv[1]

rows = load_results()
for param in "nlk":
    plot_series(rows, param)
plt.show()

