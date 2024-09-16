lts = set("04689abdegopqunc")
import re

with open("1.txt") as f:
    words = [i.lower() for i in f.read().split()]

for w in words:
    if re.match(".*s.+t.+r.*", w):  print(w)