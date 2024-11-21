/**
 * @link https://www.luogu.com.cn/problem/P11206
 */

#include <bits/stdc++.h>
#define upto(i,n) for(int i=1;i<=(n);i++)

int T;
namespace Solution {
    int N;
    const int _N = 1e5+5;

    int depth[_N];  // 到根节点（1）的距离

    std::vector<int> conn[_N];  // i 和 conn[i][j] 互相连接
    int parent[_N];  // 记录父亲节点
    int filling_queue_arr[_N];
    int filled[_N];
    std::queue<int> filling_queue;
    
    void init() {
        // 重设，防止多测数据互相污染
        std::memset(depth, 0, sizeof(depth));
        upto(i, _N)  conn[i].clear();
        std::memset(parent, 0, sizeof(parent));
        std::memset(filling_queue_arr, 0, sizeof(filling_queue_arr));
        std::memset(filled, 0, sizeof(filled));
        scanf("%d", &N);
        int x=0, y=0;
        upto(_, N-1) {
            scanf("%d%d", &x, &y);
            conn[x].push_back(y);
            conn[y].push_back(x);
        }
    }

    void dfs_depth(int p, int prev) {  // 计算深度
        parent[p] = prev;
        depth[p] = depth[prev] + 1;

        for (auto &dest: conn[p]) {
            if (dest == prev)  continue;
            dfs_depth(dest, p);
        }
    }

    bool check() {
        upto(i, N) {
            for (auto &dest: conn[i]) {
                if (filled[i] + filled[dest] > N+1) {
                    return false;
                }
            }
        }
        return true;
    }

    void solve() {
        init();
        dfs_depth(1, 0);  // 以 1 为根节点
        // 按照深度排序填充顺序
        upto(i, N)  filling_queue_arr[i] = i;
        std::sort(filling_queue_arr+1, filling_queue_arr+1+N, [](auto a, auto b) {
            return depth[a] > depth[b];
        });
        upto(i, N)  filling_queue.push(filling_queue_arr[i]);  // 压入队列

        int L=1, R=N;  // 区间最小值和最大值
        // 依次填充
        while (not filling_queue.empty()) {
            auto cur = filling_queue.front();  filling_queue.pop();
            if (filled[cur])  continue;  // 填过的就忽略
            // 尝试在该点填充一个最大的数
            filled[cur] = R; R--;  // set 的末指针用星号取值即为最大值
            // 在父节点填充一个最小的数
            if (parent[cur] != 0 /* 特判，根节点不做处理 */ && not filled[ parent[cur] ]) {
                filled[ parent[cur] ] = L; L++;
            }
        }

        // 全部填充完毕
        upto(i, N)  printf("%d ", filled[i]);
        assert(check());  // 结果正确
        printf("\n");
    }
}

int main() {
    scanf("%d", &T);
    while (T --> 0)
        Solution::solve();
    return 0;
}