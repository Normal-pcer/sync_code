/**
 * @link https://www.luogu.com.cn/problem/P3959
 * @link https://neooj.com:8082/oldoj/problem.php?id=3158
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

namespace lib{
	// template <const size_t MAXSIZE>
	struct IO {
		// char buf[MAXSIZE],*p1,*p2;
		// char pbuf[MAXSIZE],*pp;
		// inline char gc() {
		// 	if (p1==p2) p2=(p1=buf)+fread(buf,1,MAXSIZE,stdin);
		// 	return p1==p2?' ':*p1++;
		// }
        inline char gc() {
			return getchar();
        }
		inline bool blank(char ch) { return ch==' ' or ch=='\n' or ch=='\r' or ch=='\t'; }
		inline bool isd(char x) {return (x>='0' and x<='9');}

		template <class T>
		inline void scan(T &x) {
			double tmp=1;
			bool sign=0;
			x=0;
			char ch=gc();
			for (; not isd(ch); ch=gc()) if (ch=='-') sign=1;
			for (; isd(ch); ch=gc()) x=x*10+(ch^48);
			if (ch=='.') for (ch=gc(); isd(ch); ch=gc()) tmp*=.1f,x+=tmp*(ch^48);
			if (sign) x=-x;
		}
		inline void scan(char *s) {
			char ch=gc();
			for (; blank(ch); ch=gc());
			for (; not blank(ch); ch=gc()) *s++=ch;
			*s=0;
		}
		inline void scan(char &c) {
			for (c=gc(); blank(c); c=gc());
		}
		inline void scan(std::string &s){
			std::string().swap(s);
			char ch=gc();
			for (; blank(ch); ch=gc());
			for (; not blank(ch); ch=gc()) s.push_back(ch);
		}
		template <class T,class... Types>
		inline void scan(T &x,Types &...args){
			scan(x);
			scan(args...);
		}
		inline void push(const char &c) {
			// if (pp-pbuf==MAXSIZE) fwrite(pbuf,1,MAXSIZE,stdout),pp=pbuf;
			// *pp++=c;
            putchar(c);
		}
		template <class T>
		inline void write(T x) {
			if (x<0) x=-x,push('-');
			static char sta[40];
			int top=0;
			do {
				sta[top++]=x%10^48,x/=10;
			} while (x);
			while (top) push(sta[--top]);
		}
        inline void write(double x) {
            printf("%lf",x);
        }
        inline void write(float x) {
            printf("%f",x);
        }
        inline void write(char c) {
            push(c);
        }
		inline void write(std::string &s){
			for (auto i:s) push(i);
		}
        inline void write(const char *s){
			for (; *s; ++s) push(*s);
        }
		template <class T,class... Types>
		inline void write(T &x,Types &...args){
			write(x);
			write(args...);
		}

        template<class T=int>
        inline T read() {
            T x;
            scan(x);
            return x;
        }
        inline std::string input() {
            return read<std::string>();
        }

        // 流式输入输出
		template <class T>
        inline IO& operator>>(T&x) {
            scan(x);
            return *this;
        }
        template <class T>
        inline IO& operator<<(T&x) {
            write(x);
            return *this;
        }
	};
	IO io;
    const char endl = '\n';
}
#include <bits/stdc++.h>
namespace lib {
    namespace bit {
        template <class T, class Func>
        inline void traverse1(T x, Func&& f) {  // 按位遍历所有的 1
            for (; x; x&=x-1)  f(x&-x);
        }
        template <class T>
        inline bool greater(T x, T y) {  // 按位大于
            return x & ~y;
        }
    }
}
#include <bits/stdc++.h>
namespace lib {
    template <typename T, const unsigned long long sz>
    class MapArray {
    public:
        T arr[sz];
        MapArray() = default;
        MapArray(const MapArray<T, sz>&) = default;
        MapArray(std::initializer_list<T>& il) {
            std::copy(il.begin(), il.end(), arr);
        }

        T& operator[](const unsigned long long idx) {
            return arr[idx];
        }
        const T& operator[](const unsigned long long idx) const {
            return arr[idx];
        }
        T& operator()(const unsigned long long idx) {
            return arr[idx];
        }
        const T& operator()(const unsigned long long idx) const {
            return arr[idx];
        }
    };
}
using namespace lib;

namespace Solution {
    const int _N = 13, _M = 1e3+5, _v = 5e5+5;
    int N=io.read(), M=io.read();

    typedef uint16_t status;  // 状态
    // 用二进制位表示一个点是否已经被连接
    // 低位表示编号小的点
    // ! 点从 0 开始编号

    MapArray<int8_t, 1<<_N> lg2;
    int edge[_N][_N];  // 两点 i, j 之间的边长
    std::vector<uint8_t> conn[_N];  // 点 i 连接到的所有点
    status ext[1<<_N];  // 状态 s 扩展一层能到达的点

    int F[_N][1<<_N];  // 最远距离为 i，取点状态为 j，花费的最小值

    void calc_lg2() {
        // 预处理 lg2
        for (int i=0; i<N; i++) {
            for (int j=1<<i; j<(1<<(i+1)); j++) {
                lg2[j] = i;
            }
        }
    }

    void read_edges() {
        std::memset(edge, 0x3f, sizeof(edge));  // 初始化为无穷大，卡掉不存在的路
        int a, b, v;
        from(i, 0, N-1)  edge[i][i] = 0;  // 和自己的距离为 0（不知道是否有用
        from(i, 0, M-1) {
            io.scan(a, b, v);
            a-=1; b-=1;
            // edge[a][b] = edge[b][a] = v;
            // 边可以覆盖定义
            chkMin(edge[a][b], v);
            chkMin(edge[b][a], v);
            conn[a].push_back(b);
            conn[b].push_back(a);
        }
        // 预处理 ext
        for (status s=0; s<(1<<N); s++) {
            status res;
            bit::traverse1(s, [&res](status i) {
                for (auto t: conn[ lg2[i] ]) {
                    res |= (1<<t);
                }
            });
            ext[s] = res;  //* 难绷一开始算完 ext 忘赋值了
        }
    }

    void init() {
        calc_lg2();
        read_edges();

        // 避免发生意外转移，将 F 初始化为极大值
        std::memset(F, 0x3f, sizeof(F));
    }

    void solve() {
        init();

        // 初始化 dp 数组
        // 对于每个点——把它看作第 0 层，只取这个点时，花费为 0
        from(p, 0, N-1)  F[0][1<<p] = 0;
        
        from(i, 1, N-1) {
            for (status s=0; s<(1<<N); s++) {
                // 枚举状态 s 的子集
                for (status t=s; t; t=(t-1)&s) {
                    // 检验是否合法
                    // t 通过加边可以获得 s
                    // 考虑 t 是否合法
                    // 即 s-t(所有多出的点) 是否被 ext[t] 包含
                    // s^t <= ext[t]
                    status diff = s^t;
                    // log("Check: s=%d, t=%d, diff=%d, ext[t]=%d\n", s, t, diff, ext[t]);
                    if (bit::greater(diff, ext[t]) || !diff)  continue;
                    // log("Accepted!\n")
                    // 保证了这个 t 必然合法（通过加一层边可以转移到）
                    // 更新 dp 数组
                    // 其中，这次的花费是 i * L，L 为两点的距离
                    ll L = 0;
                    bit::traverse1(diff, [&](int d){
                        // 对于每一个多出来的点，计算 t 中离它最近的
                        update(d, lg2);
                        // log("d=%d:\n", d);
                        ll this_dist = 0x7fffffffffffffffLL;
                        bit::traverse1(t, [&](int p){
                            update(p, lg2);
                            // log("chkMin %lld, (%d)edge[%d][%d]\n", this_dist, edge[d][p], d, p)
                            chkMin(this_dist, (ll)edge[d][p]);
                        });
                        L += this_dist;
                    });
                    // log("chkMin (%d)F[%d][%d], (%d)F[%d][%d] + %lld*%d\n", F[i][s], i, s, F[i-1][t], i-1, t, L, i)
                    ll ans = F[i-1][t] + L*i;
                    if (ans >= Infinity) continue;
                    chkMin(F[i][s], (int)ans);
                }
            }                
        }

        never {
            from(i, 0, N-1) {
                from(j, 0, (1<<N)-1) {
                    if (F[i][j] >= 10000) printf("   INF");
                    else if (F[i][j] <= -1000)  printf("  -INF");
                    else printf("%6d", F[i][j]);
                }
                printf("\n");
            }
        }

        // 结论
        // 答案为 F[i][0b1111...] 的最小值
        status all = (1<<N) - 1;
        int ans = Infinity;
        from(i, 0, N-1)  chkMin(ans, F[i][all]);
        printf("%d\n", ans);
    }
}

int main() {
    initDebug;
    Solution::solve();
    return 0;
}