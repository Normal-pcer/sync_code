import re
import sys

MATCH = r"[^A-Za-z_][A-Za-z_]+[^A-Za-z_]"
input = sys.stdin.read

find = [i[1:-1] for i in re.findall(MATCH, input())]
cnt = {}
for i in find:
    if not i in cnt:
        cnt[i] = 0
    
    cnt[i] += len(i)

cnt_word = []
for i in cnt:
    cnt_word.append((cnt[i], i))

cnt_word.sort()
cnt_word.reverse()
for i, w in cnt_word[:50]:
    print(i, w)
