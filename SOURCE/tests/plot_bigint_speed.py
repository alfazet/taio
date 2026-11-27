
import csv
import sys

import matplotlib.pyplot as plt
import numpy as np

OP_NAMES = {"+": "dodawanie", "-": "odejmowanie"}
OP_COLORS = {"+": "g", "-": "r"}


def load_results(path):
    with open(path, newline="") as f:
        return list(csv.DictReader(f))


def plot_series(rows, op):
    points = [r for r in rows if r["op"] == op]
    if not points:
        return

    x = [int(r["digits"]) for r in points]
    y = [float(r["time_us"])/1000 for r in points]

    s = int(len(x)*0.1)
    h = int(len(x)/2)
    x_fit = np.array([x[s], x[h]])
    y_fit = np.array([y[s], y[h]])
    linear = np.polyfit(x_fit, y_fit, 1)
    y_lin = np.polyval(linear, x)

    plt.figure()
    plt.plot(x, y, f"o-{OP_COLORS[op]}", label=f"{OP_NAMES[op]}")
    plt.plot(x, y_lin, "b--", label=f"O(L)")
    plt.title(f"Czas wykonywania: {OP_NAMES[op]} w zależności od długosci liczb")
    plt.xlabel("Długość liczb L [liczba cyfr]")
    plt.ylabel("Czas [ms]")
    plt.grid(True)
    plt.legend()


if len(sys.argv) < 2:
    print(f"Uzycie: python {sys.argv[0]} <plik_csv>")
    sys.exit(1)

rows = load_results(sys.argv[1])
for op in OP_NAMES:
    plot_series(rows, op)

plt.show()
