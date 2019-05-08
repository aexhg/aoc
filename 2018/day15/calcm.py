import sys
import re
import networkx as nx
from collections import defaultdict
from collections import deque


class Player: 
    def __init__(self, side, x, y, attack): 
        self.x = x
        self.y = y
        self.attack = attack
        self.hp = 200
        self.side = side
    def pt(self):
        return (self.x, self.y)
    def __repr__(self):
        return f'{self.side}, {self.pt()}, {self.hp}'

    @property
    def alive(self):
        return self.hp > 0

def neighbours(pt):

    for x in [(0,1), (0,-1), (1,0), (-1,0)]:
       yield (pt[0] + x[0], pt[1] + x[1]) 

def show(grid, players):
    g = sorted(grid.items(), key = lambda x: x[0])
    xmax = g[-1][0][0]
    ymax = g[-1][0][1]
    pd = { (p.x, p.y):p for p in players}

    i = 0
    s = ''
    for k,gg in enumerate(g):
        x = gg[0][0]
        y = gg[0][1]

        if x != i:
            print(s)
            s = ''
            i += 1
        if (x,y) in pd and pd[(x,y)].alive: 
            s += pd[(x,y)].side
        else:
            s += gg[1]
    print(s)
    for p in players:
        print(p)

def findTargets(players, player):
    return [p for p in players if p.side != player.side and p.alive]

def findGridPointsNextToPlayer(player, grid):
    return [ np for np in neighbours(player.pt())  if np in grid and grid[np] == '.']

def checkAdjOfTarget(player, targets):

    targetPoints = {t.pt(): t for t in targets}
    adjs = []
    for np in neighbours(player.pt()):
        if np in targetPoints:
            adjs.append(targetPoints[np])
    return adjs

def buildGraph(grid, excludedNodes):
    G = nx.Graph()
    for p, d in grid.items():
        if d == '.' and p not in excludedNodes:
            G.add_node(p)
            for n in neighbours(p):
                if n not in excludedNodes and testBoundary(n,grid):
                    G.add_edge(p,n)
    return G


def findClosest(grid, excludedNodes, start, targets):

    G = buildGraph(grid, excludedNodes)
    routes = []
    minDist = None
    for t in targets:
        try:
            sp = nx.shortest_path(G, start, t)
            if minDist is None or len(sp) <= minDist:
                minDist = len(sp)
                routes.append(sp)
        except nx.NetworkXNoPath: 
            continue
    return routes
    #if start not in graph:
    #    return [], None

    #seen = set()
    #q = deque([(start, 0)])
    #found_dist = None
    #closest = []
    #while q:
    #    cell, dist = q.popleft()
    #    if found_dist is not None and dist > found_dist:
    #        return closest, found_dist
    #    if cell in seen or cell in excluded_nodes:
    #        continue
    #    seen.add(cell)
    #    if cell in targets:
    #        found_dist = dist
    #        closest.append(cell)
    #    for n in graph.neighbors(cell):
    #        if n not in seen:
    #            q.append((n, dist + 1))
    #return closest, found_dist



def takeTurn(players, grid):


    def doAttack(player, targets):
        sortedTargets = sorted(targets, key = lambda x : (x.hp, x.pt()))
        if sortedTargets:
            sortedTargets[0].hp -= player.attack


    for p in sorted(filter(lambda x : x.alive, players), key=lambda x: x.pt()):
        if not p.alive:
            continue
        targets = findTargets(players, p)
        if not targets:
           return False
        adjTargets = checkAdjOfTarget(p, targets)
        
        if adjTargets:
            doAttack(p, adjTargets)
            continue
        

        excludeNodes = [x.pt() for x in players if x.alive and x != p ]
        targetNeighbours = [x for y in targets for x in neighbours(y.pt()) if testBoundary(x, grid) and x not in excludeNodes]
        paths = findClosest(grid, excludeNodes, p.pt(), targetNeighbours)
        if paths:
            chosenPath = sorted(paths)[0]
            p.x = chosenPath[1][0]
            p.y = chosenPath[1][1]
            adjTargets = checkAdjOfTarget(p, targets)
            if adjTargets:
                doAttack(p, adjTargets)
    return True
       

def testBoundary(x, grid):
    return x in grid and grid[x] != '#'
    
def calculate(lines, attack = 3):

    grid = dict()
    players = []

    for i,l in enumerate(lines):
        for j,c in enumerate(l.strip()):
            if c == '.' or c == '#': 
                grid[(i,j)] = c
            if c == 'G' or c == 'E':
                players.append(Player(c,i,j,attack))
                grid[(i,j)] = '.'
    #G = nx.Graph() 
    #for pt, g in grid.items():
    #    if g == '.':
    #        for n in neighbours(pt):
    #            if n in grid and grid[n] != '#':
    #                G.add_edge(pt, n)

    show(grid, players)
    battle = True
    i = 1
    while battle:
        #print(f't={i}')
        takeTurn(players, grid)
        battler = [(1, p.side, p.pt(), p.alive)  for p in players if findTargets(players,p) and p.alive]
        battle = any(battler)
        #show(grid, players)
        if battle:
            i += 1
    hps = [ x.hp for x in players if x.alive]
    show(grid, players)
    print(f'Remaining hit points: {hps} sum is {sum(hps)}')
    print(f'Time={i}, t * hp = {i * sum(hps)}')

if __name__=='__main__':

    filename = sys.argv[1]

    with open(filename, 'r') as f:
        lines = f.readlines()
        calculate(lines)
