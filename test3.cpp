/**
 * @link
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

#ifdef int
#define __defined_int_to_long_long
#undef int
#endif
#ifdef __linux__
#include <sys/stat.h>
#include <sys/mman.h>
#endif
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
#ifdef __linux__
        struct stat s;
        char *c;
#else
        char buf[MAXSIZE],*p1,*p2;
#endif// __linux__
        char pbuf[MAXSIZE],*pp;
#ifdef __linux__
        IO(): pp(pbuf) {
            fstat(0, &s);
            c = (char*)mmap(nullptr, s.st_size, 1, 2, 0, 0);
        }
#else // not __linux__
		IO(): p1(buf), p2(buf), pp(pbuf) {}
#endif // __linux
        ~IO() {  fwrite(pbuf,1,pp-pbuf,stdout);  }
#ifdef __linux__
		inline char gc() { return *c++; }
#else  // not __linux__
        inline char gc() {
            if (p1==p2) p2=(p1=buf)+fread(buf,1,MAXSIZE,stdin);
            return p1==p2?'\0':*p1++;
        }
#endif // __linux__
        inline void sync() { fwrite(pbuf,1,MAXSIZE,stdout); pp=pbuf; }
#else // not USE_FREAD
        inline void sync() {}
        inline char gc() {  return getchar();  }
#endif// USE_FREAD
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
#else  // not USE_FREAD
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
        template <class T>
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
}

#ifdef __defined_int_to_long_long
#undef __defined_int_to_long_long
#define int long long
#endif
using namespace lib;

namespace Solution_4157617573719578 {
    auto solve() -> void {

    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 t; io >> t;
    while (t --> 0) Solution_4157617573719578::solve();
    return 0;
}
