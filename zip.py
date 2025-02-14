import re

filename = input("filename: ")
with open(filename, "r", encoding="UTF-8") as f:
    content = f.read()

content = content.replace("    ", "")  # 缩进
content = re.sub(r"//.*", "", content)  # 单行注释


def remove_comment(s: str) -> str:  # 多行注释
    res = ""
    asterisk = False
    slash = False
    comment = False

    for ch in s:
        if ch == '*':
            if slash:
                comment = True
                slash = False
                res = res[:-1]
            elif not comment:
                res += "*"
            asterisk = True
        elif ch == "/":
            if asterisk:
                comment = False
                asterisk = False
            elif not comment:
                res += "/"
            slash = True
        else:
            slash = False
            asterisk = False
            if not comment:
                res += ch
    
    return res

content = content.replace("\\\n", " ")
for _ in range(5):
    content = content.replace("  ", " ")  # 空格
for _ in range(5):
    content = content.replace("\n\n", "\n")  # 换行
content = remove_comment(content)
content = re.sub(r"\* =", "dick", content)
content = re.sub(r"\s*=\s*", "=", content)
content = re.sub(r"\s*\+\s*", "+", content)
content = re.sub(r"\s*-\s*", "-", content)
content = re.sub(r"\s*,\s*", ",", content)
content = re.sub(r"dick", "* =", content)
content = content.replace("std::", "")

with open(filename, "w", encoding="UTF-8") as f:
    f.write(content)

