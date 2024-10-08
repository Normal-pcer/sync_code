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
namespace lib {
    template <typename T>
    class vector: public std::vector<T> {
    public:
        vector() = default;
        vector(const std::initializer_list<T>& list): std::vector<T>(list) {}
        vector(const vector<T>& other): std::vector<T>(other) {}
        vector(const size_t size): std::vector<T>(size) {}

        void filtered(const std::function <bool(const T&)>& f) {
            auto ptr1 = this->begin();
            auto ptr2 = this->begin();

            for (; ptr1!=this->end(); ptr1++) {
                if (f(*ptr1))  *(ptr2++) = *ptr1;
            }

            this->resize(ptr2 - this->begin());
        }

        vector<T> filter(const std::function <bool(const T&)>& f) {
            vector<T> res;
            for (auto& i: *this) {
                if (f(i)) {
                    res.push_back(i);
                }
            }
            return res;
        }

        template <typename Func>  
        auto map(Func&& f) const {  
            vector<decltype(std::invoke(std::forward<Func>(f), std::declval<const T&>()))> result;
            for (auto &i: *this) {
                result.push_back(std::invoke(std::forward<Func>(f), i));
            }
            return result;  
        }

        template <typename Func>
        auto reduce(Func&& f) const { 
            auto result = this->front();
            for (size_t i=1; i<this->size(); i++) {
                result = f(result, this->at(i));
            }
            return result;
        }
        
        bool all(const std::function <bool(const T&)>& f) {
            for (auto& i: *this) {
                if (!f(i)) {
                    return false;
                }
            }
        }

        bool some(const std::function <bool(const T&)>& f) {
            for (auto& i: *this) {
                if (f(i)) {
                    return true;
                }
            }
            return false;
        }

        inline void push(T&& t) {
            this->push_back(t);
        }
    };
}

#define lambda(expr, args...) [&](args) {return expr;}
#define select_func(ret, name, args...) static_cast<ret(*)args>(name)

#include <bits/stdc++.h>
namespace lib {
    class string: public std::string {
    public:
        string() = default;
        string(const char* s) : std::string(s) {}
        string(std::string s) : std::string(s) {}
        string join(std::vector<std::string> s) {
            string res = "";
            size_t size = 0;
            for (auto& i: s)  size += i.size();
            res.reserve(size);
            for (auto& i: s) {
                res += i;
                res += *this;
            }
            return res;
        }
        vector<string> split(std::vector<char> c={' ', '\n', '\t'}) {
            vector<string> res;
            size_t start = 0;
            for (size_t i = 0; i < this->size(); i++) {
                if (std::find(c.begin(), c.end(), (*this)[i]) != c.end()) {
                    if (i - start > 0)
                        res.push_back(this->substr(start, i - start));
                    start = i + 1;
                }
            }
            if (start < this->size())
                res.push_back(this->substr(start, this->size()));
            return res;
        }
        vector<string> split(char c) {
            return split(vector<char>{c});
        }
    };
}
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
#include <bits/stdc++.h>
namespace lib {
    template <typename T, const int p>
    class mod {
    public:
        T data;

        mod() = default;
        mod(T num) : data(positive(num % p)) {}

        T positive(T num) { 
            if (num>=0)  return num;
            if (num+p>=0)  return num+p;
            return num + p * ((-num) / p + 1);
        }

        mod operator+(const mod &other) { return mod(positive(data + other.data) % p); }
        mod operator-(const mod &other) { return mod(positive(data - other.data) % p); }
        mod operator*(const mod &other) { return mod(positive(data * other.data) % p); }
        mod operator/(const mod &other) { return mod((data * other.data)); }
    };
}
#include <bits/stdc++.h>
namespace lib {
    template <typename T, const long long sz>
    class RollingArray {  // 滚动数组
    public: 
        T arr[sz];
        long long first = 0;  // arr[0] 对应的原下标
        

        void forward() {
            for (long long i=0; i<sz-1; i++) {
                std::memcpy(arr+i, arr+i+1, sizeof(T));
            }
            first++;
        }

        void backward() {
            for (long long i=sz-1; i>0; i--) {
                std::memcpy(arr+i, arr+i-1, sizeof(T));
            }
            first--;
        }

        T& operator[](long long i) {
            long long real = i - first;
            while (real >= sz) {
                real--;
                this->forward();
            }
            while (real < 0LL) {
                real++;
                this->backward();
            }
            return arr[real];
        }
    };
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


int D = io.read();

namespace Solution {
    const int pow3[] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561, 
                    19683, 59049, 177147, 531441, 1594323, 4782969, 14348907};
    const int _N=155, _M=10, _K=1505;
    int N, M, K;  // 长度，高度和坏块数量
    // 状态压缩
    // 连续 表示 i, i-1, i-2
    // 三进制，2 表示连续三行本位没被占用，1 表示两行，0 表示一行或零行。
    RollingArray2<int[59049], 4> F;  // F[i][j] ~ 第 i 行状态为 j，芯片数量最大值
    std::bitset<_M> bad[_N];  // bad[i][j] 表示第 i 行第 j 列是否为坏块，0 开始存

    // 对于一个三进制状态 src，从第 pos 位开始向右搜索所有子集
    // 对于能够保证三进制下每一位不大于 lmt 的子集，调用回调函数 f
    void subset(
        int pos,  // 当前的数位
        int pst,  // 先前的数字和（不包含pos位）
        int src,  // 要搜索的超集（三进制）
        int lmt,  // 限制条件；子集每一位必须大于等于 lmt
        const std::function<void(int)>& f  // 回调函数，传入子集
    ) {
        // 先前搜索到的一定是一个正解
        f(pst);
        if (pos >= M) {
            // 结束递归
            return;
        }

        // 枚举向后的每一位
        from(i, pos, M-1) {
            int digit = src / pow3[i] % 3;
            // 如果这位符合要求
            if (digit >= lmt) {
                subset(i+1, pst + digit * pow3[i], src, lmt, f);
            }
        }
    }

    void init() {
        std::memset(F.arr, 0, sizeof(F.arr));
        from(i, 0, N-1)  bad[i].reset();
        io >> N >> M >> K;
        int x, y;
        from(i, 0, K-1) {
            io >> x >> y;
            bad[x-1][y-1] = true;
        }
    }

    void solve() {
        init();
        
        // DP 
        from(i, 2, N-1) {  // 行号
            // 对于这个行中的每一个状态
            // 从左到右枚举列号作为左下角
            // 如果一行有连续的三个大于等于 1，可以放置一个横排
            // 如果一行有连续的两个等于 2，可以放置一个竖排

            // 需要先处理这个行的初始状态
            int prev = 0;
            from(j, 0, M-1) {  // 第 j 列
                int digit=0;
                if (!bad[i-1][j]) {
                    digit=1;
                    if (!bad[i-2][j]) {
                        digit=2;
                    }
                }
                prev += pow3[j] * digit;
            }

            // 这一行的初始状态处理完毕
            // 对于这一行所有的可能状态，都是初始状态的若干位设 0
            // 枚举所有可能的未来状态

            // 尝试加一个横块
            #define digitAt(x, p) (x / pow3[p] % 3)
            subset(0, 0, prev, 1, [&i](int st){
                for (int k=0; k+2<M; k++) {  // 左下角
                    // 判断连续三位是否均可行
                    if (digitAt(st, k) >= 1 && digitAt(st, k+1) >= 1 && digitAt(st, k+2) >= 1) {
                        // 说明可以在这里放一个横块
                        int after = st + pow3[k] + pow3[k+1] + pow3[k+2];
                        // 可以转移
                        chkMax(F[i][after], F[i-1][st] + 1);
                    }
                }
            });

            // 尝试加一个竖块
            subset(0, 0, prev, 2, [&i](int st){
                for (int k=0; k+1<M; k++) {  // 右下角
                    // 判断连续两位是否均可行
                    if (digitAt(st, k) >= 2 && digitAt(st, k+1) >= 2) {
                        // 说明可以在这里放一个竖块
                        int after = st + pow3[k] + pow3[k+1];
                        // 可以转移
                        chkMax(F[i][after], F[i-1][st] + 1);
                    }
                }
            });


            #undef digitAt
        }
    }
}


int main() {
    initDebug;
    while (D --> 0)
        Solution::solve();
    return 0;
}