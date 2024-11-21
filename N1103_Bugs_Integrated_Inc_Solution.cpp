#include <iostream>
#include <cstring>
template <class T>
inline T read()
{
    register T data = 0, w = 1;
    register char ch = getchar();
    while (!isdigit(ch))
    {
        if (ch == '-')
            w = -1;
        ch = getchar();
    }
    while (isdigit(ch))
        data = data * 10 + ch - '0', ch = getchar();
    return data * w;
}
template <class T>
inline T read(register T &x) { return x = read<T>(); }
typedef long long ll;
using namespace std;

const int N = 151, M = 11;
int n, m, k, f[2][60000], 
pre[M],  // 前一行的放置状态
cur[M];  // 当前行的放置状态
bool v[N][M];  // 标记障碍物
const int pow3[M] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049};

// 三进制数组转为整数
inline int three_ten(int *a)
{
    int ans = 0;
    for (int i = 0; i < m; ++i)
        ans += a[i] * pow3[i];
    return ans;
}

// 整数转为三进制数组
inline void ten_three(int x, int *a)
{
    for (int i = 0; i < m; ++i)
        a[i] = x % 3, x /= 3;
}

// 尝试在当前状态下放置块
void dfs(
    int now,    // 当前行的行号 
    int j,      // 当前
    int last,   // 放置完的数量
    int state   // 当前状态
)
{
    f[now][state] = max(f[now][state], last);  // 更新状态
    if (j >= m)  // 边界条件，结束递归
        return;
    
    // 竖向
    if (j + 1 < m  // 横向不溢出
        && !pre[j] && !pre[j + 1] // 上一行符合要求
        && !cur[j] && !cur[j + 1] // 当前行符合要求
    ) {
        cur[j] = cur[j + 1] = 2;  // 完全占用
        dfs(now, j + 2, last + 1, three_ten(cur));  // 继续往下搜
        cur[j] = cur[j + 1] = 0;  // 解除占用
    }

    // 横向
    if (j + 2 < m && !cur[j] && !cur[j + 1] && !cur[j + 2])
    {
        cur[j] = cur[j + 1] = cur[j + 2] = 2;
        dfs(now, j + 3, last + 1, three_ten(cur));
        cur[j] = cur[j + 1] = cur[j + 2] = 0;
    }
    dfs(now, j + 1, last, state);
}
void Bugs_Integrated()
{
    read(n), read(m), read(k);
    memset(v, 0, sizeof v);
    for (int x, y; k--;)
    {
        read(x), read(y);
        v[x][y - 1] = 1;
    }
    memset(f[0], -1, sizeof f[0]);
    for (int i = 0; i < m; ++i)
        pre[i] = v[1][i] ? 2 : 1;
    int now = 0, tmp = three_ten(pre);
    f[now][tmp] = 0;
    for (int i = 2; i <= n; ++i)
    {
        now ^= 1;  // 滚动数组，等价于 +1
        memset(f[now], -1, sizeof f[now]);
        for (int j = 0; j < pow3[m]; ++j)  // 枚举状态
            if (f[now ^ 1][j] != -1)  // 上一行的这一列不等于 -1，即被遍历过
            {
                ten_three(j, pre);  // 转成三进制按位
                for (int k = 0; k < m; ++k)  // 对于每一位
                    cur[k] = v[i][k] ? 2 : (pre[k] ? pre[k] - 1 : 0);  // 从上一行的状态转移过来
                                                                    // 这是作为初始状态
                tmp = three_ten(cur);  // 转成压缩状态
                dfs(now, 0, f[now ^ 1][j], tmp);  // 在初始状态上进行 dfs，追加若干个块
            }
    }
    int ans = 0;
    for (int i = 0; i < pow3[m]; ++i)
        ans = max(ans, f[now][i]);
    printf("%d\n", ans);
}
int main()
{
    freopen("N1103.in", "r", stdin);
    freopen("N1103.out", "w", stdout);
    for (int t = read<int>(); t--;)
        Bugs_Integrated();
    return 0;
}
