import sys
import re
from collections import defaultdict
from collections import deque


def calculate(num):
    
    e1 = 0
    e2 = 1

    s = '37'

    found = False
    k = len(s) + num + 10
    size = 2
    strNum = str(num)
    numSize = len(strNum) + 5
    while not found:
    #for i in range(k):
        n = int(s[e1]) + int(s[e2])
        d,m = divmod(n, 10)
        if d > 0:
            s += str(d)
            size += 1
        s += str(m)
        size += 1
        e1 = (e1 + int(s[e1]) + 1) % size
        e2 = (e2 + int(s[e2]) + 1) % size 
        if strNum in s[-numSize:]:
            found = True

    print(s[num:num+10])
    print(s.index(strNum))

if __name__=='__main__':
    n = int(sys.argv[1])
    calculate(n)
   # filename = sysv.argv[1]

   # with open(filename, 'r') as f:
   #     lines = filename.readlines()
   #     calculate(lines)
