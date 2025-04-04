a, b, c, d, x, y = [0] * 6
x, y = map(int, input().split())

cpa, cpb = x, y

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

a = 0
for _ in range(64):
    b -= b
    b += y # is odd
    c -= c
    c += y
    c >>= 1
    c <<= 1
    b -= c
    d -= d
    d += x
    c -= c
    c -= b
    d ^= c
    c -= c
    c += x
    c -= b
    c -= d
    c >>= 1
    a += c
    y >>= 1
    x += x

print(a)
assert(cpa * cpb == a)
