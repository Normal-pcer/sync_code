/**
 * @link https://www.luogu.com.cn/problem/AT_arc166_a
 * @link https://atcoder.jp/contests/arc166/tasks/arc166_a
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
    const char endl = '\n';

}

#include "./libs/range.hpp"

using namespace lib;

int T;

namespace Solution_3822230069806222 {

    struct StringView {
        std::string &base;
        size_t begin_pos;
        size_t end_pos;

        StringView(std::string &base, size_t begin_pos, size_t end_pos):
            base(base), begin_pos(begin_pos), end_pos(end_pos)  {}

        auto begin() const { return base.begin() + begin_pos; }
        auto end() const { return base.begin() + end_pos; }
        auto empty() const { return begin_pos == end_pos; }
        const char &at(size_t x) const { return base.at(begin_pos + x); }
        char &at(size_t x) { return base.at(begin_pos + x); }
        const char &operator[] (size_t x) const { return base[begin_pos + x]; }
        char &operator[] (size_t x) { return base[begin_pos + x]; }

        size_t size() const { return end_pos - begin_pos; }

        StringView slice(size_t x, size_t y = 0) {
            return StringView(base, begin_pos + x, begin_pos + y);
        }
    };

    struct AttachedString {
        char prefix;
        StringView content;

        AttachedString(char prefix, StringView content): prefix(prefix), content(content) {}
        AttachedString(StringView origin): 
            prefix(origin.empty()? '\0': origin.at(0)), content(origin.slice(1)) {}
        
        auto empty() const { return prefix == '\0'; }
        auto size() const { return content.size() + 1; }
    };

    // 检查能否用 origin 凑出 target
#if false
    bool check(AttachedString origin, AttachedString target) {
        assert(origin.size() == target.size());
        if (origin.empty())  return true;

        // 首位能够匹配，向后检查
        if (origin.prefix == target.prefix or origin.prefix == 'C')  return check(origin.content, target.content);

        // 如果首位是 A，期望 B，尝试替换
        if (origin.prefix == 'A' and target.prefix == 'B') {
            // 具体地，对于
            // A.......
            // 如果它能变成 AB.....（后面与 target 相同），则可行
            // AAABB
            // BBAAA

            //
        }
    }
#endif
    void solve() {
        // debug std::cout << std::format("T = {}", T) << std::endl;
        int N;  io >> N;
        std::string s1, s2;
        io >> s1 >> s2;

        assert(s1.size() == s2.size());

        auto s2_A_cnt = 0, s2_B_cnt = 0, s2_C_cnt = 0;
        auto s1_A_cnt = 0, s1_B_cnt = 0, s1_C_cnt = 0;

        for (auto i: s1) {
            if (i == 'A')  s1_A_cnt++;
            else if (i == 'B')  s1_B_cnt++;
            else  s1_C_cnt++;
        }

        for (auto i: s2) {
            if (i == 'A')  s2_A_cnt++;
            else if (i == 'B')  s2_B_cnt++;
            else  s2_C_cnt++;
        }

        for (auto idx: range(s1.size())) {
            auto &i = s1.at(idx);
            if (i == 'C' and s2.at(idx) != 'C') {
                if (s1_A_cnt < s2_A_cnt)  i = 'A', s1_A_cnt++, s1_C_cnt--;
                else if (s1_B_cnt < s2_B_cnt)  i = 'B', s1_B_cnt++, s1_C_cnt--;
            }
        }

        // std::cout << s1 << ' ' << s2 << std::endl;

        if (s1_A_cnt != s2_A_cnt or s1_B_cnt != s2_B_cnt or s1_C_cnt != s2_C_cnt)  return void(io << "No" << endl);
        // ABACB
        // BBACA

        std::vector<std::pair<int, int>> B_pairs;
        for (auto i = (size_t)0, j = (size_t)0; i <= s1.size(); i++, j++) {
            while (i < s1.size() and s1.at(i) != 'B')  i++;
            while (j < s2.size() and s2.at(j) != 'B')  j++;
            B_pairs.push_back({i, j});
            if (i < j)  return void(io << "No" << endl);
        }
        assert(B_pairs.back().first == B_pairs.back().second);
        B_pairs.pop_back();

        std::set<int> all_A, all_B, all_C;
        for (auto i: range(s1.size())) {
            if (s1.at(i) == 'A')  all_A.insert(i);
            if (s1.at(i) == 'B')  all_B.insert(i);
            if (s1.at(i) == 'C')  all_C.insert(i);
        }

        // io << "all_A ";  for (auto i: all_A) io << i << ' ';
        // io << endl;
        // io << "all_B ";  for (auto i: all_B)  io << i << ' ';
        // io << endl;
        // io << "all_C ";  for (auto i: all_C)  io << i << ' ';
        // io << endl;

        // for (auto [i, j]: B_pairs)  std::cout << i << ' ' << j << endl;

        for (auto [i, j]: B_pairs) {
            // 如果 i 和 j 中间（开区间）没有 B 和 C
            auto find1 = all_B.upper_bound(j);
            auto find2 = all_C.upper_bound(j);
            // io << i << ' ' << j << ' ' << find1 - all_B.begin() << ' ' << find2 - all_C.begin() << endl;

            if ( not (
                    (find1 == all_B.end() or *find1 >= i) and 
                    (find2 == all_C.end() or *find2 >= i)
                )
            ) {
                goto egg;
            } else {
                if (find1 != all_B.end())  all_B.erase(find1);
            }

        }

        io << "Yes" << endl;
        return;
    egg:
        io << "No" << endl;
    }
}

int main() {
    initDebug;

    io >> T;

    while (T --> 0)  Solution_3822230069806222::solve();
    return 0;
}
