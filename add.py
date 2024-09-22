from sys import argv
n1 = argv[1]
n2 = argv[2]

#// print(n1, n2)

with open("out.txt", "w") as f:
    f.write(str(int(n1)+int(n2)))