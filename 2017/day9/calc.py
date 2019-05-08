import sys
import re
from collections import defaultdict
from collections import deque


def calculate(lines):

    group = 0
    s = ''.join([x.strip() for x in lines])
    inGarbage = False
    px = ''
    score = 0
    cancelNext = False
    garbageCount = 0
    for x in s:
        if not inGarbage and x == '{':
            group += 1
            score += group
        if not inGarbage and x == '}':
            group -= 1
        if x == '<' and not cancelNext and not inGarbage:
            inGarbage = True
            garbageCount -=1
        if inGarbage and not cancelNext and x != '!' and x != '>' :
            garbageCount += 1
        if x == '>' and not cancelNext: 
            inGarbage = False
        cancelNext = x == '!' and not cancelNext
        px = x
    print(group,score,garbageCount)



if __name__=='__main__':

    filename = sys.argv[1]

    with open(filename, 'r') as f:
        lines = f.readlines()
        calculate(lines)
