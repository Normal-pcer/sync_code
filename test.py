n = int(input())

from bisect import bisect_right
from math import gcd

def rank(lis, it):
    # print(lis, it)
    return len(lis) - bisect_right(lis, it)

p = [[] for _ in range(n)]
ids = [[] for _ in range(n)]
scs = [[] for _ in range(n)]
scs2 = []
w = [[] for _ in range(n)]


for i in range(n):
    p[i] = int(input())  # 人数
    ids[i] = list(map(int, input().split())) # 学号
    scs[i] = list(map(int, input().split())) # 参与度
    scs2.append(list(zip(ids[i], scs[i])))
    w[i] = list(map(int, input().split())) # 排名的给分

mainId = int(input())  #主角的学号

class Mixed:  # 带分数
    i: int
    a: int
    b: int
    def __init__(self, i, a, b):
        self.i = i
        self.a = a
        self.b = b
    
    def __str__(self):
        if self.a != 0:
            return f"{self.i}+{self.a}/{self.b}"
        else:
            return f"{self.i}"
    
    def format(self):
        self.i += self.a // self.b
        self.a %= self.b
        g = gcd(self.a, self.b)
        self.a //= g
        self.b //= g
    
    def __add__(self, other):
        return Mixed(self.i + other.i, self.a * other.b + self.b * other.a, self.b * other.b)
    
    def div(self, other: int):
        return Mixed(0, (self.i*self.b+self.a), self.b*other)


# 平均分数
res = Mixed(0,0,1)
cnt = 0
for le_i in range(n):  # 课
    # scs2[le_i].sort(key=lambda x: x[1])
    ids[le_i].sort()
    try:
        idx = ids[le_i].index(mainId)
    except:
        continue

    # rk = rank(scs[le_i], scs[le_i][idx])
    # rk = scs[le_i].index(scs[le_i][idx])
    # print(idx, scs[le_i])
    rk = 0
    for i in scs[le_i]:
        if i > scs[le_i][idx]:
            rk += 1
    # print(scs[le_i], idx, rk)

    res = res + Mixed(0, w[le_i][rk], 1)
    cnt+=1

res = res.div(cnt)
res.format()
print(res)

