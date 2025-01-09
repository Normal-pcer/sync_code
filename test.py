h, w = map(int, input().split())

try:
    kkk = list[str]()
    p = []
    for i in range(h):
        kkk.append(input())
        if 'o' in kkk[-1]:
            p.append((i, kkk[-1].index('o')))

    x1, y1 = p[0]
    x2, y2 = p[1]
    print(abs(x1 - x2) + abs(y1 - y2))
except Exception:
    pass