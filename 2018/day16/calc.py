import sys
import re
from collections import defaultdict
from collections import deque

opcodes = {
        'addr': lambda a,b,reg: reg[a] + reg[b],
        'addi': lambda a,b,reg: reg[a] + b,
        'mulr': lambda a,b,reg: reg[a] * reg[b],
        'muli': lambda a,b,reg: reg[a] * b,
        'banr': lambda a,b,reg: reg[a] & reg[b],
        'bani': lambda a,b,reg: reg[a] & b,
        'borr': lambda a,b,reg: reg[a] | reg[b],
        'bori': lambda a,b,reg: reg[a] | b,
        'setr': lambda a,b,reg: reg[a],
        'seti': lambda a,b,reg: a,
        'gtir': lambda a,b,reg: a > reg[b],
        'gtri': lambda a,b,reg: reg[a] > b,
        'gtrr': lambda a,b,reg: reg[a] > reg[b], 
        'eqir': lambda a,b,reg: a == reg[b],
        'eqri': lambda a,b,reg: reg[a] == b,
        'eqrr': lambda a,b,reg: reg[a] == reg[b], 
        }


def calculate(lines):

    calc = True
    i = 0
    
    numberWithThreeOrMoreOpCodes = 0 

    operations = defaultdict(list)    
    ops = defaultdict(set)
    while calc and i < len(lines):
        calc = 'Before' in lines[i] 
        if calc: 
            opCodesNumbers = defaultdict(int) 
            inps = lines[i:i+4]
            regBefore = [int(x) for x in re.findall('\d+', inps[0])] 
            regAfter  = [int(x) for x in re.findall('\d+', inps[2])] 
            regops = [int(x) for x in re.findall('\d+', inps[1])]
            #print(regBefore, regAfter, regops)
            operations[regops[0]].append((regBefore, regAfter, regops))

            for op, f in opcodes.items():
                value = f(regops[1], regops[2], regBefore)
                #print(op, regBefore, value, regAfter[regops[3]])
                if regAfter[regops[3]] == value:
                    ops[regops[0]].add(op)
                    opCodesNumbers[op] += 1
            if len(opCodesNumbers) > 2:
                #print(op)
                numberWithThreeOrMoreOpCodes+= 1
            i+=4
    print(numberWithThreeOrMoreOpCodes)        
    print(ops)
    print(i)

    opsDict = {}

    totalOps = set()
    for k,v in ops.items():
        totalOps.update(v)

    singleOps = [ (k,v) for k,v in ops.items() if len(v) == 1]
    print(f'Found single ops: {singleOps}')
    foundOps = set()
    for k,v in singleOps:
        opsDict[k] = list(v)[0]
        foundOps.update(v)

 
    while foundOps != totalOps:
        opsToFind = [ (k,v) for k,v in ops.items() if len(v - foundOps) == 1]
        for k,v in opsToFind:
            if len(v - foundOps) == 1: 
                op = v - foundOps
                foundOps.update(v - foundOps)
                opsDict[k] = list(op)[0]
                break

    #totalOpOps = set([k for k,v in opcodes.items()])
    #if totalOpOps - totalOps:
    #    opnumbers = set([k for k, _ in opsDict.items()])
    #    totalOpNumbers = set(range(len(opcodes)))
    #    missing = totalOpNumbers - opnumbers
    #    opsDict[missing] = totalOpOps= totalOps


    for x in sorted(opsDict.items(), key = lambda x : x[0]):
        print(x[0], x[1])
    reg = [0]* 4
    for l in lines[i:]:
        if len(l.strip()):
            regOp = [ int(x) for x in re.findall('\d+', l)]
            op = opsDict[regOp[0]]
            f = opcodes[op]
            value = f(regOp[1], regOp[2], reg)
            reg[regOp[3]] = value
    print(reg)
            


if __name__=='__main__':

    filename = sys.argv[1]

    with open(filename, 'r') as f:
        lines = f.readlines()
        calculate(lines)
