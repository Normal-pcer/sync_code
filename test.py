T = int(input())
for _ in range(T):
    lis = [*map(int, input().split())][1:]
    ans = 0
    for i in lis:
        ans |= i
    
    print(ans * pow(2, len(lis) - 1, 998244353) % 998244353)
