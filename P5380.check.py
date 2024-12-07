from random import *
import os
from time import perf_counter, sleep

def random_data():
    maxQ = 1000
    Q = randint(1, maxQ)
    maxX = 9
    maxY = 8
    init_pos = [(0, 0), (0, 1), (0, 2), (0, 3), (0, 4), (0, 5), (0, 6), (0, 7), (0, 8), (0, 9), 
                (8, 0), (8, 1), (8, 2), (8, 3), (8, 4), (8, 5), (8, 6), (8, 7), (8, 8), (8, 9)]
    qs = [(*choice(init_pos), randint(0, maxX), randint(0, maxY))for _ in range(Q)]
    return f"{Q}\n" + "\n".join(f"{x1} {y1} {x2} {y2}" for x1, y1, x2, y2 in qs)

def process_out(out_lines):
    # 去掉行尾空格
    return [i.rstrip() for i in out_lines]

def check(out, ans):
    lines = process_out(out.splitlines())
    ans_lines = process_out(ans.splitlines())

    if len(lines) != len(ans_lines):
        if (len(lines) > len(ans_lines)):
            return f"Read too many lines. Read {len(lines)}, expected {len(ans_lines)}"
        else:
            return f"Read too few lines. Read {len(lines)}, expected {len(ans_lines)}"
    else:
        for i in range(len(lines)):
            if lines[i] != ans_lines[i]:
                return f"""On line {i+1}.
Read: {lines[i]}
Expected: {ans_lines[i]}"""
        return None

    
IOfilename = "P5380"

inferior = "P5380_[THUPC2019]_鸭棋.exe" + f"< {IOfilename}.in > {IOfilename}.out"
solution = "P5380.sol.exe" + f"< {IOfilename}.in > {IOfilename}.ans"
TIME_LIMIT = 1

if __name__ == "__main__":
    for i in range(114514):
        # data = random_data()
        # with open(f"{IOfilename}.in", "w", newline="\n") as f:
        #     f.write(data)
        print("Waiting inferior...", ' '*50, end='\r')
        tm = perf_counter()
        ret = os.system(inferior)
        tm = perf_counter() - tm

        print("Waiting solution...", ' '*50, end='\r')
        os.system(solution)

        with open(f"{IOfilename}.out") as f1, open(f"{IOfilename}.ans") as f2:
            out = f1.read()
            ans = f2.read()
        
        chk_result = check(out, ans)
        if chk_result is not None:
            print(f"Wrong Answer on test {i} in {tm:.3f}s")
            print(chk_result)
            input()
        elif ret != 0:
            print(f"Runtime Error on test {i} in {tm:.3f}s")
            input()
        elif tm > 1:
            print(f"Time Limit Exceeded on test {i} in {tm:.3f}s")
            input()
        else:
            print(f"Accepted on test {i} in {tm:.3f}s")
        
        # sleep(0.05)
