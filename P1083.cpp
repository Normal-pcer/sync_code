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
template <typename T>
T chkMin(T &base, T cmp) {
    return (base = std::min(base, cmp));
}
#define chkMaxEx(base,exchange,other,cmp...) {auto __b__=base;if(__b__!=chkMax(base,##cmp)){exchange;} else other;}
#define chkMinEx(base,exchange,other,cmp...) {auto __b__=base;if(__b__!=chkMin(base,##cmp)){exchange;} else other;}
#define update(base,op,modify...) base=op((base),##modify)
#define ensure(v, con, otw) (((v) con)? (v): (otw))
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
namespace lib{}

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
namespace lib {
    namespace binary {  // 二分
        template <class T, class U, class Func>
        T lower_bound_mapping(
            T begin,
            T end,
            U val,
            Func &&mapping,
            T eps = 1
        ) {
            while (end-begin >= eps) {
                T mid = begin + (end-begin)/2;
                if (mapping(mid) < val) {
                    begin = mid + eps;
                } else {
                    end = mid;
                }
            }
            return begin;
        }

        template <class T, class U, class Func>
        T upper_bound_mapping(
            T begin,
            T end,
            U val,
            Func &&mapping,
            T eps = 1
        ) {
            while (end-begin >= eps) {
                T mid = begin + (end-begin)/2;
                if (mapping(mid) <= val) {
                    begin = mid + eps;
                } else {
                    end = mid;
                }
            }
            return begin;
        }
    }
}
using namespace lib;

#define ls (p<<1)
#define rs (p<<1|1)

namespace Solution {

    int N, M;
    const int _N = 1e6+5;

    int r[_N];
    int d[_N], s[_N], t[_N];

    struct Node {
        int l, r;
        int min;  // 区间最小值
        int t;  // 减法标记
    } tr[_N<<2];

    void push_up(int p) {
        tr[p].min = std::min(tr[ls].min, tr[rs].min);
    }

    void push_down(int p) {
        if (tr[p].t) {
            tr[ls].min -= tr[p].t;
            tr[rs].min -= tr[p].t;
            tr[ls].t += tr[p].t;
            tr[rs].t += tr[p].t;
            tr[p].t = 0;
        }
    }

    void build(int p, int l, int r) {
        tr[p].l = l, tr[p].r = r;
        if (l == r) {
            tr[p].min = Solution::r[l];
            return;
        }
        int mid = (l+r)>>1;
        build(ls, l, mid);
        build(rs, mid+1, r);
        push_up(p);
    }

    int query(int p, int l, int r) {
        if (tr[p].l >= l and tr[p].r <= r) {
            return tr[p].min;
        }
        push_down(p);
        int min = Infinity;
        if (tr[ls].r >= l)  chkMin(min, query(ls, l, r));
        if (tr[rs].l <= r)  chkMin(min, query(rs, l, r));
        return min;
    }

    void reduce(int p, int l, int r, int val) {
        if (tr[p].l >= l and tr[p].r <= r) {
            tr[p].min -= val;
            tr[p].t += val;
            return;
        }
        push_down(p);
        if (tr[ls].r >= l)  reduce(ls, l, r, val);
        if (tr[rs].l <= r)  reduce(rs, l, r, val);
        push_up(p);
    }
    
    void init() {
        io >> N >> M;
        from(i, 1, N)
            io >> r[i];
        from(i, 1, M)
            io >> d[i] >> s[i] >> t[i];
        build(1, 1, N);
    }

    void solve() {
        init();

        from(i, 1, M) {
            reduce(1, s[i], t[i], d[i]);

            if (query(1, 1, N) < 0) {
                printf("-1\n");
                printf("%d\n", i);
                return;
            }
        }

        printf("0\n");
    }
}


int main() {
    initDebug;

    never {
        freopen("P1083.in", "r", stdin);
        freopen("P1083.out", "w", stdout);
    }
    Solution::solve();
    return 0;
}