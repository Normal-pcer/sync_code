A = "N2171_Grape.exe -d"
B = "N2171.Solution.exe"

import os
import time
from random import *

exeFile = "P11206_[Cfz_Round_9]_Dove.exe"

for i in range(114514):
    print("On test", i)
    with open("P11206.in", "w") as f:
        N = 10
        # 生成一棵树
        f.write(f"1\n{N}\n")
        for i in range(2, 11):
            f.write(str(randint(1, i - 1)) + " " + str(i) + "\n")
    
    ret = os.system(exeFile + " < P11206.in > P11206.out")
    if ret:
        input("Runtime Error!")
    time.sleep(0.1)