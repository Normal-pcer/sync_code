/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=3342
 */

#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#include <bits/stdc++.h>
bool DEBUG_MODE=false;
#define debug never
template <typename T> inline auto chkMax(T& base, const T& cmp) { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T& base, const T& cmp) { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;
const char endl = '\n';

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{
#if __cplusplus > 201703LL
namespace ranges { using namespace std::ranges; }
namespace views { using namespace std::ranges::views; }
#endif
}
#include <bits/stdc++.h>
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
    <1<<20>
#endif
    io;
}
using namespace lib;

#define at operator[]
#define ll int
namespace Solution_9736888855439916 {
    class BIT {
        // std::vector<int> c;
        int c[1048576];

        static int inline constexpr lowbit(int x) noexcept { return x & -x; }
    public:
        BIT() {}
        int query(int x) const {
            x++;  auto ans = 0;
            while (x) {
                ans += c[x];
                x -= lowbit(x);
            }
            return ans;
        }
        void update(int x, int val) {
            x++;
            while (x < 1048576) {
                c[x] += val;
                x += lowbit(x);
            }
        }
    } tr;

    struct Segment {
        int left, right, index;
        bool update;
    };

    std::vector<Segment> segs;
    std::vector<int> values {-inf, +inf};
    int getIndex(int x) {
        auto it = std::lower_bound(values.begin(), values.end(), x);
        assert(*it == x);
        return std::distance(values.begin(), it);
    }

    void cdq(std::vector<Segment>::iterator begin, std::vector<Segment>::iterator end, auto &ans) {
        // auto x = std::distance(segs.begin(), begin), y = std::distance(segs.begin(), end);
        // debug  io << std::format("cdq({}, {})", x, y) << endl;
        
        if (end - begin == 1)  return;
        auto mid = begin + ((end - begin) >> 1);

        cdq(begin, mid, ans);
        cdq(mid, end, ans);

        std::sort(begin, mid, lam(x, y, x.right < y.right));
        std::sort(mid, end, lam(x, y, x.left < y.left));

        auto i = begin, j = mid;
        auto cnt = 0;
        while (i != mid or j != end) {
            if (j == end or (i != mid and i->right < j->left)) {
                if (i->update)  cnt++;
                i++;
            } else {
                if (not j->update) {
                    ans.at(j->index) += cnt;
                    // debug {
                    //     if (begin != i) {
                    //         debug io << std::format("[{}]({}, {}) is not cross with: ", j->index, j->left, j->right);
                    //         for (auto it = begin; it != i; ++it)  io << std::format("[{}]({}, {})", it->index, it->left, it->right) << ' ';
                    //         io << endl;
                    //     }
                    // }
                }
                j++;
            }
        }
    }

    void cdq2(std::vector<Segment>::iterator begin, std::vector<Segment>::iterator end, auto &ans) {
        // auto x = std::distance(segs.begin(), begin), y = std::distance(segs.begin(), end);
        // debug  io << std::format("cdq({}, {})", x, y) << endl;
        
        if (end - begin == 1)  return;
        auto mid = begin + ((end - begin) >> 1);

        cdq2(begin, mid, ans);
        cdq2(mid, end, ans);

        std::sort(begin, mid, lam(x, y, x.left > y.left));
        std::sort(mid, end, lam(x, y, x.right > y.right));

        auto i = begin, j = mid;
        auto cnt = 0;
        while (i != mid or j != end) {
            if (j == end or (i != mid and i->left > j->right)) {
                if (i->update)  cnt++;
                i++;
            } else {
                if (not j->update) {
                    ans.at(j->index) += cnt;
                    // debug {
                    //     if (begin != i) {
                    //         debug io << std::format("[{}]({}, {}) is not cross with: ", j->index, j->left, j->right);
                    //         for (auto it = begin; it != i; ++it)  io << std::format("[{}]({}, {})", it->index, it->left, it->right) << ' ';
                    //         io << endl;
                    //     }
                    // }
                }
                j++;
            }
        }
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, Q;  io >> N;
        segs.resize(N);
        for (auto i = 0; i < N; i++) {
            auto &[left, right, index, update] = segs.at(i);
            io >> left >> right, index = i, update = true;
            values.push_back(left), values.push_back(right);
        }
        io >> Q;
        segs.resize(N + Q);
        values.reserve(1<<24);
        for (auto i = 0; i < Q; i++) {
            auto &[left, right, index, update] = segs.at(N+i);
            io >> left >> right, index = i, update = false;
            values.push_back(left), values.push_back(right);
        }

        std::sort(values.begin(), values.end());
        values.erase(std::unique(values.begin(), values.end()), values.end());
        for (auto &i: segs)  i.left = getIndex(i.left), i.right = getIndex(i.right);
        auto const begin = segs.begin(), end = segs.end(), mid = std::partition(begin, end, lam(x, x.update));

        // tr = BIT(values.size()+5);

//         std::vector<ll> left_cross(Q);
//         {
//             auto &ans = left_cross;
//             std::sort(begin, mid, lam(const &x, const &y, x.right < y.right));
//             std::sort(mid, end, lam(const &x, const &y, x.right < y.right));
//             auto i = begin, j1 = mid, j2 = mid;
//             tr.tick();
//             while (j1 != end or j2 != end) {
//                 if (j1 == end or (j2 != end and j2->left < j1->right)) {
//                     while (i != mid and i->right < j2->left) {
//                         debug io << std::format("tr.update({}, +1)", i->right) << endl;
//                         tr.update(i->left, +1);
//                         i++;
//                     }
// debug io << std::format("[{}]ans.at({}) -= [{}]tr.query({})", ans.at(j2->index), j2->index, tr.query(j2->left-1), j2->left-1) << endl;
//                     ans.at(j2->index) -= tr.query(j2->left-1);
//                     j2++;
//                 } else {
//                     while (i != mid and i->right < j1->right) {
//                         tr.update(i->left, +1);
//                         i++;
//                     }
//                     ans.at(j1->index) += tr.query(j1->left-1);
//                     j1++;
//                 }
//             }
//         }
        std::vector<ll> left_contain(Q);
        {
            auto &ans = left_contain;
            std::sort(begin, mid, lam(const &x, const &y, x.right < y.right));
            std::sort(mid, end, lam(const &x, const &y, x.right < y.right));
            auto i = begin, j = mid;
            while (j != end) {
                while (i != mid and i->right <= j->right) {
                    tr.update(i->left, +1);
                    i++;
                }
                ans.at(j->index) += tr.query(getIndex(inf)) - tr.query(j->left - 1);
                j++;
            }
        }
        // std::vector<ll> right_cross(Q);
        // {
        //     auto &ans = right_cross;
        //     std::sort(begin, mid, lam(const &x, const &y, x.left > y.left));
        //     std::sort(mid, end, lam(const &x, const &y, x.left > y.left));
        //     auto i = begin, j1 = mid, j2 = mid;
        //     tr.tick();
        //     while (j1 != end or j2 != end) {
        //         if (j1 == end or (j2 != end and j2->right > j1->left)) {
        //             while (i != mid and i->left > j2->right) {
        //                 debug io << std::format("tr.update({}, +1)", i->right) << endl;
        //                 tr.update(i->right, +1);
        //                 i++;
        //             }
        //             debug  io << std::format("ans.at({}) -= tr.query(inf) - tr.query({})", j2->index, j2->right) << endl;
        //             ans.at(j2->index) -= tr.query(getIndex(inf)) - tr.query(j2->right);
        //             j2++;
        //         } else {
        //             while (i != mid and i->left > j1->left) {
        //                 tr.update(i->right, +1);
        //                 i++;
        //             }
        //             debug  io << std::format("ans.at({}) += tr.query(inf) - tr.query({})", j1->index, j1->right) << endl;
        //             ans.at(j1->index) += tr.query(getIndex(inf)) - tr.query(j1->right);
        //             j1++;
        //         }
                
        //     }
        // }
        // std::vector<ll> rev_contain(Q);
        // {
        //     auto &ans = rev_contain;
        //     std::sort(begin, mid, lam(const &x, const &y, x.right > y.right));
        //     std::sort(mid, end, lam(const &x, const &y, x.right > y.right));
        //     auto i = begin, j = mid;
        //     tr.tick();
        //     while (j != end) {
        //         while (i != mid and i->right >= j->right) {
        //             tr.update(i->left, +1);
        //             i++;
        //         }
        //         ans.at(j->index) += tr.query(j->left);
        //         j++;
        //     }
        // }
        std::vector<ll> not_cross(Q);
        {
            // std::vector<ll> tmp(Q);
            cdq(begin, end, not_cross);
            std::partition(begin, end, lam(x, x.update));
            cdq2(begin, end, not_cross);
            // std::transform(tmp.begin(), tmp.end(), not_cross.begin(), not_cross.begin(), lam(x, y, x+y));
        }
        

        // // debug for (auto i: left_cross)  io << i << ' ';
        // // debug io << endl;
        // debug for (auto i: left_contain)  io << i << ' ';
        // debug io << endl;
        // // debug for (auto i: right_cross)  io << i << ' ';
        // // debug io << endl;
        // // debug for (auto i: rev_contain)  io << i << ' ';
        // // debug io << endl;
        // debug for (auto i: not_cross)  io << i << ' ';
        // debug io << endl;
        for (auto i = 0; i < Q; i++) {
            auto contain = left_contain.at(i);
            // auto cross = left_cross.at(i) + right_cross.at(i) + rev_contain.at(i);
            auto cross = N - not_cross.at(i) - contain;
            io << cross << ' ' << contain << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_9736888855439916::solve();
    return 0;
}
