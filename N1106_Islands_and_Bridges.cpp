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
#define update(base,op,modify...) base=op((base),##modify)
#define ensure(con, otw) ((con)? (con): (otw))
#define check(v, con, otw) (((v) con)? (v): (otw))
#define optional(ptr) if(ptr)ptr
#define never if constexpr(0)
#define always if constexpr(1)
#define bitOr(x,y) (((x)&(y))^(((x)^(y))|(~(x)&(y))))
#define Infinity 2147483647
#define putInt(n) printf("%d\n",(n))
#define compare(x,y,g,e,l) (((x)>(y))?(g):(((x)<(y))?(l):(e)))
bool DEBUG_MODE=false;
typedef long long ll; typedef unsigned long long ull;
inline void batchOutput(int *begin, int n, const char *format){upto(i, n)printf(format, begin[i]);printf("\n");} inline void batchOutput(int*begin, int n) {batchOutput(begin,n,"%3d ");}
#define batchOutput2d(b, r, c, fmt) upto(i,r){upto(j,c)printf(fmt,b[i][j]);printf("\n");}

#include <bits/stdc++.h>
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
        std::string input() {
            char c = gc();
            std::string res="";
            while((c&&!blank(c)) || c==' ') {
                res.push_back(c);
                c = gc();
            }
            return res;
        }
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
using namespace lib;

int T;

namespace Solution {
    const int _N = 13;
    int N, M;
    int v[_N];
    std::bitset<_N> conn[_N];

    // 一条路径的“二进制状态”
    // 用一位表示一个岛屿是否被经过
    int F[1<<_N][_N][_N];  // 状态为 i 的路，其中最后经过的一个点是 j，倒数第二个是 k —— 最大的价值
    int C[1<<_N][_N][_N];  // 上述情况的方案数

    const auto lg2 = static_cast<int(*)(int)>(std::__lg);
    
    void init() {
        io >> N >> M;
        from(i, 0, N-1)  io >> v[i];
        int s, t;
        upto(i, M) {
            io >> s >> t;
            conn[s][t] = conn[t][s] = true;
        }
    }

    void solve() {
        init();

        C[0][0][0] = 1;
        int max_st = (1<<N)-1;
        from(i, 0, N-1) {
            F[1<<i][i][0] = v[i];
            C[1<<i][i][0] = 1;
        }
        from(st, 0, max_st) {  // 转移前的状态
            // 接下来，依次枚举可能的末点
            bit::traverse1(st, [&](int last) {  // 倒数第一个点
                bit::traverse1(~st&max_st, [&](int future) {  // 一个新的点
                    int delta = v[lg2(future)];
                    delta += v[lg2(future)] * v[lg2(last)];
                    int &target = F[st|future][lg2(future)][lg2(last)];

                    if (__builtin_popcount(st^last) == 0) {
                        // 倒数第二个点不存在
                        if (F[st|future][lg2(future)][lg2(last)] < delta) {
                            target = delta;
                            C[st|future][lg2(future)][lg2(last)] = C[st][lg2(last)][0];
                        }
                    } else {
                        C[st|future][lg2(future)][lg2(last)] += C[st][lg2(last)][0];
                    }

                    bit::traverse1(st^last, [&](int second) {  // 倒数第二个点（可能不存在）
                        log("%d %d %d %d %d\n", st, future, last, second, delta)
                        int tmp = delta;
                        if (conn[lg2(future)][lg2(second)]) {
                            log("!")
                            tmp += v[lg2(future)]*v[lg2(last)]*v[lg2(second)];
                        }
                        tmp += F[st][lg2(last)][lg2(second)];
                        log("(%d)F[%d][%d][%d] <-(%d)- (%d)F[%d][%d][%d]\n", C[st|future][lg2(future)][lg2(last)], 
                            st|future, lg2(future), lg2(last), tmp, F[st][lg2(last)][lg2(second)], 
                            st, lg2(last), lg2(second))
                        if (target < tmp) {
                            target = tmp;
                            C[st|future][lg2(future)][lg2(last)] = C[st][lg2(last)][lg2(second)];
                        } else if (target == tmp) {
                            C[st|future][lg2(future)][lg2(last)] += C[st][lg2(last)][lg2(second)];
                        }
                    });
                });
            });
        }

        int ans = 0;
        from(i, 0, N-1) {
            from(j, 0, N-1) {
                chkMax(ans, F[max_st][i][j]);
            }
        }

        printf("%d\n", ans);
    }
}


int main() {
    initDebug;

    io >> T;
    while (T --> 0)
        Solution::solve();
    return 0;
}