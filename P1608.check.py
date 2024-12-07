from random import *
import os
from time import perf_counter, sleep

def random_data():
    maxN = 5
    maxM = 10
    maxC = 10


    N = randint(3, maxN)
    M = randint(1, min((lambda n:n*(n-1))(N), maxM))

    # 生成随机图
    G = [(lambda mid:(randint(1, mid), randint(mid+1, N), randint(1, maxC)))(randint(2, N-1)) for _ in range(M)]

    data = f"{N} {M}\n" + "\n".join([" ".join(map(str, i)) for i in G]) + "\n"
    return data

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

    


inferior = "P1608_路径统计.exe < P1608.in > P1608.out"
solution = "test.exe < P1608.in > P1608.ans"
TIME_LIMIT = 1

if __name__ == "__main__":
    for i in range(114514):
        data = random_data()
        with open(f"P1608.in", "w", newline="\n") as f:
            f.write(data)
        
        print("Waiting inferior...", ' '*50, end='\r')
        tm = perf_counter()
        ret = os.system(inferior)
        tm = perf_counter() - tm

        print("Waiting solution...", ' '*50, end='\r')
        os.system(solution)

        with open(f"P1608.out") as f1, open(f"P1608.ans") as f2:
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
        
        sleep(0.05)
