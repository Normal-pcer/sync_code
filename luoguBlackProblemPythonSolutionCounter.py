import requests
import time
import re

REQUEST_COOKIE = {
    "C3VK": "ea8762",
    "__client_id": "58b769fcc8a336ce670dff641364cf046d54ece2",
    "_uid": "745184"
}

HEADER = {
    "accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7",
    "accept-language": "zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7",
    "cache-control": "max-age=0",
    "cookie": "C3VK=ea8762; __client_id=58b769fcc8a336ce670dff641364cf046d54ece2; _uid=745184",
    "referer": "https://www.luogu.com.cn/",
    "sec-ch-ua": "\"Google Chrome\";v=\"129\", \"Not=A?Brand\";v=\"8\", \"Chromium\";v=\"129\"",
    "sec-ch-ua-mobile": "?0",
    "sec-ch-ua-platform": "\"Windows\"",
    "sec-fetch-dest": "document",
    "sec-fetch-mode": "navigate",
    "sec-fetch-site": "same-origin",
    "sec-fetch-user": "?1",
    "upgrade-insecure-requests": "1",
    "user-agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/129.0.0.0 Safari/537.36"
}

PROBLEM_LIST_BASE_URL = "https://www.luogu.com.cn/problem/list?difficulty=7&page={}"
SOLUTION_BASE_URL = "https://www.luogu.com.cn/problem/solution/{}"

session = requests.Session()
session.cookies.update(REQUEST_COOKIE)

def getUrl(url):
    print("获取页面：", url)
    return session.get(url, headers=HEADER).text

if __name__ == '__main__':
    for i in range(15, 114514):
        print(f"获取第 {i} 页", PROBLEM_LIST_BASE_URL.format(i), ' '*50)
        problemList = getUrl(PROBLEM_LIST_BASE_URL.format(i))
        problemItemRe = r'<a href="(P\d+)">.*</a>'

        problems = re.findall(problemItemRe, problemList)
        solutions = list(map(SOLUTION_BASE_URL.format, problems))

        for i in range(len(solutions)):
            content = getUrl(solutions[i])
            if content.find("print") != -1 and content.find("printf") == -1 and content.find("#include") == -1:
                print("找到 Python 题解:", problems[i])
        
                with open("record.txt", "a") as f:
                    f.write(f"{problems[i]}\n")
        time.sleep(2)