import sys
import re
import itertools
from collections import defaultdict
from collections import deque
from collections import namedtuple
import networkx as nx

P = namedtuple('P', 'w x y z')

def dist(u,v):
    return abs(u.w - v.w) + abs(u.x - v.x) + abs(u.y - v.y) + abs(u.z -v.z)

def calculate(lines):

    points = [P(* map(int, x.split(','))) for x in lines]
    G = nx.utils.UnionFind(points)

    for u,v in itertools.combinations(points,2):
        #print(u,v)
        if dist(u,v) <= 3:
            G.union(u,v)

    print(f'There are {len(list(G.to_sets()))} constellations')

    return G
if __name__=='__main__':

    filename = sys.argv[1]

    with open(filename, 'r') as f:
        lines = f.readlines()
        G = calculate(lines)
