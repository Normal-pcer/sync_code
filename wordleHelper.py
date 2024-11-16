from typing import List, Tuple, Set

import csv
import re

class Word:
    text: str
    meaning: str

    def __init__(self, text: str, meaning: str):
        self.text = text
        self.meaning = meaning

def warn(msg: str):
    print("[警告] {}".format(msg))


class Limit:
    ensured: List[Tuple[int, str]]
    exist: List[Tuple[int, str]]
    without: List[str]

    def __init__(self, ensured: List[Tuple[int, str]] | None = None, exist: List[Tuple[int, str]] | None = None, 
                    without: List[str] | None = None):
        self.ensured = ensured or list()
        self.exist = exist or list()
        self.without = without or list()
    
    def match(self, source: str) -> bool:
        # 匹配确定的字母
        for slot, char in self.ensured:
            if source[slot] != char:
                return False
            else:
                source = source[:slot] + '.' + source[slot+1:]
        
        # 匹配存在性已知的字母
        for slot, char in self.exist:
            for match in re.finditer(char, source):
                span = match.span()
                if span != slot:
                    pos1, pos2 = span
                    source = source[:pos1] + '.' + source[pos2:]  # 去掉这个字母
                    break
            else:  # 循环被跳出
                return False
        
        # 匹配其他字母
        for char in self.without:
            if char in source:
                return False
        return True

if False:
    lim = Limit(
        [(0, 'a')],
        [(1, 'p')],
        ['p', 'l', 'e']
    )
    while True:
        s = input()
        print(lim.match(s))
    exit(0)


if __name__ == "__main__":
    words: List[Word] = list()
    length = int(input("单词长度 (默认为 5): ") or 5)

    with open("../EnWords.csv", "r", encoding="UTF-8") as f:
        reader = csv.reader(f)
        for row in reader:
            if len(row) != 2:
                warn("无法解析的行：" + str(row))
            else:
                if len(row[0]) == length:
                    words.append(Word(row[0], row[1]))

    # print(sorted([len(i.text) for i in words])[::-1])

    try:
        limits: List[Limit] = list()
        def match(s: str) -> bool:
            for i in limits:
                if not i.match(s):
                    return False
            return True
        while True:
            lim = Limit()
            guess = input("猜测的单词: ").lower()
            if guess == '/debug':
                for l in limits:
                    print(l.ensured, l.exist, l.without)
            elif guess == '/reset':
                lim.ensured = list()
                lim.exist = list()
                lim.without = list()
            elif guess == '/print':
                solutions = [i for i in words if match(i.text)]
                if (len(solutions) > 50):
                    if input(f"可能性过多({len(solutions)})，仍要显示？[y/n]").lower() != 'y':
                        continue
                
                for sol in solutions:
                    print(sol.text, sol.meaning)
            elif guess == '/undo':
                if len(limits):
                    limits.pop()
                else:
                    print("没有未撤销的步骤")
            else:
                res = [*map(int, input("结果(1=灰, 2=黄, 3=绿): "))]
                for i in range(len(res)):
                    if res[i] == 1:
                        lim.without.append(guess[i])
                    elif res[i] == 2:
                        lim.exist.append((i, guess[i]))
                    else:
                        lim.ensured.append((i, guess[i]))
            
                limits.append(lim)
    except KeyboardInterrupt:
        exit(0)
    except EOFError:
        exit(0)