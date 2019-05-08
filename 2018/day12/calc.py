import sys
import re
from collections import defaultdict
from collections import deque

def calculate2(lines):
    initial = lines[0].split(':')[1].strip()
    combos = defaultdict(lambda : '.')

    for l in lines[2:]:
        inp = l.split('=>')
        combos[inp[0].strip()] = inp[1].strip()

    points = defaultdict(lambda : '.')
    for i,c in enumerate(initial):
        points[i]=c
    #print(points)

    generations = 1000
    xmin = -2
    xmax = len(initial) + 3
    for g in range(generations):
        ng = [[i,combos[''.join(map(lambda k: points[k], range(i-2,i+3)))
]] for i in range(xmin,xmax)]
        #for i in range(xmin,xmax):
        #    substr = ''.join(map(lambda k: points[k], range(i-2,i+3)))
        #    c = combos[substr]
        #    ng.append([i,c])
        #    #print(i, substr, combos[substr], c, ng)
        for i,c in ng:
            points[i] = c
        xmin -= 2
        xmax += 2
    #print(points)
    finalState = ''.join([points[i] for i in range(xmin,xmax)])
    #print(finalState)
    total = sum(i for i,c in points.items() if c =='#')
    print(f'Total: {total}')


def calculate(lines):

    initial = lines[0].split(':')[1].strip()
    combos = defaultdict(lambda : '.')

    for l in lines[2:]:
        inp = l.split('=>')
        combos[inp[0].strip()] = inp[1].strip()

    initialLength = len(initial)
    initial = '....' + initial + '....'
    generations = 1000
    for i in range(generations):
        size = len(initial)
        nextiter = ''
        for i in range(2, size - 2):
            combo = initial[i-2:i+3]
            ng = combos[combo]
            nextiter += ng
            #print(combo, ng, nextiter)
        initial = '....' + nextiter + '....'
        offset = (len(initial) - initialLength) / 2
    print(initial)
    bucketSum = sum(i-offset for i,p in enumerate(initial) if p == '#')
    #print(f'Final state: {initial}')
    print(f'Final bucket score: {bucketSum}')

if __name__=='__main__':

    filename = sys.argv[1]

    with open(filename, 'r') as f:
        lines = f.readlines()
        calculate(lines)
        calculate2(lines)
