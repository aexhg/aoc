from collections import defaultdict
from collections import namedtuple
import numpy as np
import networkx as nx
import heapq as hq

P = namedtuple('P', 'x y')
depth = 510#8103
target = P(10,10)#P(9,758)
MOD = 3
bound = 3 

    #x = p.x
    #y = p.x
    #if x == y == 0: return 0
    #if x == target.x and y == target.y: return 0
    #if y == 0: return x * 16807 % MOD
    #if x == 0: return y * 48271 % MOD
    #return ((gindex(P(x-1, y)) + depth) *
    #        (gindex(P(x, y-1)) + depth) % MOD)

    #return (gindex(P(x,y)) + depth) % MOD % 3

def calculate():

    size = (target.x + bound) * (target.y + bound)
    grid = np.zeros(size).reshape((target.y + bound, target.x + bound))
    erosion = grid.copy()
    for x in range(target.x + bound):
        for y in range(target.y + bound):
            if x == 0 or y == 0:
                grid[y,x] = x * 16807 + y * 48271
            elif P(x,y) == target:
                grid[y,x] = 0
            else:
                grid[y,x] = erosion[y-1,x] * erosion[y,x-1]
            erosion[y,x] = (grid[y,x] + depth) % 20183

    grid = erosion % 3
    print(grid)                
    print(f'Part 1={np.sum(grid[:target.y+1, :target.x+1])}')

    #rocky = 0, wet = 1, narrow = 2

    # none = 0, torch = 1, climbing = 2
    allowedTools =  {0: [1,2], 1: [0, 2], 2: [1, 0]}

    G = nx.Graph()

    for x in range(target.x + bound):
        for y in range(target.y + bound):
            region = grid[y,x]
            tools = allowedTools[region]
            p = P(x,y)
            for t in tools[1:]:
                G.add_edge((p,tools[0]), (p, t), weight=7)

            for dx, dy in [[0,1],[0,-1],[1,0],[-1,0]]:
                q = P(x+dx, y+dy)
                if q.x >= 0 and q.y >= 0 and q.x < target.x + bound and q.y < target.y + bound:
                    nr = grid[q.y,q.x]
                    st = set(tools) & set(allowedTools[nr])
                    for t in st:
                        G.add_edge((p,t), (q,t), weight=1)
    t = nx.dijkstra_path_length(G, (P(0,0),1), (target,1))
    print(f'shortest length time: t={t}')

    times = defaultdict(lambda : float('inf'))
    times[(P(0,0),1)] = 0
    seen = set()
    q = []
    hq.heappush(q, (P(0,0),1))
    import pdb
    pdb.set_trace()
    while q:
        p, t = hq.heappop(q)
        if (p,t) in seen:
            continue
        seen.add((p,t))

        if (p,t) == (target,1):
            break
        region = int(grid[p.y, p.x])
        otherTools = set(allowedTools[region]) - set([t])
        for tt in otherTools:
            v = (p, tt)
            if v not in seen:
                time = times[(p,t)] + 7
                oldtime = times[v]
                if time < oldtime:
                    hq.heappush(q, v)
                    times[v] = time
        for dx, dy in [[0,1], [0,-1], [1,0], [-1,0]]:
            neighbour = P(p.x + dx, p.y + dy)
            if neighbour.x >= 0 and neighbour.y >= 0 and neighbour.x < target.x + bound and neighbour.y < target.y +bound:
                v = (neighbour, t)
                if v not in seen:
                    time = times[(p,t)] + 1
                    oldtime = times[v]
                    if time < oldtime:
                        times[v] = time
                        hq.heappush(q,v)
                #otherTools = set(allowedTools[region]) & set(allowedTools[int(grid[neighbour.y, neighbour.x])])
                #for ot in otherTools:
                #    v = (neighbour, ot)
                #    if v not in seen:
                #        time = times[(p,t)] + 1
                #        oldtime = times[v]
                #        if time < oldtime:
                #            hq.heappush(q,v)
                #            times[v] = time

    return G                

   

if __name__=='__main__':
    G = calculate()
