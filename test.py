import random

random.seed(745184)

N, Q = list(map(int, input().split()))
A = list(map(int, input().split()))
B = list(map(int, input().split()))

s1 = [0]
s2 = [0]

log = {}
def rand(kkk):
    if not kkk in log:
        log[kkk] = random.randint(0, 18446744073709551615)
    return log[kkk]

for i in range(N):
    s1.append(s1[i]+rand(A[i]))
    s2.append(s2[i]+rand(B[i]))

for _ in range(Q):
    l1, r1, l2, r2 = list(map(int, input().split()))
    p = s1[r1] - s1[l1-1]
    q = s2[r2] - s2[l2-1]
    print("Yes" if p==q else "No")
