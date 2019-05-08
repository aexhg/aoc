import sys
import re
import numpy as np
from collections import defaultdict
from collections import deque
from collections import namedtuple
from scipy.optimize import minimize  

P = namedtuple('P', 'x y z')

def calculate(lines):

    d = defaultdict(int)
    for l in lines:
        x,y,z,r = re.findall('-?\d+', l)
        p = P(int(x), int(y), int(z))
        d[p] = int(r)
    
    nanoLargestSignal = max(d.items(), key = lambda x: x[1])
    inRange = 0

    for b,_ in d.items():
        dist = abs(nanoLargestSignal[0].x - b.x) + abs(nanoLargestSignal[0].y - b.y) + abs(nanoLargestSignal[0].z - b.z) 
        if dist <= nanoLargestSignal[1]:
            inRange += 1
    print(inRange)

    def nInRange(p):
        x = p[0]
        y = p[1]
        z = p[2]
        dist = 0
        for b,r in d.items():
            dist +=   abs(x - b.x) + abs(y - b.y) + abs(z - b.z)
        return dist

    def inBoundingBox(a,b):

        nInBox = 0
        for p, r in d.items():

            if p.x >= a.x and p.x <= b.x and p.y >= a.y and p.y <= b.y and p.z >= a.z and p.z <= b.z:
                nInBox += 1
        return nInBox

    o = minimize(nInRange, (0,0,0), method='Nelder-Mead')
    print(o)

if __name__=='__main__':

    filename = sys.argv[1]

    with open(filename, 'r') as f:
        lines = f.readlines()
        calculate(lines)
