/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=1103
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
#define val const auto
#define var auto
#define lambda(expr, args...) [&](args) {return expr;}
#define select_func(ret, name, args...) static_cast<ret(*)args>(name)

#include <bits/stdc++.h>

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
#include "./libs/mod.hpp"
#include "./libs/rollingArray.hpp"
#include <bits/stdc++.h>
namespace lib {
    template <typename T, const long long sz>
    class RollingArray2 {  // 滚动数组 - 另一种实现思路
    public: 
        T arr[sz];

        T &operator[](long long idx) {
            return arr[idx % sz];
        }
    };
}
#include <bits/stdc++.h>
namespace lib {
    template <typename T, const unsigned long long sz>
    class MapArray {
    public:
        const T arr[sz];
        MapArray() = default;
        MapArray(const MapArray<T, sz>&) = default;

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

/*
Bugs Integrated, Inc. is a major manufacturer of advanced memory chips. 
Bugs Integrated, Inc. 是一个制造内存芯片的主要厂商。
They are launching production of a new six terabyte Q-RAM chip. 
他们正在启动一个新的 6TB Q-RAM 芯片制造项目。
Each chip consists of six unit squares arranged in a form of a 2 × 3 rectangle. 
每一个芯片由六个排列成 2*3 矩形的的单位方块组成。
The way Q-RAM chips are made is such that one takes a rectangular plate of silicon 
制造 Q-RAM 芯片的方法是把矩形硅片分割成
divided into N × M unit squares. Then all squares are tested carefully and the bad ones 
N*M 的单位方块。接下来小心地测试所有的方块并
are marked with a black marker.  Finally, the plate of silicon is cut into memory chips. 
将坏块标黑。最终，硅片被切割成了若干个芯片。
Each chip consists of 2 × 3 (or 3 × 2) unit squares. Of course, no chip can contain any 
每一个芯片包含 2*3 或 3*2 的单位方块。当然，芯片不能包括
bad (marked) squares. It might not be possible to cut the plate so that every good unit 
坏块。可能不存在切割方案使得所有的好块
square is a part of some memory chip. The corporation wants to waste as little good squares 
都被制造成芯片。公司希望尽可能少地浪费好块。
as possible. Therefore they would like to know how to cut the plate to make the maximum 
因此，他们想知道如何切割硅片才能制造出尽可能多的芯片。
number of chips possible. [Task] You are given the dimensions of several silicon plates 
你将被告知几个硅片的尺寸
and a list of all bad unit squares for each plate. Your task is to write a program that 
和每个硅片上所有坏块的位置。你的任务是编写一个程序，
computes for each plate the maximum number of chips that can be cut out of the plate.
可以计算每块硅片上可以切割出多少个芯片。
#Input
The first line of the input file consists of a single integer D (1 <= D<= 5), denoting the 
输入文件的第一行包含一个整数 D (1 <= D<= 5)，表示
number of silicon plates. D blocks follow, each describing one silicon plate. The first line 
硅片的数量。接下来的 D 部分，描述了每个硅片。
of each block contains three integers N (1 <= N<= 150), M (1 <= M<= 10), K (0 <= K<= MN) 
每块的第一行包含三个空格分割的整数 N, M, K，
separated by single spaces. N is the length of the plate, M is its height and K is the number 
N 是硅片的长度，M 是其高度，K 是坏块的数量。
of bad squares in the plate. The following K lines contain a list of bad squares. Each line 
接下来的 K 行包含坏块的列表。每行
consists of two integers x and y (1 <= x<= N, 1<= y<= M) – coordinates of one bad square 
包含两个整数 x 和 y (1 <= x<= N, 1<= y<= M) - 坏块的坐标。
(the upper left square has coordinates [1, 1], the bottom right is [N,M]).
（左上角是[1, 1]，右下角是[N, M]）
#Output
For each plate in the input file output a single line containing the maximum number of 
对于输入文件中的每个硅片，输出一行，包含可以切割出
memory chips that can be cut out of the plate.
的芯片的最大数量。
*/


int D;

namespace Solution {
    const int pow3[] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561, 
                    19683, 59049, 177147, 531441, 1594323, 4782969, 14348907};
    const int _N=155, _M=10, _K=1505;
    int N, M, K;  // 长度，高度和坏块数量
    // 状态压缩
    // 使用两种形式——三进制 和 四进制
    // 假设 A 点为 i 行上的一点，B 是 i-1 行上同一列的点
    // 三进制 0 表示 A, B 均空闲，1 表示仅 A 空闲，2 表示 A 被占用
    // 对应的四进制分别是 0~00 1~01 2~11
    // 这样可以保证“按位大小关系”不变
    int F[_N][59049];  // F[i][j] ~ 第 i 行三进制状态为 j，芯片数量最大值
    std::bitset<_M> bad[_N];  // bad[i][j] 表示第 i 行第 j 列是否为坏块，0 开始存
    int minStatus4[_N];  // 第 i 行四进制状态的按位最大值；必须保证状态按位小于等于这个值

    int base_3_to_4[59049];
    int base_4_to_3[1<<(_M*2)];

    int ans=0;

    void pre() {
        from(i, 0, pow3[_M]-1) {
            var copy = i;
            var base4 = 0;
            var digitVal = 1;
            while (copy) {
                var digit = copy % 3;
                if (digit == 2)  digit=3;
                base4 += digit * digitVal;
                copy /= 3;
                digitVal <<= 2;
            }
            base_3_to_4[i] = base4;
            base_4_to_3[base4] = i;
            // log("%d %d\n", i, base4)
        }
    }

    // 铺块
    // 从指定行的指定位置开始向右放置横排或者竖排的块
    // 会影响到 i 行和 i+1 行的状态
    void dfs(
        int pos,    // 指定位置（这里还没有被搜到过）
        int cur,    // 本行的当前状态（四进制）
        int nxt,    // 下一行的当前状态
        int cnt,    // 整个棋盘上放置的块数
        int tgt     // 本行行号
    ) {
        // 检验状态是否合法
        // if (bit::greater(minStatus4[tgt], cur))  return;
        // if (bit::greater(minStatus4[tgt+1], nxt))  return;

        

        // 递归终点
        if (pos >= M) {
            // 记录当前方案
            chkMax(F[tgt+1][base_4_to_3[nxt]], cnt);
            chkMax(ans, cnt);
            
            return;
        }

        val digit0 = pos<<1;  // 首个未被搜到的二进制位


        // 如果是坏块，则必定看成“被占用”
        if (bad[tgt][pos]) {
            cur |= 0b11 << digit0;
            nxt |= 0b01 << digit0;
        }
        if (bad[tgt+1][pos]) {
            cur |= 0b11 << digit0;
        }
        
        // 考虑剪枝
        // 无论什么放置方式，都需要 2*2 的空间
        // 如果这个空间被占用，则跳过
        if (0b1111<<digit0 & cur || 0b1111<<digit0 & nxt) {
            // 可以直接跳转到 pos+2
            dfs(pos+2, cur, nxt, cnt, tgt);
            return;
        }

        // 放一个横的
        // 要求：此后连续的 6 位全是 0
        if ( pos+3<=M && ~(0b111111<<digit0 & cur) ) {
            val new_cur = cur | (0b111111 << digit0);
            val new_nxt = nxt | (0b010101 << digit0);
            log("-- dfs %d %d %d %d %d\n", pos+3, new_cur, new_nxt, cnt+1, tgt)
            dfs(pos+3, new_cur, new_nxt, cnt+1, tgt);
        }

        // 放一个竖的
        // 要求：nxt, cur 此后连续的 4 位均全是 0
        if ( pos+2<=M && ~(0b1111<<digit0 & cur) && ~(0b1111<<digit0 & nxt) && tgt+1 < N ) {
            val new_cur = cur | (0b1111 << digit0);
            val new_nxt = nxt | (0b1111 << digit0);
            log("|  dfs %d %d %d %d %d\n", pos+3, new_cur, new_nxt, cnt+1, tgt)
            dfs(pos+2, new_cur, new_nxt, cnt+1, tgt);
        }

        // 什么都不放
        dfs(pos+1, cur, nxt, cnt, tgt);
    }

    void init() {
        // 清理前一次询问
        std::memset(F, 0, sizeof(F));
        from(i, 0, N-1)  bad[i].reset();
        std::memset(minStatus4, 0, sizeof(minStatus4));
        ans=0;

        io >> N >> M >> K;
        int x, y;
        from(i, 0, K-1) {
            io >> x >> y;
            bad[x-1][y-1] = true;
        }

        // 预处理最大状态
        // 最大状态：包括所有坏块
        from(i, 0, N) {
            var max_s = 0;
            from(j, 0, M-1) {
                if (bad[i][j]) {
                    max_s |= (0b11 << (j<<1));
                } else if (bad[i-1][j]) {
                    max_s |= (0b10 << (j<<1));
                }
            }
            minStatus4[i] = max_s;
        }

        // debug  batchOutput(minStatus4, N);
    }

    void solve() {
        init();
        from(i, 1, N-1) {  // 行号
            // 确保状态中不包含坏的
            from(j, base_4_to_3[ minStatus4[i] ], pow3[M]-1) {  // 状态
                if (bit::greater(minStatus4[i], base_3_to_4[j]))  continue;
                // * nxt初始值不对

                // 考虑 nxt 的初始值
                // 在四进制意义下，对于 cur 的每两位取值
                // 00 -> 00
                // 01 -> 00
                // 11 -> 01
                // 只有这三种可能性转移
                val init_cur = base_3_to_4[j];
                var init_nxt = minStatus4[i+1];
                from(k, 0, M-1) {
                    if ((init_cur >> (k<<1) & 0b11) == 0b11) {
                        init_nxt |= (0b01<<(k<<1));
                    }
                }
                dfs(0, init_cur, init_nxt, F[i][j], i);
            }
        }


        never {
            // 调试，输出 F
            from(i, 0, N) {
                from(j, 0, pow3[M]-1) {
                    printf("%d ", F[i][j]);
                }
                printf("\n");
            }
        }

        printf("%d\n", ans);
    }
}


int main() {
    never{freopen("N1103.in", "r", stdin);
    freopen("N1103.out", "w", stdout);}
    io.scan(D);
    initDebug;
    Solution::pre();
    while (D --> 0)
        Solution::solve();
    return 0;
}
