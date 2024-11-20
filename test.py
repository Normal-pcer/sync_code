import csv
from math import ceil, floor
from typing import Tuple, List, Dict

if __name__ == '__main__':
    log: Dict[str, List[int]] = {}
    with open("./CSPS2024.csv", "r", encoding="UTF-8") as f:
        reader = csv.reader(f)
        for row in reader:
            try:
                score = int(row[5])
                province = row[1]
                grade = row[-1]

                if not grade.startswith("高"):
                    continue
                if province in log:
                    log[province].append(score)
                else:
                    log[province] = [score]
            except IndexError:
                pass
    
    tot = []
    for pro in log:
        log[pro].sort()
        log[pro] = log[pro][::-1]
        sub = log[pro][:5]
        tot += sub
        print(pro, sub)
    
    tot += [0]
    tot.sort()
    tot = tot[::-1]
    length = len(tot)
    take = round(length / 5)
    print(tot[:take])
    # print('{:.2f}'.format(tot.index(376) / length * 100 )+ '%')


