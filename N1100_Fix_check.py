stdEXE = r"N1100_Fix_Solution.exe"
solutionEXE = r"N1100_Fix.exe -d"

inputFile = r"N1100_input.in"
outputFile = r"N1100_output.out"

import time
import os
from random import *

for i in range(114514):
    T = randint(1, 10)
    with open(inputFile, "w") as f:
        for _ in range(T):
            N = randint(1, 18)
            s = set()
            t = set()
            m = 100
            while len(s) < N:
                cur = (randint(0, m), randint(0, m))
                if not cur in s:
                    s.add(cur)
                    t.add((*cur, randint(0, 1)))
            
            f.write(f"{N}\n")
            for pt in t:
                f.write(f"{pt[0]} {pt[1]} {pt[2]}\n")
        f.write("0\n")
    
    stdStartTime = time.perf_counter()
    print("Waiting std...", end='\r')
    os.system(stdEXE)
    stdTime = time.perf_counter() - stdStartTime
    
    with open(outputFile, "r") as f:
        stdOutput = f.read()
    
    solStartTime = time.perf_counter()
    print("Waiting solution...", end='\r')
    returns = os.system(solutionEXE)
    solTime = time.perf_counter() - solStartTime

    with open(outputFile, "r") as f:
        solOutput = f.read()
    print(" "*50, end='\r')

    if returns != 0:
        print(f"Runtime Error on Test {i}")
        input()
    elif stdOutput != solOutput:
        print(f"Wrong Answer on Test {i}({solTime:.4f}/{stdTime:.4f})")
        print(f"Read: {solOutput}")
        print(f"Expected: {stdOutput}")
        input()
    elif solTime > 1.5:
        print(f"Time Limit Exceeded on Test {i}({solTime:.4f}/{stdTime:.4f})")
        input()
    else:
        print(f"Accepted on Test {i}({solTime:.4f}/{stdTime:.4f})")
    
    # time.sleep(0.1)