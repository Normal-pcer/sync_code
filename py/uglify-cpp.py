# uglify.py

import random
import re
import os
import functools

COMMAND_BASE = "code ..."

if not os.path.exists("input.cpp"):
    with open("input.cpp", "w", encoding="UTF-8") as f:
        f.write("")

print("请输入代码")
os.system(COMMAND_BASE.replace("...", "input.cpp"))
input("按Enter继续")

code = ""
with open("input.cpp", "r", encoding="UTF-8") as f:
    code = f.read()

symbol0 = "[\"\'].*?[\"\']"
symbol = "[a-zA-Z_0-9]+"
symbol2 = "([^a-zA-Z_0-9<]){}([^a-zA-Z_0-9>])"

tmpCode = re.sub(r'([^\\])".*?[^\\]"', "$1\"\"", code)

symbolList = list(set(re.findall(symbol0, tmpCode))) + list(set(re.findall(symbol, tmpCode)))
# print(symbolList)
symbolMap = dict()
count = 1
for s in symbolList:
    symbolMap[s] = '_'*count
    count += 1
pairs = list()
for s in symbolMap:
    v = symbolMap[s]
    pairs.append((s, v))

pairs.sort(key=functools.cmp_to_key(lambda a, b: len(a[0])-len(b[0])))


for pair in pairs:
    origin = pair[0]
    uglified = pair[1]
    if origin in ["define", "include"]:
        continue
    code = re.sub(symbol2.format(origin),
                  lambda d: d.group(1)+uglified+d.group(2), code)
    code = "#define {} {}\n".format(uglified, origin)+code

# code = ''.join(['' if random.random() < 0.5 else '\\\n'+i for i in code])

with open("result.cpp", "w", encoding="UTF-8") as f:
    f.write(code)

os.system(COMMAND_BASE.replace("...", "result.cpp"))
