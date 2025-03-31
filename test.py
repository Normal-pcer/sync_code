N = int(input())
s = input()
t = input()

cnt = ['_' for i in range(N) if s[i] != t[i]]
print(len(cnt))