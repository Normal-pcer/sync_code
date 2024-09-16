#include <bits/stdc++.h>

int N;                      // 答案的位数
int P;                      // 模数
char ans[1000005] = {'1'};  // 首位为"1"，确保符合 N 位数的条件
int main() {
    std::ios::sync_with_stdio(false); std::cin.tie(0); std::cin.tie(0);  // 加速输入输出
    std::cin >> N >> P;

    // i 表示所有数位上数字之和的可能值
    // 对于 N 位数，这个取值为 1(1000...0)~9*N(9999...9)
    // 不难发现这个闭区间内所有的值都可以取到
    // 故 minRemainder 恒为 1 或 0（与 9*N 和 P 的相对大小有关）
    int minRemainder = (N*9 < P);       // 这里的 true 和 false 会分别隐式转换为 1 和 0

    std::fill(ans+1, ans+N+1, '0');     // 除了首位继续保持为1，其他数位均置0

    // 处理低位
    // 这里采取低位均为 9，最后加一个 1~9 的数凑数位和的策略
    // 由于首位已经有了 1，剩余数位和为 (minRm-1)
    // 由于原先的最小余数可能为0，减一会变为负数，所以加上模数再次取模确保非负
    minRemainder = (minRemainder - 1 + P) % P;
    int nineCount = minRemainder / 9;           // 需要填充的"9"的个数
    int remainAmount = minRemainder % 9;        // 剩余的一个数字
    ans[N] = '\0';                              // 字符串结束
    for (int i = N-1; i >= N - nineCount; i--)  // 从低位到高位填充"9"
        ans[i] = '9';
    ans[N-nineCount-1] += remainAmount;         // 最后加上剩余的数，确保数位和依然为 minRemainder
    std::cout << ans << std::endl;              // 输出答案
    return 0;
}
