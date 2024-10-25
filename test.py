with open("duck.ans") as f:
    s = f.read()

lines = s.splitlines()
with open("duck.in") as f:
    s = f.read()

l2 = s.splitlines()[1:]
tuples = zip(lines, l2)

tuples = [(a, b) for a, b in tuples if a!='Invalid command']

new = ""
for a, b in tuples:
    print(a, b)
    new += b + "\n"

with open("duck2.in", "w") as f:
    f.write(new)