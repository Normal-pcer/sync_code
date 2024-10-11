A = "N1106_Islands_and_Bridges.exe"
B = "N1106.Solution.exe"

import os
import time
from random import *
from math import ceil

for i in range(114514):
    _Q = 20
    _N = 13
    _V = 100
    
    Q = randint(1, _Q)
    
    with open("N1106.in", "w", newline='\n') as f:
        f.write(f"{Q}\n")
        for _ in range(Q):
            N = randint(0, _N)
            M = randint(0, int(N*N/3))
            v = [randint(1, _V) for _ in range(N)]
            conn = set()
            while len(conn) < M:
                ele = (randint(1, N), randint(1, N))
                if ele[0]>ele[1]:  ele=ele[::-1]
                if ele[0]==ele[1]:  continue
                conn.add(ele)
            
            f.write(f"{N} {M}\n")
            f.write(" ".join(map(str, v)) + "\n")
            for a, b in conn:
                f.write(f"{a} {b}\n")
    
    
    print("Waiting A...", end='\r')
    timerA = time.perf_counter()
    ret = os.system(A)
    timerA = time.perf_counter() - timerA
    with open("N1106.out", "r") as f:
        ansA = f.read()
    
    print("Waiting B...", end='\r')
    timerB = time.perf_counter()
    os.system(B)
    timerB = time.perf_counter() - timerB
    with open("N1106.out", "r") as f:
        ansB = f.read()
    
    if ret != 0:
        print(f"Runtime Error on Test {i}({timerA:.4f}/{timerB:.4f} sec)")
        print(f"Returns: {ret}")
        input()
    elif timerA>2:
        print(f"Time Limit Exceed on Test {i}({timerA:.4f}/{timerB:.4f} sec)")
        input()
    elif ansA != ansB:
        print(f"Wrong Answer on Test {i}({timerA:.4f}/{timerB:.4f} sec)")
        la = ansA.splitlines()
        lb = ansB.splitlines()
        
        if len(la) < len(lb):
            print(f"Read: {la[len(lb):]}\nExpected: None\nin line {len(la)}")
        elif len(lb) < len(la):
            print(f"Read: None\nExpected {lb[len(la):]}\nin line {len(lb)}")
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