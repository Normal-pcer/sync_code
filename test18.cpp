#include <cstdio>
#include <vector>

int main() {
    int n;
    scanf("%d", &n);
    std::vector<int> div_counts(n + 1, 0);

    // 筛法计算约数个数
    for (int i = 1; i <= n; ++i) {
        for (int j = i; j <= n; j += i) {
            ++div_counts[j];
        }
    }

    // 统计总和和最大值
    long long total = 0;
    int max_val = 0;
    for (int i = 1; i <= n; ++i) {
        total += div_counts[i];
        if (div_counts[i] > max_val) max_val = div_counts[i];
    }

    // 输出结果
    printf("平均值: %.2f\n最大值: %d\n", static_cast<double>(total)/n, max_val);
    return 0;
}
