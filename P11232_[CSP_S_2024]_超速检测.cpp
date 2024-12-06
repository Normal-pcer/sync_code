#include <bits/stdc++.h>
#define DEBUG_MODE false
#define debug if(DEBUG_MODE)
#define log debug printf
#define from(i, l, r) for(int i=l; i<=r; i++)
typedef long long ll;

#define USE_FREAD
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
using namespace lib;

int T;

namespace Solution {
    template <const int SZ>
    struct SegTree {
        struct Node {
            int l, r;
            int val;
        } tr[SZ<<2];
        #define ls (p<<1)
        #define rs (p<<1|1)

        void pushup(int p) {
            tr[p].val = tr[ls].val + tr[rs].val;
        }

        void build(int p, int l, int r) {
            tr[p].l = l, tr[p].r = r;
            tr[p].val = 0;
            if (l==r) {
                return;
            }
            int mid = (l+r)>>1;
            build(ls, l, mid);
            build(rs, mid+1, r);
        }

        void add(int p, int pos, int val) {
            if (tr[p].l == pos and tr[p].r == pos) {
                tr[p].val += val;
                return;
            }
            if (tr[ls].r >= pos)  add(ls, pos, val);
            if (tr[rs].l <= pos)  add(rs, pos, val);
            pushup(p);
        }

        int query(int p, int l, int r) {
            if (tr[p].l >= l and tr[p].r <= r) {
                return tr[p].val;
            }
            int res = 0;
            if (tr[ls].r >= l)  res += query(ls, l, r);
            if (tr[rs].l <= r)  res += query(rs, l, r);
            return res;
        }

        #undef ls
        #undef rs
    };

    struct seg {
        int l, r;
        bool selected = false;
    };

    const int _N = 1e5+5, _L = 1e6+5;
    // N: cars' count
    // M: observers' count
    // L: length of road
    // V: speed limit
    int N, M, L, V;
    std::vector<seg> carSegs;  // Segments that each car in it is illegal.
    std::vector<int> observers;  // Initial position of observers.
    SegTree<_L> detected;  // How many observers are there in the segment queried.

    int ceil_div(int a, int b) {
        const int c = a/b, d = a%b;
        if (d!=0)  return c+1;
        else  return c;
    }

    template <class ptr_t, class T>
    ptr_t most_right_in(ptr_t begin, ptr_t end, T val) {
        auto tmp = std::upper_bound(begin, end, val);
        if (tmp == begin)  return end;
        return tmp - 1;
    }

    // Calculate illegal segment of a car with argument (d, v0, a).
    // Returns a segment of [-1, -1] if it is always running legally.
    // I think this function is right. 
    seg IllegalSegOf(int d, int v0, int a) {
        if (a==0) {
            if (v0 <= V)  return {-1, -1};
            else  return {d, L};
        } else if (a>0) {
            if (v0 > V)  return {d, L};
            else  return {std::min(L+1, ((V*V-v0*v0)/(2*a))+1+d), L};
        } else {
            if (v0 > V)  return {d, std::min(L+1, ceil_div(v0*v0-V*V, 2*abs(a))-1+d)};
            else  return {-1, -1};
        }
    }

    void init() {  // DON'T forget to clear all arrays
        debug io << "T = " << T << endl;
        carSegs.clear();
        observers.clear();

        io >> N >> M >> L >> V;
        detected.build(1, 1, _L);
        // d: initial position; v0: initial speed;
        // Notice that d means 'distance to the outset', which is able to be zero.
        int d, v0, a;
        // carSegs.reserve(N);
        from(i, 1, N) {
            io >> d >> v0 >> a;
            auto seg = IllegalSegOf(d, v0, a);
            // if (seg.l != -1)
            carSegs.push_back(seg);
            log("%d %d\n", seg.l, seg.r);
        }

        std::sort(carSegs.begin(), carSegs.end(), [&](seg a, seg b) {
            return a.r<b.r;
            // return a.r==b.r? a.l>b.l: a.r<b.r;
        });

        observers.resize(M);
        from(i, 0, M-1)  io >> observers[i];
    }

    

    void solve() {
        init();
        
        // Find all cars can be found by initial observers
        int cnt = 0;
        for (auto &obs: observers)  detected.add(1, obs, 1);
        for (auto &seg: carSegs) {
            if (seg.l!=-1 and detected.query(1, seg.l, seg.r)) {
                debug io.writeln(seg.l, seg.r, detected.query(1, seg.l, seg.r), endl);
                seg.selected = true;
                cnt++;
            }
        }

        int require = 0;
        detected.build(1, 1, _L);
        for (auto &seg: carSegs) {
            if (not seg.selected)  continue;
            if (detected.query(1, seg.l, seg.r))  continue;

            require++;
            auto obsIter = most_right_in(observers.begin(), observers.end(), seg.r);
            debug io << obsIter - observers.begin() << endl;
            detected.add(1, *obsIter, 1);
        }

        io.writeln(cnt, M-require);
    }
}

int main() {
#if false
    freopen("detect.in", "r", stdin);
    freopen("detect.out", "w", stdout);
#endif
    io >> T;
    while (T --> 0)
        Solution::solve();
    return 0;
}