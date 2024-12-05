/**
 * 
 */

#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=(n);i++)
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define rev(i,e,b) for(int i=(e);i>=(b);i--)

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
namespace lib{}

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

#define lambda(expr, args...) [&](args) {return expr;}
#define select_func(ret, name, args...) static_cast<ret(*)(args)>(name)

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
#define REQUIRE_EOF
// #undef REQUIRE_EOF
namespace lib{
#ifdef REQUIRE_EOF
    template <const size_t MAXSIZE>
#endif
    struct IO {
#ifdef REQUIRE_EOF
        char buf[MAXSIZE],*p1,*p2;
        char pbuf[MAXSIZE],*pp;
        inline char gc() {
            if (p1==p2) p2=(p1=buf)+fread(buf,1,MAXSIZE,stdin);
            return p1==p2?' ':*p1++;
        }
#endif
#ifndef REQUIRE_EOF
        inline char gc() {
            return getchar();
        }
#endif
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
        inline void write(char *s){
            for (; *s; ++s) push(*s);
        }
        inline void write(const char *s){
            for (; *s; ++s) push(*s);
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
        template <class T,class... Types>
        inline void write(T &x,Types &...args){
            write(x);
            write(' ');
            write(args...);
        }
        template <class... Types>
        inline void writeln(Types &...args){
            write(args...);
            write('\n');
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
    IO
#ifdef REQUIRE_EOF
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

    int N;
    const int _N = 1e5+5;

    struct Node {
        int to, val=0;
    };

    vector<Node> conn[_N];
    int F[_N];
    
    void init() {
        // std::memset(F, 0x3f, sizeof(F));

        io >> N;
        int x, y;
        from(_, 1, N-1) {
            io >> x >> y;
            conn[x].push_back({y});
            conn[y].push_back({x});
        }
    }

    void dfs(int p, int prev, std::vector<Node>::iterator it_prev) {
        int cnt = 1;
        for (auto it=conn[p].begin(); it!=conn[p].end(); it++) {
            if (it->to == prev)  continue;
            dfs(it->to, p, it);
            cnt += it->val;
        }

        if (prev) {
            it_prev->val = cnt;
        }
        log("dfs %d->%d => %d\n", p, prev, cnt);
    }

    void dfs2(int p, int prev) {
        for (auto &dest: conn[p]) {
            if (dest.to == prev)  continue;
            for (auto &dzd: conn[dest.to]) {
                if (dzd.to == p) {
                    chkMax(dzd.val, dest.val);
                    log("%d->%d %d\n", dest.to, dzd.to, dzd.val)
                    break;
                }
            }
        }
    }

    void solve() {
        init();
        from(i, 1, 1)
            dfs(i, 0, conn[i].end());
        from(i, 1, N) {
            for (auto &dest: conn[i]) {
                log("%d->%d %d\n", i, dest.to, dest.val)
                chkMax(F[i], dest.val);
                // chkMax(F[dest.to], dest.val);
            }
        }
        bool none = true;
        from(i, 1, N) {
            if (F[i] <= N / 2) {
                io.writeln(i);
                none = false;
            }
        }
        if (none)  io.writeln("NONE");
    }
}

int main(int argc, char const *argv[]) {

    initDebug;
    Solution::solve();
    return 0;
}