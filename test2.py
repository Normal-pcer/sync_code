a, b, c, d, x, y = [0] * 6
a, b = map(int, input().split())

cpa, cpb = a, b

# y 为 0，返回 0
# y 为 1，返回 x
def condition(x: int, y: int):
    return ((x - y) - (x ^ (-y))) >> 1

def condition2(x: int, y: int):
    tmp = x  # tmp = x ^ (-y)
    tmp2 = 0
    tmp2 -= y  # tmp2 = -y
    tmp ^= tmp2

    x -= y
    x -= tmp
    x >>= 1
    return x

def isOdd(x: int):
    tmp = x
    tmp >>= 1
    tmp <<= 1
    x -= tmp
    return x

res = 0
for _ in range(64):
    res += condition2(a, isOdd(b))
    b >>= 1
    a += a

assert(cpa * cpb == res)
print(res)
