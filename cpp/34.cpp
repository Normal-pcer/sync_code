#include <cstdio>
#include <algorithm>
#include <string>
#define Infinity 1002

int memory[Infinity];  // 存储 dfs(n) 的值
int numbers[21];  // 递增
int minNumberIndex = 0;
int N, t;

inline std::string addSpace(int count, std::string base) {
    std::string result = base;
    for (int i = 1; i <= count; i++)  result = "    " + result;
    return (result);
}

inline int getInt() {
    int temp;
    scanf("%d", &temp);
    return temp;
}

long long dfs(int n, int depth) {  // n的拆分方式数量
    if (n <= 0)  return 0;
    if (n == numbers[1])  return 1;
    long long answer = 1;
    for (int i = minNumberIndex ? minNumberIndex : n;
        i >= 1; i--) {
        int copy = minNumberIndex;
        minNumberIndex = i;
        long long delta = dfs(n - numbers[i], depth + 1);
        answer += delta;
        printf(addSpace(depth, "[%d] %d %d\n").c_str(), n, i, delta);
        minNumberIndex = copy;
    }
    printf(addSpace(depth, ":[%d]=>%lld\n").c_str(), n, answer);
    return answer;
}

int main(int argc, char const* argv[]) {
    N = getInt();
    t = getInt();
    for (int i = 1; i <= N; i++)
        numbers[i] = getInt();
    std::sort(numbers + 1, numbers + 1 + N);
    printf("%lld\n", dfs(t, 0));
    return 1;
}