# scoreboard
from typing import Dict, Set
from collections import deque
from time import time, strftime, localtime
import json

data_file = ".scoreboard"

names: Set[str] = set()
scores: Dict[str, float] = dict()
history: Dict[str, deque[float]] = dict()  # 先前的扣分

def str_similar(s1: str, s2: str) -> float:
    if s1 == s2:
        return float("inf")
    
    s = 0.0
    s2_set = set(s2)
    for i in s1:
        if i in s2_set:
            s += 1
    
    return s

def select_user(name: str) -> str:
    if name in names:
        return name
    
    names_copy = list(names)
    names_copy.sort(key=lambda x: str_similar(x, name))
    names_copy.reverse()

    cnt = min(10, len(names_copy))
    for i in range(cnt):
        print(f"{i} {names_copy[i]}")
    
    sel = int(input("选择用户："))
    return names_copy[sel]

def save():
    with open(data_file, "w") as f:
        data = json.dumps({
            "names": list(names),
            "scores": scores,
            "history": {key: list(value) for key, value in history.items()}
        })
        f.write(data)

def load():
    try:
        with open(data_file, "r") as f:
            data = json.load(f)
            names.update(data["names"])
            scores.update(data["scores"])

            for key, value in data["history"].items():
                history[key] = deque(value)
    except Exception as e:
        print(e)


if __name__ == '__main__':
    load()
    while True:
        print("""scoreboard
reduce [user] [amount] 扣分
add [user] [amount] 加分
list 列出所有用户及其分数
list [user] 列出指定用户的记录
join [user] 添加用户
delete [user] 删除用户
quit 退出程序""")
        try:
            cmd = input("> ").split()
            op = cmd[0]

            if op == "reduce":
                user = select_user(cmd[1])
                amount = float(cmd[2])

                # 记录三天过期
                expired = time() - 3 * 24 * 60 * 60
                while len(history[user]) != 0 and history[user][0] < expired:
                    history[user].popleft()
                
                cnt = len(history[user])
                real_amount = amount ** (1 + cnt / 10)
                history[user].append(time())
                scores[user] -= real_amount
                print(f"{user} 实际扣分：{real_amount:.2f}")
            elif op == "add":
                user = select_user(cmd[1])
                amount = float(cmd[2])

                scores[user] += amount
                print(f"{user} 加分：{amount}")
            elif op == "list":
                if len(cmd) == 1:
                    # 所有用户
                    for name, score in sorted(scores.items(), key=lambda x: x[1], reverse=True):
                        print(f"{name} {score}")
                else:
                    # 指定用户
                    user = select_user(cmd[1])
                    print("现在分数：", scores[user])
                    print("近期扣分：")
                    for i in history[user]:
                        print(strftime("%Y-%m-%d %H:%M:%S", localtime(i)))
            elif op == "join":
                user = cmd[1]
                names.add(user)
                scores[user] = 100.0
                history[user]= deque()
            elif op == "delete":
                user = select_user(cmd[1])
                names.remove(user)
                del scores[user]
                del history[user]
            elif op == "quit" or op == "q":
                break
            save()
            print()
            print()
            print()
            print()
        except EOFError:
            break
        except KeyboardInterrupt:
            break
        except Exception as e:
            print(e)
