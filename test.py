def max_score(N, X, Y, Z):
    intervals = []
    for i in range(N):
        ai, bi = map(int, input().split())
        intervals.append((ai, bi))
    intervals.sort(key=lambda x: x[0])
    max_score = 0
    p = 0
    while p < len(intervals):
        if intervals[p][0] <= p:
            max_score += Y
            p += 1
        elif intervals[p][1] < p:
            max_score += X
            p += 1
        else:
            max_score += Z
            p += 1
    return max_score

N, X, Y, Z = map(int, input().split())
intervals = []
for i in range(N):
    ai, bi = map(int, input().split())
    intervals.append((ai, bi))
max_score = max_score(N, X, Y, Z)
print(max_score)