#include <cstdio>
#include <cmath>
#include <algorithm>
#include <string>
#define range(from, to) for(int i=from; i<=to; i++)

int N;
int totalRage = 1;  // 乘算
int totalFxxk;      // 加算
int result = 2147483647;
struct Stone {
    int rage = 1;
    int fxxk = 0;
    Stone() { this->rage = 1; this -> fxxk = 0; }
    Stone(int rage, int fxxk) { this->rage = rage; this->fxxk = fxxk; }
    void apply() { totalRage *= this->rage; totalFxxk += this->fxxk; }
    void undo() { totalRage /= this->rage; totalFxxk -= this->fxxk; }
};
Stone stones[11];
bool selected[11];

auto space(int num) {
    std::string result = "";
    range(1, num)  result += "  ";
    return result;
}

void dfs(int depth) {
////printf("%s=min(%d, |%d-%d|)\n", space(depth).c_str(), result, totalRage, totalFxxk);
    if (depth!=0)   result = std::min(result, abs(totalFxxk-totalRage));
    if (depth==N)   return;
    range(1, N) {
        if (selected[i])  continue;
    ////printf("%s:%d=Stone(%d, %d)\n", space(depth).c_str(), i, stones[i].rage, stones[i].fxxk);
        stones[i].apply();
        selected[i] = true;
        dfs(depth+1);
        selected[i] = false;
        stones[i].undo();
    }
}

int main() {
    scanf("%d", &N);  
    range(1, N) {
        int tempRage, tempFxxk;
        scanf("%d%d", &tempRage, &tempFxxk);
        stones[i] = Stone(tempRage, tempFxxk);
    };
    dfs(0);
    printf("%d\n", result);
    return 0;
}