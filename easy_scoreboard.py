# scoreboard
from typing import Dict, Set
from collections import deque
from time import time, strftime, localtime
import json

data_file = ".scoreboard"

names: Set[str] = set()
scores: Dict[str, float] = dict()
history: Dict[str, deque[float]] = dict()  # 先前的扣分
free_give: Dict[str, float] = dict()  # 剩余的免费额度
prev_update = 0.0 # 上次统计免费额度的时间

# 免费赠予额度，一周刷新一次
FREE_GIVE_REFRESH = 7 * 24 * 60 * 60
FREE_GIVE_AMOUNT = 10.0

# 初始分数
INIT_SCORE = 100.0

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
            "history": {key: list(value) for key, value in history.items()},
            "free_give": free_give,
            "prev_update": prev_update,
        })
        f.write(data)

def load():
    try:
        with open(data_file, "r") as f:
            global prev_update
            data = json.load(f)
            names.update(data["names"])
            scores.update(data["scores"])
            free_give.update(data["free_give"])
            prev_update = data["prev_update"]

            for key, value in data["history"].items():
                history[key] = deque(value)
        
        # 补全不存在的值
        for name in names:
            if name not in free_give:
                free_give[name] = FREE_GIVE_AMOUNT
            if name not in history:
                history[name] = deque()
            if name not in scores:
                scores[name] = INIT_SCORE
    except Exception as e:
        print(repr(e))


if __name__ == '__main__':
    load()

    if time() - prev_update > FREE_GIVE_REFRESH:
        for name in names:
            free_give[name] = FREE_GIVE_AMOUNT
        prev_update = time()

    while True:
        print("""scoreboard
reduce [user] [amount] 扣分
add [user] [amount] 加分
list 列出所有用户及其分数
list [user] 列出指定用户的记录
join [user] 添加用户
delete [user] 删除用户
give [source] [target] [amount] 赠予
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
                print(f"{user} 加分：{amount:.2f}")
            elif op == "list":
                if len(cmd) == 1:
                    # 所有用户
                    for name, score in sorted(scores.items(), key=lambda x: x[1], reverse=True):
                        print(f"{name} {score}")
                else:
                    # 指定用户
                    user = select_user(cmd[1])
                    print(f"现在分数：{scores[user]:.2f}")
                    print("近期扣分：")
                    for i in history[user]:
                        print(strftime("%Y-%m-%d %H:%M:%S", localtime(i)))
                    print(f"剩余免费赠送额度：{free_give[user]:.2f}")
            elif op == "join":
                user = cmd[1]
                names.add(user)
                scores[user] = INIT_SCORE
                history[user]= deque()
                free_give[user] = FREE_GIVE_AMOUNT
            elif op == "delete":
                user = select_user(cmd[1])
                names.remove(user)
                del scores[user]
                del history[user]
            elif op == "give":
                source, target, amount = cmd[1], cmd[2], float(cmd[3])

                source_prev_score, target_prev_score = scores[source], scores[target]

                if source not in names or target not in names:
                    print("用户不存在")
                    continue
                free = min(free_give[source], amount)
                free_give[source] -= free
                other = amount - free
                scores[source] -= other / 3
                scores[target] += amount

                print(f"{source} 赠送了 {target} {amount} 分。")
                print(f"{source}: {source_prev_score:.2f} -> {scores[source]:.2f}")
                print(f"{target}: {target_prev_score:.2f} -> {scores[target]:.2f}")
                print(f"{source} 剩余免费赠送额度：{free_give[source]:.2f}")
            elif op == "quit" or op == "q":
                break
            save()
            print()
            print()
        except EOFError:
            break
        except KeyboardInterrupt:
            break
        except Exception as e:
            print(repr(e))
