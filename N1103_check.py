solutionEXE = r".\N1103_Bugs_Integrated_Inc_Solution.exe"
myEXE = r".\N1103_Bugs_Integrated_Inc.exe"

inputFile = "N1103.in"
outputFile = "N1103.out"

from random import *
from time import *
import os

for i in range(114514):
    t = randint(1, 5)
    
    with open(inputFile, "w") as f:
        f.write(f"{t}\n")
        for __ in range(t):
            n = randint(1, 5)
            m = randint(1, 10)
            k = randint(0, m*n)
            
            bad = set()
            while len(bad) < k:
                bad.add((randint(1, n), randint(1, m)))
            
            f.write(f"{n} {m} {k}\n")
            f.write(
                '\n'.join( map(lambda x: f"{x[0]} {x[1]}", bad) )
            )
            f.write("\n")

    print("Waiting Solution...", end='\r')
    st0 = perf_counter()
    os.system(solutionEXE)
    s_last = perf_counter() - st0

    with open(outputFile, "r") as f:
        sol = f.read()

    print("Waiting My Program...", end='\r')    
    t0 = perf_counter()
    os.system(myEXE)
    last = perf_counter() - t0

    with open(outputFile, "r") as f:
        my = f.read()
    
    print(' '*50, end='\r')
    
    if sol != my:
        print(f"Wrong Answer on test {i} ({last:.4f}/{s_last:.4f}sec)")
        # 第几行不一样
        sol_sp = sol.split('\n')
        my_sp = my.split('\n')
        for l in range(len(sol_sp)):
            if sol_sp[l] != my_sp[l]:
                print(f"Line {l} is different!")
                print(f"Read {my_sp[l]}")
                print(f"Expect {sol_sp[l]}")
        input()
    elif last > 1.75:
        print(f"Time Limit Exceed on test {i} ({last:.4f}/{s_last:.4f}sec)")
        input()
    else:
        print(f"Accepted on test {i} ({last:.4f}/{s_last:.4f}sec)")
    
    sleep(0.05)