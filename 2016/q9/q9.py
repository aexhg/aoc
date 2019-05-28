inp = open('input.txt').readlines()

part2 =True 

def decompress(s):
    print(s)
    if '(' not in s:
        return len(s)
    ret = 0
    while '(' in s:
        ret += s.find('(')
        s = s[s.find('('):]
        marker = s[1:s.find(')')].split('x')
        s = s[s.find(')') + 1:]
        if part2:
            ret += decompress(s[:int(marker[0])]) * int(marker[1])
        else:
            ret += len(s[:int(marker[0])]) * int(marker[1])
        s = s[int(marker[0]):]
    ret += len(s)
    return ret
for l in inp:
    print( decompress(l.strip())  )
