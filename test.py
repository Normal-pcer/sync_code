N = int(input())

mem = dict[int, int]()
def f(x):
    if x == 0:  return 1
    if x in mem:  return mem[x]
    ans = f(x // 2) + f(x // 3)
    mem[x] = ans
    return ans

print(f(N))