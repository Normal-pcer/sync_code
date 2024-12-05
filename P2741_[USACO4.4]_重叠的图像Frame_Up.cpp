/**
 * https://www.luogu.com.cn/problem/P2741
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

using namespace lib;

namespace Solution_6383929149905497 {
    
    struct Realm {
        int top = inf;
        int left = inf;
        int bottom = -1;
        int right = -1;

        operator std::string () {
            std::stringstream s;
            s << "Realm(" << top << ", " << left << ", " << bottom << ", " << right << ')';
            return s.str();
        }

        int getWidth() { return right - left; }
        int getHeight() { return bottom - top; }
    };
    
    void solve() {
        const static auto MAX_POINT = 128;
        int H, W;  io >> H >> W;
        std::vector<std::string> origin(H);
        std::map<char, Realm> realmOf;
        
        from(i, 0, H) {
            io >> origin.at(i);
        }

        std::vector<char> isExist(MAX_POINT);
        from(row, 0, H) {
            from(col, 0, W) {
                auto cur = origin[row][col];
                if (cur == '.')  continue;
                isExist[cur] = true;
                chkMin(realmOf[cur].top, row);
                chkMax(realmOf[cur].bottom, row+1);
                chkMin(realmOf[cur].left, col);
                chkMax(realmOf[cur].right, col+1);
            }
        }

        debug for (auto [key, value]: realmOf) {
            io << key << ' ' << (std::string)value << endl;
        }

        std::vector<std::pair<int, int>> limits;
        std::vector<char> exist;
        
        for (auto [ch, realm]: realmOf) {
            auto check = [&](int r, int c) -> void {
                if (origin.at(r).at(c) != ch) {
                    limits.push_back({ch, origin[r][c]});
                }
            };
            auto [top, left, bottom, right] = realm;
            from(r, top, bottom)  check(r, left);
            from(r, top, bottom)  check(r, right-1);
            from(c, left, right)  check(top, c);
            from(c, left, right)  check(bottom-1, c);
        }

        debug for (auto [a, b]: limits) {
            io << "limit ";
            io.push(a), io.push(' '), io.push(b), io.push(endl);
        }

        std::vector<std::set<char>> graph(MAX_POINT);
        std::vector<int> in(MAX_POINT);
        for (auto [a, b]: limits) {
            graph.at(a).insert(b);
        }

        from(i, 0ULL, graph.size()) {
            auto &set = graph[i];
            if (isExist[i])  exist.push_back(i);
            for (auto dest: set) {
                in.at(dest)++;
                debug io << (char)i << ' ' << dest << endl;
            }
        }

        std::vector<char> solution(realmOf.size());
        std::vector<char> vis(MAX_POINT);
        std::function<void(char, int)> dfs = [&](char top, int depth) {
            if (vis[top])  return;
            vis[top] = '\1';
            // debug io << std::format("dfs while top = {}, depth = {}", top, depth) << endl;
            solution.at(depth) = top;
            if (depth+1 >= (int)solution.size()) {
                for (auto ch: solution)  io.push(ch);
                io << endl;
                goto egg;
            } 
            for (auto dest: graph.at(top)) {
                in[dest]--;  // 删除限制
            }
            for (auto dest: exist)  if (in[dest] == 0) {
                dfs(dest, depth+1);  // 继续搜索
            }
            for (auto dest: graph.at(top)) {
                in[dest]++;  // 把这个点再加回来，重新启用限制
            }
        egg:
            vis[top] = '\0';
        };

        for (auto i: exist) {
            if (in.at(i) == 0) {
                dfs(i, 0);
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_6383929149905497::solve();
    return 0;
}