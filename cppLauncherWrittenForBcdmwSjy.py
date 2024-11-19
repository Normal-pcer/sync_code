import os
import sys
import time

if __name__ == '__main__':
    name = sys.argv[1]
    begin = time.perf_counter()
    ret = os.system(name)
    last = time.perf_counter() - begin

    print("\n" + "-"*15 + "\nExited with code {} after {:.6f} seconds...".format(ret, last))
    os.system("pause")
