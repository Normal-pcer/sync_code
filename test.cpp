/**
 * 
 */

#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define rev(i,e,b) for(int i=(e);i>=(b);i--)
#define main() main(int argc, char const *argv[])
#define chkMax(base,cmp...) (base=std::max({(base),##cmp}))
#define chkMin(base,cmp...) (base=std::min({(base),##cmp}))
#define never if constexpr(0)
#define always if constexpr(1)
#define bitOr(x,y) (((x)&(y))^(((x)^(y))|(~(x)&(y))))
#define Infinity 2147483647
bool DEBUG_MODE=false;
typedef long long ll; typedef unsigned long long ull;

#define __macro_arg_counter(_1,_2,_3,_4,_5, N, ...) N
#define macro_arg_counter(...)  __macro_arg_counter(__VA_ARGS__,5,4,3,2,1,0)
#define __macro_choose_helper(M,count)  M##count
#define macro_choose_helper(M,count)   __macro_choose_helper(M,count)
#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define lambda(args...) macro_choose_helper(__lambda_, macro_arg_counter(args))(args)
#define lam lambda
namespace lib{}

#include <bits/stdc++.h>
#define USE_FREAD
// #undef USE_FREAD
// 取消注释上一行会使用 getchar() 替代 fread，可以不使用 EOF 结束读入，但是降低性能 
namespace lib{
#ifndef LIB_STRING
    using string=std::string;
#endif
#ifdef USE_FREAD
    template <const long long MAXSIZE, const long long MAX_ITEM_SZ=500>
#endif
    struct IO {
#ifdef USE_FREAD
        char buf[MAXSIZE],*p1,*p2;
        char pbuf[MAXSIZE],*pp;
        IO():p1(buf),p2(buf),pp(pbuf) {}
        ~IO() {  fwrite(pbuf,1,pp-pbuf,stdout);  }
        inline char gc() {
            if (p1==p2) p2=(p1=buf)+fread(buf,1,MAXSIZE,stdin);
            return p1==p2?'\0':*p1++;
        }
        inline void sync() { fwrite(pbuf,1,MAXSIZE,stdout); pp=pbuf; }
#endif
#ifndef USE_FREAD
        inline void sync() {}
        inline char gc() {  return getchar();  }
#endif
        char floatFormat[10]="%.6f", doubleFormat[10]="%.6lf";
        inline bool blank(char ch) { return ch<=32 or ch==127; }
        inline bool isd(char x) {return (x>='0' and x<='9');}
        inline IO& setprecision(int d) {
            sprintf(floatFormat, "%%.%df", d); sprintf(doubleFormat, "%%.%dlf", d);
            return *this;
        }
        string input(int reserve=0) {
            char c = gc(); string res=""; res.reserve(reserve);
            while((c&&!blank(c)) || c==' ') {  res.push_back(c); c = gc(); }
            return res;
        }
        template <class T>
        inline void read(T &x) {
            double tmp=1; bool sign=0; x=0; char ch=gc();
            for (; not isd(ch); ch=gc()) if (ch=='-') sign=1;
            for (; isd(ch); ch=gc()) x=x*10+(ch^48);
            if (ch=='.') for (ch=gc(); isd(ch); ch=gc()) tmp*=.1f,x+=tmp*(ch^48);
            if (sign) x=-x;
        }
        inline void read(char *s) {
            char ch=gc();
            for (; blank(ch); ch=gc());
            for (; not blank(ch); ch=gc()) *s++=ch;
            *s=0;
        }
        inline void readln(char *s) {
            char c = gc(); while((c&&!blank(c)) || c==' ') {  *(s++)=c; c = gc();  } *s=0;
        }
        inline void readln(string &res, int reserve=0) {
            char c = gc(); string().swap(res); res.reserve(reserve);
            while((c&&!blank(c)) || c==' ') {  res.push_back(c); c = gc(); }
        }
        inline void read(char &c) {  for (c=gc(); blank(c); c=gc());  }
        inline void read(string &s){
            string().swap(s); char ch=gc();
            for (; blank(ch); ch=gc());
            for (; not blank(ch); ch=gc()) s.push_back(ch);
        }
        template <class T,class... Types> inline void read(T &x,Types &...args){  read(x); read(args...);  }
        template <class T> inline void scan(const T &x) { read(*x); }
        template <class T,class ...Types> inline void scan(const T &x,const Types &...args) {  read(*x); scan(args...);  }
        inline void push(const char &c) {
#ifdef USE_FREAD
            if (pp-pbuf==MAXSIZE) sync();
            *pp++=c;
#endif
#ifndef USE_FREAD
            putchar(c);
#endif
        }
        inline void write(const double x) {
#ifdef USE_FREAD
            if (pp-pbuf>=MAXSIZE-MAX_ITEM_SZ) sync();
            pp += sprintf(pp, doubleFormat, x);
#endif
#ifndef USE_FREAD
            printf(doubleFormat, x);
#endif
        }
        inline void write(const float x) {
#ifdef USE_FREAD
            if (pp-pbuf>=MAXSIZE-MAX_ITEM_SZ) sync();
            pp += sprintf(pp, floatFormat, x);
#endif
#ifndef USE_FREAD
            printf(floatFormat, x);
#endif
        }
        inline void write(const char c) {  push(c);  }
        inline void write(const string &s){  for (auto i:s)  push(i);  }
        inline void write(const char *s){  for (; *s; ++s) push(*s);  }
        template <class T
#if __cplusplus > 201403L
        , class = typename std::enable_if_t<std::is_integral_v<T>>
#endif
        >
        inline void write(T x) {
            static char sta[40]; int top=0;
            if (x<0) {
                push('-'),sta[top++]=(-(x%10))^48,x=-(x/10);
                if (x==0) { push(sta[--top]); return; }
            }
            do {  sta[top++]=x%10^48,x/=10;  } while (x);
            while (top) push(sta[--top]);
        }
        template <class T,class... Types>  inline void write(const T &x,const Types &...args){ write(x); write(' '); write(args...); }
        template <class... Types> inline void writeln(const Types &...args){  write(args...); write('\n');  }
        template<class T=int> inline T get() {  T x; read(x); return x;  }
        // 流式输入输出
        template <class T> inline IO& operator>>(T&x) {  read(x); return *this; }
        template <class T> inline IO& operator<<(const T&x) {  write(x); return *this; }
    };
    IO
#ifdef USE_FREAD
    <1048576>
#endif
    io;
    const char endl = '\n';

}

using namespace lib;


namespace Solution {

    const int mod = 1e9+7;
    const int _N = 1005;
    int N;
    int super[_N][_N];
    
    void init() {
        io >> N;
        from(i, 1, N)  from(j, 1, N) {
            io >> super[i][j];
        }
    }

    int sub(int x, int l, int m, int r) {
        auto a = (m-l+1) * (r-m+1) % mod;
        return a * x % mod;
    }

    template <class arr>
    int count(arr&& src) {
        int tot = 0;
        static int cnt[_N][_N];  // 从该点向上有几个连续的 1
        from(digit, 0, 31) {
            const int mask = 1<<digit;  // 掩码
            int ans = 0;  // 记录这一位的答案
            from(i, 1, N)  from(j, 1, N) {
                if (src[i][j] & mask)  cnt[i][j] = cnt[i-1][j] + 1;
                else  cnt[i][j] = 0;
            }

            from(bottom, 1, N) {  // 枚举底边
                auto cur = cnt[bottom];  // 当前行
                static int st[_N];  // 存储下标，对应值单调不减
                static int st2[_N];  // 存储下标，对应值单调递增
                int p = 0, q = 0;
                int ans_line = 0;  // 本行产生的贡献

#if false  // 错误的实现思路
                from(i, 1, N+1) {  // 从左到右遍历，右边界视为 0
                    auto last = i;  // 最后一个被弹出的元素
                    bool flag = false;  // 是否有元素在本次流程中被弹出
                    while (p and cur[st[p]] > cur[i]) {
                        // 一个元素将要被弹出
                        // 它的上端可以直接被结算
                        auto height = cur[st[p]] - cur[i];  // 比当前元素高的部分
                        auto width = i - st[p];  // 宽度
                        // 上端的一个长方形区域，计算这部分的子矩阵数量
                        auto cnt1 = sub(height, width);
                        // 减去只在右边的部分
                        auto passed = i - last;  // 需要减去的宽度；高度依然是 height
                        auto cnt2 = sub(height, passed);
                        // 减去只在上边的部分
                        // 故这次结算出来的数量为 cnt1 - cnt2
                        assert(cnt1-cnt2>=0), ans_line = (ans_line - cnt2 + cnt1) % mod;
                        last = st[p--], flag = true;  // 记录并弹出当前元素
                    }

                    if (flag)  cur[st[++p]] = cur[i];  // 截断
                    else  st[++p] = i;
                }

                debug io << std::format("ans of line {}: {}", bottom, ans_line) << endl;
#endif
                // 对于每一列，寻找分别寻找它左侧第一个低于它的，和右侧第一个不高于它的列
                std::vector<int> left(N+5);  // 记录左侧
                from(i, 1, N+1) {
                    while (p and cur[st[p]] > cur[i]) {
                        // 栈顶元素的右侧添加了一个更高的
                        auto height = cur[st[p]];
                        auto l = left[st[p]] + 1;
                        auto r = i - 1;
                        auto ans_this = sub(height, l, st[p], r);
                        ans_line += ans_this;  // 记录必须包含这一列的方法总数
                        ans_line %= mod;
                        p--;
                    }
                    while (q and cur[st2[q]] >= cur[i]) {
                        q--;
                    }

                    // 此时 st2[p] 栈顶的对应值严格小于 i
                    // 符合条件，直接记录
                    left[i] = st2[q];

                    st[++p] = st2[++q] = i;  // 将 i 压入两个栈
                }

                ans = (ans + ans_line) % mod;
                debug io << std::format("({})ans of line {}: {}", digit, bottom, ans_line) << endl;
            }
            tot += (ll)ans * mask % mod;
            tot %= mod;
        }
        return tot;
    }

    void solve() {
        init();

        io << count(super) << endl;
    }
}


int main() {
    initDebug;
    Solution::solve();
    return 0;
}