import sys
import re
from collections import defaultdict
from collections import deque

class P:

    def __init__(self, x, y):
        self.x = x
        self.y = y

    @property
    def up(self):
        return P(self.x, self.y-1)
    
    @property
    def down(self):
        return P(self.x,self.y+1)

    @property
    def left(self):
        return P(self.x-1,self.y)

    @property
    def right(self):
        return P(self.x+1,self.y)

    def __hash__(self):
        return hash(self.__repr__())

    def __eq__(self, o):
        return self.x == o.x and self.y == o.y

    def __repr__(self):
        return f'x={self.x},y={self.y}'


def calculate(lines):
    
    def doread(l, r):
        x = re.findall(r+'=(\d+)(..(\d+))?', l)[0]
        if len(x[1]):
            return int(x[0]), int(x[2])
        return int(x[0]), None

    clayPoints = set()
    settled = set()
    for l in lines:
        x1, x2 = doread(l, 'x')
        y1, y2 = doread(l, 'y')
        if x2 is None:
            for y in range(y1, y2 + 1):
                clayPoints.add(P(x1, y))
        else:
            for x in range(x1, x2 + 1):
                clayPoints.add(P(x, y1))

    springSource = P(500,0)
    maxy = max([p.y for p in clayPoints])

    flowing = deque()
    flowing.append(springSource)


    while flowing:
        p = flowing.pop()
        
        if p.down not in clayPoints and p.down.y < maxy:
            flowing.append(p.down)
        elif p.down in clayPoints:
            spread(p.down)

    def spread(p):
        spl = spreadDir(p, 'left')
        spr = spreadDir(p, 'right')


    def spreadDir(p,lr):
        spreading = deque()
        spreading.append(p)

        while spreading:
            p = spreading.pop()
            pd = getattr(p, lr)
            if pd not in clayPoint and (pd.down in clayPoints or pd.down in settled):


if __name__=='__main__':

    filename = sys.argv[1]

    with open(filename, 'r') as f:
        lines = f.readlines()
        calculate(lines)
