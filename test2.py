import random
from time import perf_counter
from os import system


a = "test.exe < 1.txt > 2.txt"
b = "N1058_猴子.exe < 1.txt > 3.txt"

def pro(s: str):
    lis = s.splitlines()
    s = '\n'.join([i.rstrip() for i in lis])
    return s

for __ in range(114514):
    N = random.randint(1, 300)
    D = random.randint(1, 10000)
    M = random.randint(1, N)
    lis = [[random.randint(0, 10000), 0]] + [[random.randint(0, 10000), random.randint(0, 100)] for _ in range(N)]
    for i in range(1, len(lis)):
        lis[i][1] += lis[i-1][1]
    inp = f"{N} {D} {M}\n{'\n'.join([f'{i[0]} {i[1]}' for i in lis])}\n"

    with open('1.txt', 'w', newline='\n') as f:
        f.write(inp)
    tm = perf_counter()
    ret = system(a)
    tm = perf_counter() - tm

    with open('2.txt', 'r') as f:
        inferior = pro(f.read())
    
    system(b)
    with open("3.txt", 'r') as f:
        solution = pro(f.read())
    
    if ret != 0:
        input('Runtime Error')
    elif tm > 1:
        input('Time Limit Exceed')
    elif inferior != solution:
        input('Wrong Answer')
    else:
        print('Test {}: Accepted in {:.2f} seconds'.format(__, tm))
