/**
 * 
 */

#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=(n);i++)
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define rev(i,e,b) for(int i=(e);i>=(b);i--)
#define main() main(int argc, char const *argv[])
#define optimizeIO std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
#define chkMax(base,cmp...) (base=std::max({(base),##cmp}))
#define chkMin(base,cmp...) (base=std::min({(base),##cmp}))
#define chkMaxEx(base,exchange,other,cmp...) {auto __b__=base;if(__b__!=chkMax(base,##cmp)){exchange;} else other;}
#define chkMinEx(base,exchange,other,cmp...) {auto __b__=base;if(__b__!=chkMin(base,##cmp)){exchange;} else other;}
#define ensure(v, con, otw) (((v) con)? (v): (otw))
#define never if constexpr(0)
#define always if constexpr(1)
#define bitOr(x,y) (((x)&(y))^(((x)^(y))|(~(x)&(y))))
#define Infinity 2147483647
#define compare(x,y,g,e,l) (((x)>(y))?(g):(((x)<(y))?(l):(e)))
bool DEBUG_MODE=false;
typedef long long ll; typedef unsigned long long ull;
inline void batchOutput(int *begin, int n, const char *format){upto(i, n)printf(format, begin[i]);printf("\n");} inline void batchOutput(int*begin, int n) {batchOutput(begin,n,"%3d ");}
#define batchOutput2d(b, r, c, fmt) upto(i,r){upto(j,c)printf(fmt,b[i][j]);printf("\n");}
namespace lib{}

using namespace lib;

int T;
namespace Solution {

    int N;
    const int _N = 1e5+5;

    int depth[_N];  // 到叶子节点的距离的最小值

    std::vector<int> conn[_N];  // i 和 conn[i][j] 互相连接
    int parent[_N];  // 记录父亲节点
    int filling_queue_arr[_N];
    int filled[_N];
    std::queue<int> filling_queue;
    std::set<int> numbers;
    
    void init() {
        std::memset(depth, 0, sizeof(depth));
        upto(i, _N)  conn[i].clear();
        std::memset(parent, 0, sizeof(parent));
        std::memset(filling_queue_arr, 0, sizeof(filling_queue_arr));
        std::memset(filled, 0, sizeof(filled));
        numbers.clear();
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
        log("parent[%d] = %d\n", p, prev);
        if (conn[p].size() == 1ULL and prev != 0) {  // 当前已经是叶子节点
            depth[p] = 0;
            return;
        }
        int res = 0x7fffffff;
        for (auto dest: conn[p]) {
            if (dest == prev) {  // 父亲节点
                continue;
            }
            dfs_depth(dest, p);  // 先处理子节点
            chkMin(res, depth[dest]+1);
        }
        depth[p] = res;
    }

    bool check() {
        upto(i, N) {
            for (auto &dest: conn[i]) {
                if (filled[i] + filled[dest] > N+1) {
                    log("\n%d+%d>N+1\n", i, dest)
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
            return depth[a] < depth[b];
        });
        upto(i, N)  filling_queue.push(filling_queue_arr[i]);

        upto(i, N)  numbers.insert(i);  // 初始可用所有数字
        // 依次填充
        while (not filling_queue.empty()) {
            auto cur = filling_queue.front();  filling_queue.pop();
            if (filled[cur])  continue;  // 填过的就忽略
            if (parent[cur] == 0)  continue;  // 根节点不填大数
            log("!%d %d\n", cur, depth[cur])
            debug for (auto &i: numbers) {
                log("%d ", i);
            }
            log("\n")
            // 尝试填充一个最大的数
            filled[cur] = *numbers.rbegin();
            log("filled[%d]=%d\n", cur, *numbers.rbegin())
            numbers.erase(*numbers.rbegin());
            // 在父节点填充一个最小的数
            if (parent[cur] != 0) {
                if (filled[ parent[cur] ] + filled[cur] <= N+1)  continue;
                if (filled[ parent[cur] ])  numbers.insert(filled[ parent[cur] ]);
                filled[ parent[cur] ] = *numbers.begin();
                log("filled[%d]=%d\n", parent[cur], *numbers.begin())
                numbers.erase(*numbers.begin());
            }
        }
        if (filled[1] == 0)  filled[1] = *numbers.begin();

        // 全部填充完毕
        upto(i, N)  printf("%d ", filled[i]);
        assert(check());
        printf("\n");
    }
}


int main() {;
    initDebug;
    scanf("%d", &T);
    while (T --> 0)
        Solution::solve();
    return 0;
}