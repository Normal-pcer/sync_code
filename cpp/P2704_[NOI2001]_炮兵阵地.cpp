/**
 * @link https://www.luogu.com.cn/problem/P2704
 * @link https://neooj.com:8082/oldoj/submitpage.php?id=1105
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
#define update(base,op,modify...) base=op((base),##modify)
#define ensure(con, otw) ((con)? (con): (otw))
#define check(v, con, otw) (((v) con)? (v): (otw))
#define optional(ptr) if(ptr)ptr
#define never if(0)
#define always if(1)
#define bitOr(x,y) (((x)&(y))^(((x)^(y))|(~(x)&(y))))
#define Infinity 2147483647
#define putInt(n) printf("%d\n",(n))
#define compare(x,y,g,e,l) (((x)>(y))?(g):(((x)<(y))?(l):(e)))
bool DEBUG_MODE=false;
typedef long long ll; typedef unsigned long long ull;
inline void batchOutput(int *begin, int n, const char *format){upto(i, n)printf(format, begin[i]);printf("\n");} inline void batchOutput(int*begin, int n) {batchOutput(begin,n,"%3d ");}
#define batchOutput2d(b, r, c, fmt) upto(i,r){upto(j,c)printf(fmt,b[i][j]);printf("\n");}
namespace lib {
    template <typename T, const long long sz>
    class RollingArray {  // 滚动数组
    public: 
        T arr[sz];

        T &operator[](long long idx) {
            return arr[idx % sz];
        }
    };
}
int pct[1024];

// 重写 std::__popcount
namespace std {
    inline int __popcount(int x) {
        return pct[x];
    }
}
namespace Solution {
    const int _N = 100, _M = 10;
    int N, M;  // N 行 M 列
    typedef uint16_t status;  // 一列的状态；小行号对应低位；0 代表未被占用，1 代表被占用
    status map[_N];  // 第 i 行，第 j 列；0 表示平地，1 表示山

    lib::RollingArray<int[1<<_M][1<<_M], 4> F;  // 第 i 行状态为 j，第 i-1 行状态为 k 时军队数量最大值

    void init() {
        scanf("%d%d", &N, &M);
        char line[_M+5];
        log("%d %d\n", N, M);

        from(i, 0, (1<<M)-1) {
            pct[i] = __builtin_popcount(i);
        }

        from(r, 0, N-1) {
            scanf("%s", line);
            from(c, 0, M-1) {
                if (line[c] == 'H')  map[r] |= 1 << c;
            }
        }

        never {
            from(i, 0, N-1) {
                printf("%d: %d\n", i, map[i]);
            }
        }

        std::memset(F.arr, -0x3f, sizeof(F.arr));
    }

    void solve() {
        // Your code here...
        init();

        // 初始化
        // 0 行
        from(j0, 0, (1<<M)-1) {
            if (map[0] & j0 || j0&(j0<<1) || j0&(j0<<2))  continue;
            F[0][j0][0] = std::__popcount(j0);
        }

        // 1 行
        from(j0, 0, (1<<M)-1) {
            if (map[1] & j0 || j0&(j0<<1) || j0&(j0<<2))  continue;
            from(j1, 0, (1<<M)-1) {
                // log("%d %d %d %d %d %d\n", j0, j1, map[1]&j1, j1&j0, j1&(j1<<1), j1&(j1<<2));
                if (map[0] & j1 || j1&j0 || j1&(j1<<1) || j1&(j1<<2))  continue;
                F[1][j0][j1] = std::__popcount(j0) + std::__popcount(j1);
            }
        }
        
        from(i, 2, N-1) {  // 行号
            std::memset(F[i], -0x3f, sizeof(F[i]));  // 清空为极小值
            from(j0, 0, (1<<M)-1) {  // 当前行状态
                // 当前行的状态合法
                if (map[i] & j0 || j0&(j0<<1) || j0&(j0<<2))  continue;
                from(j1, 0, (1<<M)-1) {  // 上一行状态
                    if (map[i-1] & j1 || j1&j0 || j1&(j1<<1) || j1&(j1<<2))  continue;
                    from(j2, 0, (1<<M)-1) {  // 上上行状态
                        if (map[i-2] & j2 || j2&(j2<<1) || j2&(j2<<2) || j2&j1 || j2&j0)  continue;
                        log("%d %d %d %d\n", i, j0, j1, j2)
                        chkMax(F[i][j0][j1], F[i-1][j1][j2] + std::__popcount(j0));
                    }
                }
            }
        }

        debug {
            from(i, 0, N-1) {
                printf("Row %d\n", i);
                from(j, 0, (1<<M)-1) {
                    from(k, 0, (1<<M)-1) {
                        if (F[i][j][k] >= 10000)  printf("  INF");
                        else if (F[i][j][k] < -1000)  printf(" -INF");
                        else  printf("%5d", F[i][j][k]);
                    }
                    printf("\n");
                }
                printf("\n\n");
            }
        }

        int ans = 0;
        from(j0, 0, (1<<M)-1) {
            from(j1, 0, (1<<M)-1) {
                chkMax(ans, F[N-1][j0][j1]);
            }
        }

        printf("%d\n", ans);
    }
}


int main() {
    initDebug;
    Solution::solve();
    return 0;
}