# 压缩 #. 组成的二进制字符串

def handleString(s: str):
    segs = []
    prev = '#'
    for i in range(len(s)):
        if s[i] != prev:
            segs.append(0)
        else:
            segs[-1] += 1
        prev = s[i]
    
    ans = ''
    for x in segs:
        ans += chr(ord('A') + x)
    return ans

from sys import stdin
import re

input = stdin.read
strings = [x[1:-1] for x in re.findall(r'".*?"', input())]

zipped = [handleString(x) for x in strings]
# 65 个一组
for i in range(0, len(zipped), 65):
    print(', '.join([f'"{x}"' for x in zipped[i:i+65]]))
