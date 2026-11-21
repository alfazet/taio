
import csv
import sys

import matplotlib.pyplot as plt

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
    y = [float(r["time_us"]) for r in points]

    plt.figure()
    plt.plot(x, y, f"o-{OP_COLORS[op]}")
    plt.title(f"Czas wykonywania: {OP_NAMES[op]} w zaleznosci od dlugosci liczb")
    plt.xlabel("Dlugosc liczb [liczba cyfr]")
    plt.ylabel("Czas [µs]")
    plt.grid(True)


if len(sys.argv) < 2:
    print(f"Uzycie: python {sys.argv[0]} <plik_csv>")
    sys.exit(1)

rows = load_results(sys.argv[1])
for op in OP_NAMES:
    plot_series(rows, op)

plt.show()
