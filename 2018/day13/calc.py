import sys
import re
from collections import defaultdict
from collections import deque

def calculate(lines):
    pass

if __name__=='__main__':

    filename = sys.argv[1]

    with open(filename, 'r') as f:
        lines = f.readlines()
        calculate(lines)
