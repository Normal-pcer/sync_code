from typing import List, Tuple, Dict, Set

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


class Limits:
    ensured: List[Tuple[int, str]]
    exist: List[Tuple[int, str]]
    without: List[str]
    count: Dict[str, Tuple[int, int]]

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
                # source = source[:slot] + '.' + source[slot+1:]
                pass
        
        # 匹配存在性已知的字母
        for slot, char in self.exist:
            for match in re.finditer(char, source):
                span = match.span()
                if span != slot:
                    # pos1, pos2 = span
                    # source = source[:pos1] + '.' + source[pos2:]  # 去掉这个字母
                    break
            else:  # 循环被跳出
                return False
        
        # 匹配其他字母
        for char in self.without:
            if char in source:
                return False
        return True

if False:
    lim = Limits(
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

    words = [Word("girl", "")]

    try:
        lim = Limits()
        while True:
            guess = input("猜测的单词: ").lower()
            if guess == '/debug':
                print(lim.ensured, lim.exist, lim.without)
            elif guess == '/reset':
                lim.ensured = list()
                lim.exist = list()
                lim.without = list()
            elif guess == '/print':
                solutions = [i for i in words if lim.match(i.text)]
                if (len(solutions) > 50):
                    if input(f"可能性过多({len(solutions)})，仍要显示？[y/n]").lower() != 'y':
                        continue
                
                for sol in solutions:
                    print(sol.text, sol.meaning)
            else:
                res = [*map(int, input("结果(1=灰, 2=黄, 3=绿): "))]
                l1 = list()
                l2 = list()
                l3 = list()
                letters: Dict[str, int] = dict()
                letters_tot: Dict[str, int] = dict()
                for i in range(len(res)):
                    if res[i] == 1:
                        l1.append(guess[i])
                    elif res[i] == 2:
                        l2.append((i, guess[i]))
                        letters[guess[i]] += letters.get(guess[i], 0) + 1
                    else:
                        l3.append((i, guess[i]))
                        letters[guess[i]] += letters.get(guess[i], 0) + 1
                    letters_tot[guess[i]] += letters_tot.get(guess[i], 0) + 1

                for ch in letters:
                    ch_min = letters[ch]
                    ch_max = letters_tot[ch]
                    



    except KeyboardInterrupt:
        exit(0)
    except EOFError:
        exit(0)