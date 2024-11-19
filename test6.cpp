/**
 * 
 */

#include "./lib_legecy.hpp"
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

using namespace lib;


namespace Solution_1545947219600800 {

    const int _N = 1e5+5;
    int N;
    ll a[_N];
    ll b[_N];

    template <size_t SZ>
    struct SegTree {
        struct Node {
            int l, r, len;
            ll sum, tag = -infLL;
        } tr[SZ >> 2];

        #define ls (p<<1)
        #define rs (p<<1 | 1)

        void push_up(int p) {
            tr[p].sum = tr[ls].sum + tr[rs].sum;
        }

        void push_down(int p) {
            if (tr[p].tag != -infLL) {
                tr[ls].sum = tr[p].tag * tr[ls].len;
                tr[rs].sum = tr[p].tag * tr[rs].len;
                tr[ls].tag = tr[p].tag;
                tr[rs].tag = tr[p].tag;
                tr[p].tag = 0;
            }
        }

        void build(int p, int l, int r) {
            tr[p].l = l, tr[p].r = r, tr[p].len = r - l + 1;
            if (l == r) {
                tr[p].sum = a[l];
                return;
            }

            auto mid = (l + r) >> 1;
            build(ls, l, mid);
            build(rs, mid+1, r);
            push_up(p);
        }

        void update(int p, int l, int r, ll val) {
            never io << std::format("update {} {} {} {}", p, l, r, val) << endl;
            if (tr[p].l >= l and tr[p].r <= r) {
                tr[p].sum = val * tr[p].len;
                tr[p].tag = val;
                return;
            }
            push_down(p);
            if (tr[ls].r >= l)  update(ls, l, r, val);
            if (tr[rs].l <= r)  update(rs, l, r, val);
            push_up(p);
        }

        ll query(int p, int l, int r) {
            if (tr[p].l >= l and tr[p].r <= r) {
                return tr[p].sum;
            }
            push_down(p);
            ll res = 0;
            if (tr[ls].r >= l)  res += query(ls, l, r);
            if (tr[rs].l <= r)  res += query(rs, l, r);
            return res;
        }

        #undef ls
        #undef rs
    };

    SegTree<_N> seg;

    void solve() {
        io >> N;
        from(i, 1, N)  io >> a[i];
        from(i, 1, N)  io >> b[i];

        seg.build(1, 1, N);

        int Q = io.get();
        from(_, 1, Q) {
            int op, x, y;
            io >> op >> x >> y;
            if (op == 1) {
                seg.update(1, x, x, y);
                a[x] = y;
            } else if (op == 2) {
                b[x] = y;
            } else {
                // 寻找区间内所有乘法操作
                std::vector<int> times;
            }
        }
    }
}


int main() {
    initDebug;
    Solution_1545947219600800::solve();
    return 0;
}