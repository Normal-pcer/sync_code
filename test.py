N = int(input())
cnt = [0 for _ in range(N)]

for _ in range(3):
    M = int(input())
    for x in map(int, input().split()):
        cnt[x-1] += 1

ans = 0
for i in cnt:
    if i != 1:
        ans += 1
print(ans)