#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<string> grid(n);
    for (int i = 0; i < n; ++i) {
        cin >> grid[i];
    }

    // 构建前缀和数组
    vector<vector<int>> prefix(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            prefix[i][j] = prefix[i-1][j] + prefix[i][j-1] - prefix[i-1][j-1] + (grid[i-1][j-1] == '#');
        }
    }

    long long result = 0;

    // 固定行范围 [x1, x2]
    for (int x1 = 1; x1 <= n; ++x1) {
        for (int x2 = x1; x2 <= n; ++x2) {
            // 滑动窗口检查列区间 [y1, y2]
            int y1 = 1;
            for (int y2 = 1; y2 <= m; ++y2) {
                // 计算当前列区间内的钉子数量
                int cnt = prefix[x2][y2] - prefix[x1-1][y2] - prefix[x2][y1-1] + prefix[x1-1][y1-1];
                // 如果钉子数量超过 1，移动 y1
                while (cnt > 1 && y1 <= y2) {
                    y1++;
                    cnt = prefix[x2][y2] - prefix[x1-1][y2] - prefix[x2][y1-1] + prefix[x1-1][y1-1];
                }
                // 统计合法的列区间
                result += (y2 - y1 + 1);
            }
        }
    }

    cout << result << endl;
    return 0;
}