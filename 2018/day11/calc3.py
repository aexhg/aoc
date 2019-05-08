import sys
import re
from collections import defaultdict
from collections import deque
import numpy as np
from scipy import signal

def calculate(lines):
    serial = lines

    def power(x, y):
        rack = (x + 1) + 10
        power = rack * (y + 1)
        power += serial
        power *= rack
        return (power // 100 % 10) - 5

    grid = np.fromfunction(power, (300, 300))

    for width in range(3, 30):
        c = np.diag(np.ones(width))
        c.fill(1)
        windows = signal.convolve2d(grid, c, mode='valid')
        maximum = int(windows.max())
        location = np.where(windows == maximum)
        print(width, maximum, location[0][0] + 1, location[1][0] + 1)


if __name__ == "__main__":
    serial = int(sys.argv[1])
    calculate(serial)
    # filename = sys.argv[1]
    # with open(filename, 'r') as f:
    #     lines = f.readlines()
    #     calculate(lines)
