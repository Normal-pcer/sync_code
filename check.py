import random

for i in range(114514):
    with open("edit.in", "w") as f:
        s = ["".join([str(random.randint(0, 1)) for i in range(5)]) for _ in range(4)]
        f.write("1\n5\n")
        f.write("\n".join(s)+"\n")
    
    from os import system
    system(".\\sjy.exe < edit.in > edit.out")

    with open("edit.out", "r") as f:
        ans = f.read()
    
    system(".\\P11361_[NOIP2024]_编辑字符串.exe < edit.in > edit.out")
    with open("edit.out", "r") as f:
        res = f.read()
    
    if (res != ans):
        print("WA on test", i)
        break
    else:
        print("AC on test", i)