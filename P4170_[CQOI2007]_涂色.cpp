/**
 * @link https://www.luogu.com.cn/problem/P4170
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
#define ensure(v, con, otw) (((v) con)? (v): (otw))
#define never if constexpr(0)
#define always if constexpr(1)
#define bitOr(x,y) (((x)&(y))^(((x)^(y))|(~(x)&(y))))
#define Infinity 2147483647
#define compare(x,y,g,e,l) (((x)>(y))?(g):(((x)<(y))?(l):(e)))
bool DEBUG_MODE=false;
typedef long long ll; typedef unsigned long long ull;
inline void batchOutput(int *begin, int n, const char *format){upto(i, n)printf(format, begin[i]);printf("\n");} inline void batchOutput(int*begin, int n) {batchOutput(begin,n,"%3d ");}
#define batchOutput2d(b, r, c, fmt) upto(i,r){upto(j,c)printf(fmt,b[i][j]);printf("\n");}
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

#define LIB_VECTOR
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

#if __cplusplus >= 201702L

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

#endif

        template <typename Func>  
        void foreach(Func&& f) const {  
            for (auto &i: *this) {
                f(i);
            }
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
            return true;
        }

        bool all() { return all([](auto&x){return x;}); }

        bool some(const std::function <bool(const T&)>& f) {
            for (auto& i: *this) {
                if (f(i)) {
                    return true;
                }
            }
            return false;
        }

        bool some() { return some([](auto&x){return x;}); }

        inline void push(T&& t) {
            this->push_back(t);
        }
    };
}

#define select_func(ret, name, args...) static_cast<ret(*)(args)>(name)

#include <bits/stdc++.h>
#define LIB_STRING
namespace lib {
    class string: public std::string {
    public:
        string() = default;
        string(const char* s) : std::string(s) {}
        string(std::string s) : std::string(s) {}
        template <class T>
        string join(T&& s) {
            string res = "";
            size_t size = 0;
            for (auto& i: s)  size += i.size();
            res.reserve(size);
            bool first = true;
            for (auto& i: s) {
                if (!first)
                    res += *this;
                res += i;
                first = false;
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
        string repeat(int times) const {
            string result = *this;
            result.reserve(times * this->size());
            for (int i=1; i<times; i++)  result += *this;
            return result;
        }
        string operator* (int times) const {
            return this->repeat(times);
        }
    };
}
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
            return p1==p2?' ':*p1++;
        }
        inline void sync() { fwrite(pbuf,1,MAXSIZE,stdout); pp=pbuf; }
#endif
#ifndef USE_FREAD
        inline void sync() {}
        inline char gc() {  return getchar();  }
#endif
        char floatFormat[10]="%.6f", doubleFormat[10]="%.6lf";
        inline bool blank(char ch) { return ch==' ' or ch=='\n' or ch=='\r' or ch=='\t'; }
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
            char c = gc(); while((c&&!blank(c)) || c==' ') {  *(s++)=c; c = gc();  }
        }
        inline void readln(string &res, int reserve=0) {
            char c = gc(); res.reserve(reserve);
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
#endif
#ifndef USE_FREAD
            printf(floatFormat, x);
#endif
        }
        inline void write(const char c) {  push(c);  }
        inline void write(const string &s){  for (auto i:s)  push(i);  }
        inline void write(const char *s){  for (; *s; ++s) push(*s);  }
        template <class T>
        inline void write(T x) {
            if (x<0) x=-x,push('-');
            static char sta[40]; int top=0;
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
    const char endl[] = "\n";

}

#include "./libs/bit.hpp"
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
#include "./libs/mapArray.hpp"
#include "./libs/binary.hpp"
using namespace lib;

;
namespace Solution {

    string s;
    int N; const int _N = 55;
    int F[_N][_N];
    
    void init() {
        io.readln(s);
        s = " "+s;
        N = s.size() - 1;
        std::memset(F, 0x3f, sizeof(F));
    }

    void solve() {
        init();

        from(i, 1, N) {
            F[i][i] = 1;
            // F[i+]
        }

        from(len, 2, N) {
            from(i, 1, N) {
                auto j = i + len - 1;
                if (j > N)  break;
                if (s[i] == s[j]) {
                    // if (s[i]==s[i+1])  chkMin(F[i][j], F[i+1][j]);
                    // if (s[j]==s[j-1])  chkMin(F[i][j], F[i][j-1]);
                    // chkMin(F[i][j], F[i+1][j-1]+1);
                    chkMin(F[i][j], F[i][j-1]);
                }
                from(k, i, j) {
                    chkMin(F[i][j], F[i][k] + F[k+1][j]);
                }
                log("F[%d][%d] = %d\n", i, j, F[i][j]);
            }
        }

        auto ans = F[1][N];
        io << ans << endl;
    }
}


int main() {;
    initDebug;

    Solution::solve();
    return 0;
}