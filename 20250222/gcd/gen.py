N = 3 * 10**3

lis = []

for i in range(N // 2):
    lis.append(i)
    lis.append(i)

with open("gcd5.in", "w", newline = "\n") as f:
    f.write(str(N) + "\n")
    for x in lis:
        f.write(str(x + 2))
        f.write(" ")
    f.write("\n")

    f.write(str(N) + "\n")
    for i in range(N // 2):
        f.write(str(i * 2 + 1) + " " + str(i * 2 + 2) + "\n")
    for i in range(N // 2):
        f.write(str(i * 2 + 1) + " " + str(i * 2 + 2) + "\n")
