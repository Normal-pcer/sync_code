A = "N2171_Grape.exe -d"
B = "N2171.Solution.exe"

import os
import time
from random import *

for i in range(114514):

    with open("N2171.in", "w", newline='\n') as f:
        
        b, c, d, a = sorted([randint(0, 5),randint(0, 5),randint(1, 5), randint(1, 5)])
        v = [randint(-10, 10) for _ in range(a)]
        f.write(f"{a} {d} {b} {c}\n")
        f.write(" ".join(map(str, v)))
        f.write("\n")
    
    
    print("Waiting A...", end='\r')
    timerA = time.perf_counter()
    ret = os.system(A)
    timerA = time.perf_counter() - timerA
    with open("N2171.out", "r") as f:
        ansA = f.read()
    
    print("Waiting B...", end='\r')
    timerB = time.perf_counter()
    os.system(B)
    timerB = time.perf_counter() - timerB
    with open("N2171.out", "r") as f:
        ansB = f.read()
    la = [*filter(lambda x:x != '', ansA.split('\n'))]
    lb = [*filter(lambda x:x != '', ansB.split('\n'))]
    if ret != 0:
        print(f"Runtime Error on Test {i}({timerA:.4f}/{timerB:.4f} sec)")
        print(f"Returns: {ret}")
        input()
    elif timerA>2:
        print(f"Time Limit Exceed on Test {i}({timerA:.4f}/{timerB:.4f} sec)")
        input()
    elif la != lb:
        print(f"Wrong Answer on Test {i}({timerA:.4f}/{timerB:.4f} sec)")

        if len(la) > len(lb):
            print(f"Read: {la[len(lb)]}\nExpected: None\nin line {len(la)}")
        elif len(lb) > len(la):
            print(f"Read: None\nExpected {lb[len(la)]}\nin line {len(lb)}")
        else:
            for i in range(len(la)):
                if la[i] != lb[i]:
                    print(f"Read: {la[i]}\nExpected: {lb[i]}\nin line {i}")
                    break
        input()
    else:
        print(f"Accepted on Test {i}({timerA:.4f}/{timerB:.4f} sec)", end='.')
        print("" if timerA>timerB else " Better.")
    time.sleep(0.1)