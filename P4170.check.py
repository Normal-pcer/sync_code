import os
from random import *
from time import perf_counter as pf

tj = "solution.exe"
sb = "P4170_[CQOI2007]_涂色.exe"

for i in range(114514):
    print(f"Running on test {i}")
    N = 6
    n = randint(1, N)
    s = [chr(ord('a')+randint(0, 25)) for _ in range(n)]
    s = ''.join(s)

    with open(f"P4710.in", "w") as f:
        f.write(s + '\n')
    
    print('等待题解', ' '*50, end='\r')
    os.system(f"{tj} < P4710.in > tj.out")
    print('等待程序', ' '*50, end='\r')
    t = pf()
    os.system(f"{sb} < P4710.in > sb.out")
    t = pf() - t

    with open("tj.out", "r") as f:
        tj_out = f.read()
    with open("sb.out", "r") as f:
        sb_out = f.read()

    if tj_out != sb_out:
        print(f"WA on test {i} in {t:.3f}s")
        print(f"tj.out: {tj_out}")
        print(f"sb.out: {sb_out}")
        break
    elif t > 1:
        print(f"TLE on test {i} in {t:.3f}s")
        break
    else:
        print(f"AC on test {i} in {t:.3f}s")
