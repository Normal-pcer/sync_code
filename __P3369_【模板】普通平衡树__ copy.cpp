/**
 * @link https://www.luogu.com.cn/problem/P3369
 */

#include "./lib"

#include <bits/stdc++.h>
#define USE_FREAD
#undef USE_FREAD
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
#include "./libs/binary.hpp"

using namespace lib;

namespace Solution_1062104036224782 {

    int Q;  const int _N = 1e5+5;
    
    int min_element = inf;
    
    template <int _N>
    struct set {
    protected:
        std::array<int, _N> tr;
    
#define lowbit(x) ((x) & -(x))
    public:
        void insert(int x) {
            while (x < _N) {
                tr[x]++;
                x += lowbit(x);
            }
        }

        void erase(int x) {
            while (x < _N) {
                tr[x]--;
                x += lowbit(x);
            }
        }

        int rank(int x) {
            auto x_cpy = x;
            auto res = 1;
            x--;
            while (x) {
                res += tr[x];
                x -= lowbit(x);
            }
            debug io << std::format("Rank of {} is {}", x_cpy, res) << endl;
            return res;
        }

        int at(int x) {
#if false
            x--;
            if (x == 0)  return min_element;
            auto idx = 0;
            int begin = std::__lg(_N);

            for (auto i = begin; i >= 0; i--) {
                if (idx + (1<<i) < _N and tr[idx + (1<<i)] < x) {
                    idx += 1<<i;
                    x -= tr[idx];
                }
            }
#else
            auto idx = binary::upper_bound_mapping(1, _N, x, lam(x, rank(x)));
#endif
            return idx - 1;
        }

        int prev(int x) {
            auto rank = this->rank(x);
            auto res = this->at(rank);
            if (res == x)  return this->at(rank-1);
            else  return res;
        }

        int next(int x) {
            auto rank = this->rank(x);
            auto res = this->at(rank+1);
            return res;
        }

#undef lowbit
    };

    set<_N> s;

    void solve() {
        io >> Q;
        std::vector<std::pair<int, int>> ops;
        std::vector<int> numbers = {-inf};
        while (Q --> 0) {
            int op, x;
            io >> op >> x;
            ops.push_back({op, x});
            numbers.push_back(x);
        }

        std::sort(numbers.begin(), numbers.end());
        numbers.erase(std::unique(numbers.begin(),numbers.end()),numbers.end());
        debug {
            for (auto i: numbers)  io << i << ' ';
            io << endl;
        }

        int size=0;
        for (auto [op, num]: ops) {
            int x = std::lower_bound(numbers.begin(), numbers.end(), num) - numbers.begin();
            debug io << std::format("{} <- {}", x, num) << endl;
            if (op == 1) {
                s.insert(x);
                chkMin(min_element, x);
                size++;
            } else if (op == 2) {
                s.erase(x);
                size--;
            } else if (op == 3) {
                io << s.rank(x) << endl;
            } else if (op == 4) {
                io << numbers[s.at(num)] << endl;
            } else if (op == 5) {
                io << numbers[s.prev(x)] << endl;
            } else {
                io << numbers[s.next(x)] << endl;
            }
            never {    
                io<<"a:";
                for (int i=1; i<=size; i++) io << numbers[s.at(i)] << " ";
                io << "\n";
            }
        }
    }
}
int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1062104036224782::solve();
    return 0;
}