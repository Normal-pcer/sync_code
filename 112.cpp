#include "lib"

const int maxN = 300005;
int N, K;
int a[maxN];
int tail[maxN];  // 数值最小的、长度为 i 的最长上升子序列的尾部
int top = 0;

int main() {
    initDebug;
    optimizeIO;
    std::cin >> N >> K;
    upto(i, N)  std::cin >> a[i];
    upto(i, N) {
        if (a[i] >= tail[top]) {
            tail[++top] = a[i];
        } else {
            int *j = std::lower_bound(tail+1, tail+top+1, a[i]);
            *j = a[i];
        }
    }
    upto(i, N) {
        log("%3d", tail[i]);
    }
    return 0;
}