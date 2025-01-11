a, b, c = map(int, input().split())

def f(x):
    if 0 == x:
        return 0
    elif x <= 24:
        return 1
    elif x <= 44:
        return 2
    elif x <= 84:
        return 3
    else:
        return 4

a, b, c = map(f, sorted([a, b, c]))
def check():
    if c == 3 and b == 2 and a == 1:
        return True
    if c == 4 and b == 3 and a == 2:
        return True
    if c == 4 and b == 4 and a == 3 or a == 2:
        return True
    if c == 4 and b == 4 and a == 4:
        return True
    return False

print("Yes" if check() else "No")