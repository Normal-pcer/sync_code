import re
from time import perf_counter

if __name__ == '__main__':
    s = ""
    with open('1.in', 'r') as f:
        s = f.read()
    s = s.splitlines()

    begin = perf_counter()

    pattern = re.compile("z[^zpair]*?p.*?a[^zpair]*?i.*?r")
    ans = 0
    for ss in s:
        cnt = len(pattern.findall(ss))
        ans ^= cnt

    end = perf_counter()
    print(ans)
    print(end - begin, "sec")