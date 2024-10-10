N=int(input())
for _ in range(N):
    x, m=input().split()
    if int(x)%2==0:
        print(m, 1)
    else:
        print(1, m)