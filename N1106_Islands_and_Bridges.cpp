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
    ll F[1<<_N][_N][_N];  // 状态为 i 的路，其中最后经过的一个点是 j，倒数第二个是 k —— 最大的价值
    int C[1<<_N][_N][_N];  // 上述情况的方案数

    const auto lg2 = static_cast<int(*)(int)>(std::__lg);
    
    void init() {
        std::memset(F, -0x3f, sizeof(F));
        std::memset(C, 0, sizeof(C));
        from(i, 0, _N-1)  conn[i]=0;
        std::memset(v, 0, sizeof(v));
        io >> N >> M;
        from(i, 0, N-1)  io >> v[i];
        int s, t;
        upto(i, M) {
            io >> s >> t;
            conn[s-1][t-1] = conn[t-1][s-1] = true;
        }
    }

    void solve() {
        init();

        C[0][0][0] = 1;
        int max_st = (1<<N)-1;
        ll ans = 0, cnt = 0;
        from(i, 0, N-1) {
            F[1<<i][i][0] = v[i];
            C[1<<i][i][0] = 1;

            log("cnt %lld->", cnt);
            if (F[1<<i][i][0] > ans)  ans=F[1<<i][i][0], cnt=C[1<<i][i][0];
            else if (F[1<<i][i][0] == ans)  cnt += C[1<<i][i][0];
            log("%lld\n", cnt)
        }
        if (N==1) {
            printf("%lld %lld\n", ans, cnt);
            return;
        } else {
            ans = cnt = 0;
        }
        from(st, 0, max_st) {  // 转移前的状态
            // 接下来，依次枚举可能的末点
            bit::traverse1(st, [&](int last) {  // 倒数第一个点
                debug io.write("  ",last,"\n");
                int lg_last = lg2(last);
                bit::traverse1(~st&max_st, [&](int future) {  // 一个新的点
                    debug io.write("    ",future,"\n");
                    int lg_ftr = lg2(future);
                    if (not conn[lg_ftr][lg_last]) {
                        log("%d %d not connected\n", lg_ftr, lg_last)
                        return;
                    }
                    ll delta = v[lg_ftr];
                    delta += v[lg_ftr] * v[lg_last];
                    // log("%d ", delta)
                    auto &target = F[st|future][lg_ftr][lg_last];

                    if (__builtin_popcount(st^last) == 0) {
                        // 倒数第二个点不存在
                        // 限制第一步只能正向走
                        // if (future < last)  return;
                    log("(%lld)F[%d][%d][%d] <-(%lld)- (%lld)F[%d][%d][%d]\n", F[st|future][lg_ftr][lg_last], 
                                st|future, lg_ftr, lg_last, delta+ F[st][lg_last][0], F[st][lg_last][0], 
                                st, lg_last, 0)
                        if (F[st|future][lg_ftr][lg_last] < delta + F[st][lg_last][0]) {
                            target = delta + F[st][lg_last][0];
                            C[st|future][lg_ftr][lg_last] = C[st][lg_last][0];
                        } else {
                            C[st|future][lg_ftr][lg_last] += C[st][lg_last][0];
                        }
                    } 

                    bit::traverse1(st^last, [&](int second) {  // 倒数第二个点（可能不存在）
                        debug printf("      %d %lld\n", second, delta);
                        int lg_sec = lg2(second);
                        if (not conn[lg_sec][lg_last])  return;
                        ll tmp = delta;
                        if (conn[lg_ftr][lg_sec]) {
                            log("!")
                            tmp += v[lg_ftr]*v[lg_last]*v[lg_sec];
                        }
                        tmp += F[st][lg_last][lg_sec];
                        log("(%lld)F[%d][%d][%d] <-(%lld)- (%lld)F[%d][%d][%d]\n", F[st|future][lg_ftr][lg_last], 
                            st|future, lg_ftr, lg_last, tmp, F[st][lg_last][lg_sec], 
                            st, lg_last, lg_sec)
                        if (target < tmp) {
                            target = tmp;
                            C[st|future][lg_ftr][lg_last] = C[st][lg_last][lg_sec];
                        } else if (target == tmp) {
                            C[st|future][lg_ftr][lg_last] += C[st][lg_last][lg_sec];
                        }
                    });                    
                });
            });
        }

        from(lg_ftr, 0, N-1) {
            from (lg_last, 0, N-1) {
                if (F[max_st][lg_ftr][lg_last] > ans)  ans=F[max_st][lg_ftr][lg_last], cnt=C[max_st][lg_ftr][lg_last];
                else if (F[max_st][lg_ftr][lg_last] == ans)  cnt +=C[max_st][lg_ftr][lg_last];
            }
        }
        if (ans == 0)  cnt=0;
        printf("%lld %lld\n", ans, cnt/2);
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    always {
        freopen("N1106.in", "r", stdin);
        freopen("N1106.out", "w", stdout);
    }
    io >> T;
    while (T --> 0)
        Solution::solve();
    return 0;
}

/*
1
4 4
1 2 3 4
1 4
4 2
2 3
1 3
*/