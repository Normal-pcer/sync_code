/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=1100
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
template <class T=int>inline T read(){ T x=0;int f=1;char c;while((c=getchar())<'0'||c>'9')if(c=='-')f=-1;do{x=(((x<<2)+x)<<1)+c-'0';}while((c=getchar())>='0'&&c<='9');return x*f; }

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

using namespace lib;
/*
There are a few points on a plane, and some are fixed on the plane, some are not. 
一个平面上有一些点，其中有些是固定的，有些则不是。
We want to connect these points by some sticks so that all the points are fixed on the plane. 
我们希望用若干根木棍连接这些点，使得所有点都固定在平面上。
Of course, we want to know the minimum length of the sum of the sticks. 
当然，我们希望知道这些木棍总长度的最小值。
As in the images, the black points are fixed on the plane and red ones are not, which need to be fixed by sticks. 
如图所示，黑点是固定在平面上的，红点则需要用木棍固定。
All the points in the left image have been fixed. But the middle one is not, 
在左图中，所有的点已经固定，但中图中的点还没有固定。
which we need add one stick to fix those four points (the right image shows that stick). 
我们需要添加一根木棍来固定这四个点（右图展示了这根木棍）。
Triangle is steady, isn’t it?
三角形是稳定的，不是吗？
*/

// 对于每一个点，如果希望固定它，只需要找到另外两个点，分别和目标点连接即可。
// 我们可以枚举所有的点，作为「最后一个被连接的点」，尝试连接先前的所有点

namespace Solution {

    // 考虑状态压缩
    // 最大可能的点数 N<=18
    // 用 32 位整数，每一位表示一个点是(1)否(0)被固定
    // 小位对应小的编号
    typedef uint32_t status; 

    const int _N = 19;
    int N;
    struct Point {
        int x, y;
        bool fixed;
    } p[_N];  // ! 从 0 开始存

    double F[1<<_N]; // F[i][j] 表示状态为 j 的最小长度
    double distance[_N][_N];  // 记录两个点之间的距离

    void init() {
        std::memset(p, 0, sizeof(p));
        std::memset(F, 0x75, sizeof(F));
        std::memset(distance, 0, sizeof(distance));

        from(i, 0, N-1) {
            io >> p[i].x >> p[i].y >> p[i].fixed;
        }
    }

    void pre() {
        from(i, 0, N-1) {
            from(j, i+1, N-1) {
                distance[i][j] = distance[j][i] = 
                    sqrt((p[i].x-p[j].x)*(p[i].x-p[j].x) + (p[i].y-p[j].y)*(p[i].y-p[j].y));
            }
        }
    }

    // 指定状态下，让 cur 点固定需要增加的最少距离
    double best(status s, int cur) {
        // 找到距离的最小值和次小值
        double best = 1e300, second = 1e300;
        from(i, 0, N-1) {
            if (s&(1<<i)) {
                if (distance[cur][i] < best) {
                    second = best;
                    best = distance[cur][i];
                } else if (distance[cur][i] < second) {
                    second = distance[cur][i];
                }
            }
        }

        return best + second;
    }

    void solve() {
        init();
        pre();
        if (N==1 && p[0].fixed) {
            printf("0.000000\n");
            return;
        }
        from(i, 0, N) 
            from(j, i+1, N)
                if (p[i].fixed && p[j].fixed) {
                    F[(1<<i) | (1<<j)] = 0;
                    from(k, 1, N) 
                        chkMin(F[(1<<i) | (1<<j) | (1<<k)], distance[i][k] + distance[j][k]);  // 两条线
                }

        from(j, 0, (1<<N)-1) {  // 连接前的状态
            if (F[j] > 1e200)  continue;
            from(i, 0, N-1) {  // 枚举要连接的点
                if (p[i].fixed) {
                    chkMin(F[j | (1<<i)], F[j]);
                    continue;
                }
                if (j&(1<<i)) {  // 如果已经连接了，则不需要再连
                    continue;
                }
                // log("i = %d, j = %d\n", i, j);
                // log("(%lf)F[%d] ?= (%lf)F[%d] + (%lf)best(%d, %d)\n", F[j | (1<<i)], j | (1<<i), F[j], j, best(j, i), j, i);
                chkMin(F[j | (1<<i)], F[j] + best(j, i));
            }
        }

        if (F[(1<<N)-1] > 1e200) {
            io << "No Solution" << endl;
        } else {
            io << F[(1<<N)-1] << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    debug {
            freopen("N1100_input.in", "r", stdin);
        freopen("N1100_output.out", "w", stdout);
    }
    while( (Solution::N = io.read()) ) {
        Solution::solve();
    }
    return 0;
}