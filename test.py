T = int(input())

for _ in range(T):
    N, K = map(int, input().split())
    n = N
    cnt = 0
    while n > 0:
        cnt += n % 3
        n = n // 3
    
    if cnt <= K and (K - cnt) % 2 == 0:
        print("Yes")
    else:
        print("No")