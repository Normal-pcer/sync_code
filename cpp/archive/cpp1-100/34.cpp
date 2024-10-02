#include <cstdio>
#include <string>
#include <algorithm>

//// int usedNumbers[21];  // 递增排列
int usableNumbers[21];  // 递增排列
int minUsable = 0;  // 0 表示暂无此限制
int N, TARGET;
long long totallyCount;  // 结果总数

// bool isUsed(int number) {
//     for (int i=1; i<=TARGET; i++) 
//         if (usedNumbers[i] == number)
//             return true;
//     return false;
// }

std::string repeatString(const std::string& str, int n) {  
    std::string result;
    for (int i = 0; i < n; ++i) {  
        result += str;  
    }  
    return result;  
}  

void dfs(int target, int depth) {  // 获取target的拆分方式数量
    if (target == 0)    totallyCount++;
    if (target <= 0)     return;
    int copy = minUsable;
    for (int i=(minUsable? minUsable+1: 1); i<=N; i++) {
        minUsable = i;
        //// printf( (repeatString("  ", depth)+"[%d]%d\n").c_str(), target, usableNumbers[i] );
        dfs( target - usableNumbers[i], depth+1 );
    }
    minUsable = copy;
}

int main() {
    scanf("%d%d", &N, &TARGET);
    for (int i=1; i<=N; i++)  scanf("%d", usableNumbers+i);
    std::sort(usableNumbers+1, usableNumbers+1+N);
    dfs(TARGET, 0);
    printf("%lld\n", totallyCount);
    return 0;
}