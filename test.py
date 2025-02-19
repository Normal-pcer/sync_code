input()
s = input()
print(s.replace(r"\../\../", "w").replace(r"\../", "v").replace(".", ""))
