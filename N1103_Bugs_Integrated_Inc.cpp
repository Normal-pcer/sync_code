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
    template <typename T, const long long sz>
    class RollingArray2 {  // 滚动数组 - 另一种实现思路
    public: 
        T arr[sz];

        T &operator[](long long idx) {
            return arr[idx % sz];
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

    const int _N = 153, _M = 11;
    int N, M, K;

    const int pow3[] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049, 177147};

    #define COLUMN_WIDTH_LIMIT (M)

    /*
    状态压缩。压了个寂寞。
    用三进制数表示两行——i, i-1 中对应列的占用状态。
    0 : 均未占用
    1 : 仅 i-1 行占用
    2 : i 行被占用。此时无论 i-1 行是否被占用。

    用若干个三进制位构成一个“压缩状态”整数。其中，低位数代表靠左的块。
    */

    RollingArray2<int[59049], 2> F;  // 遍历到前 i 行，且第 i 行的“压缩状态”为 j，此时的最大数量
    int prev[_M];  // 前一行的状态数组
    int cur[_M];  // 当前行的状态数组
    bool broken[_N][_M];  // 是否为坏块

    /**
     * 将一个整数转为三进制数组。数组靠前的项表示低位。
     * 总会对数组 COLUMN_WIDTH_LIMIT 之前的项进行处理。多余的位会置 0。
     * @param n - 将被转换的整数
     * @param result - 存储结果的数组
     */
    void integerToTernaryDigits(int n, int *result) {
        from(i, 0, COLUMN_WIDTH_LIMIT-1) {
            result[i] = n % 3;
            n /= 3;
        }
    }

    /**
     * 将一个三进制数组转为整数。规则参考 void integerToTernaryDigits(int, int*)
     * @param digits - 将被转换的数组
     * @return - 转换后的整数
     */
    int ternaryDigitsToInteger(int *digits) {
        int res = 0;
        from(i, 0, COLUMN_WIDTH_LIMIT-1) {
            res += digits[i] * pow3[i];
        }
        return res;
    }

    /**
     * 搜索，在给定状态下放置若干个芯片。
     * 将会自动记录结果。
     * 
     * @param ptr - 指针位置。这个位置并未被遍历过
     * @param row_index - 当前行的下标。
     * @param prs_count - 当前*整个地图上*已经放置的芯片数量
     *
     */
    void dfs_setBlocksInStatus(
        int ptr,
        int row_index,
        int prs_count
    ) {
        // 更新状态
        // 边界条件
        if (ptr >= COLUMN_WIDTH_LIMIT) {
            chkMax(F[row_index][ternaryDigitsToInteger(cur)], prs_count);
            return;
        }

        // 剪枝
        // 如果 2*2 范围非空——直接跳过
        if (
            ptr+1 < COLUMN_WIDTH_LIMIT &&
            (cur[ptr] || cur[ptr+1])
        ) {
            dfs_setBlocksInStatus(ptr+1, row_index, prs_count);
            return;
        }

        // 竖向放置
        if (
            ptr + 1 < COLUMN_WIDTH_LIMIT    &&  // 横向不溢出
            !prev[ptr]    &&  // 先前行不占用 
            !prev[ptr+1]  &&
            !cur[ptr]     &&  // 当前行不占用
            !cur[ptr+1]
        ) {
            // 修改状态——这些格子被完全占用
            cur[ptr] = cur[ptr+1] = 2;

            // 进一步搜索
            dfs_setBlocksInStatus(ptr+2, row_index, prs_count+1);

            // 解除占用
            cur[ptr] = cur[ptr+1] = 0;
        }

        // 横向放置
        if (
            ptr + 2 < COLUMN_WIDTH_LIMIT            &&  // 横向不溢出
            !cur[ptr] && !cur[ptr+1] && !cur[ptr+2]     // 不占用
        ) {
            cur[ptr] = cur[ptr+1] = cur[ptr+2] = 2;
            dfs_setBlocksInStatus(ptr+3, row_index, prs_count+1);
            cur[ptr] = cur[ptr+1] = cur[ptr+2] = 0;
        }

        dfs_setBlocksInStatus(ptr+1, row_index, prs_count);
    }

    void init() {
        io >> N >> M >> K;
        std::memset(broken, false, sizeof(broken));

        std::memset(F[1], -1, sizeof(F[1]));

        int x, y;
        from(i, 0, K-1) {
            io >> x >> y;
            broken[x][y-1] = true;
        }
    }

    void solve() {
        init();
        
        // 第一行的状态
        from(i, 0, M-1)  prev[i] = broken[1][i]? 2: 1;

        F[1][ternaryDigitsToInteger(prev)] = 0;
        from(i, 2, N) {  // 行号
            std::memset(F[i], -1, sizeof(F[i]));
            from(s, 0, pow3[M]-1) {  // 上一行的状态
                // 上一行对应项不可达，跳过
                if (F[i-1][s] == -1)  continue;

                integerToTernaryDigits(s, prev);  // 上一行
                from(k, 0, M-1) {  // 当前行的初始状态
                    if (broken[i][k])  cur[k] = 2;
                    else  cur[k] = std::max(prev[k]-1, 0);
                }

                dfs_setBlocksInStatus(0, i, F[i-1][s]);  // 在此基础上添加芯片
            }
        }
        int ans = 0;
        from(i, 0, pow3[M]-1)  chkMax(ans, F[N][i]);
        io << ans << endl;
    }
}

int main() {
    initDebug;
    freopen("N1103.in", "r", stdin);
    freopen("N1103.out", "w", stdout);
    io.scan(D);
    while (D --> 0)
        Solution::solve();
    return 0;
}