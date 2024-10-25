/**
 * @link https://www.luogu.com.cn/problem/P5380
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
        
        template <class Func>
        bool all(Func&& f) {
            for (auto& i: *this) {
                if (!f(i)) {
                    return false;
                }
            }
            return true;
        }

        bool all() { return all([](auto&x){return x;}); }

        template <class Func>
        bool some(const Func&& f) {
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
            for (const auto& i: s)  size += i.size();
            res.reserve(size);
            bool first = true;
            for (const auto& i: s) {
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

using namespace lib;





const int _X = 9;
const int _Y = 10;

struct Vec2d {
    int x, y;
    Vec2d operator+(const Vec2d &other) const { return {x+other.x, y+other.y}; }
    Vec2d operator-(const Vec2d &other) const { return {x-other.x, y-other.y}; }
    int operator*(const Vec2d &other) const { return x*other.x + y*other.y; }
    Vec2d operator*(const int other) const { return {x*other, y*other}; }
    Vec2d operator/(const int other) const { return {x/other, y/other}; }
    int cross(const Vec2d &other) const { return x*other.y - y*other.x; }
    bool operator==(const Vec2d &other) const { return x==other.x and y==other.y; }
    bool operator!=(const Vec2d &other) const { return x!=other.x or y!=other.y; }

    // 两点间欧几里得距离的平方
    int sqrDistance(const Vec2d &other) const { 
        return (x-other.x)*(x-other.x)+(y-other.y)*(y-other.y); 
    }
    // 曼哈顿距离
    int manhattan(const Vec2d &other) const { return abs(x-other.x)+abs(y-other.y); }
    // 切比雪夫距离
    int chebyshev(const Vec2d &other) const { return std::max(abs(x-other.x), abs(y-other.y)); }

    int sqrAbs() const { return x*x+y*y; }
};

enum Team {
    Red,
    Blue,
};

struct Piece;

vector<Piece> pieces;
struct Piece {
    string  name;
    Vec2d   pos;
    Team    team;

    // 检查是否可以到达目标位置，但是只对位置进行判断
    bool reachable_pos(Vec2d dest) {
        if (name == "captain") {
            return pos.sqrDistance(dest) == 1;
        } else if (name == "guard") {
            return pos.manhattan(dest) == 2 and pos.chebyshev(dest) == 1;
        } else if (name == "elephant") {
            return pos.manhattan(dest) == 4 and pos.chebyshev(dest) == 2 and 
                not pieces.some(lambda(p, (p.pos-pos)*2==(dest-pos))); 
        } else if (name == "horse") {
            return pos.manhattan(dest) == 3 and pos.chebyshev(dest) == 2 and 
                not pieces.some(lambda(p, 
                    p.pos.sqrDistance(pos) == 1 and (p.pos-pos)*(dest-pos)==2));
        } else if (name == "car") {
            auto a = dest-pos;
            if (pos.chebyshev(dest) != pos.manhattan(dest))  return false;
            if (pos.manhattan(dest) == 0)  return false;
            auto meta = a / pos.manhattan(dest);
            for (auto p=pos+meta; p!=dest; p=p+meta) {
                if (pieces.some(lambda(piece,piece.pos==p)))  return false;
            }
            return true;
        } else if (name == "soldier") {
            return pos.chebyshev(dest) == 1;
        } else {  // duck
            auto a = dest-pos;
            return pos.sqrDistance(dest) == 13 and not pieces.some([&](auto p) {
                auto b = p.pos-pos;
                return (a*b==8 and b.sqrAbs()==5) or (a*b==3 and b.sqrAbs()==1);
            });
        }
    }

    bool reachable(Vec2d dest) {
        return 1<=dest.x and dest.x<=_X and
        1<=dest.y and dest.y<=_Y and
        reachable_pos(dest) and 
        not pieces.some(lambda(p, p.pos==dest and p.team==team));
    }
};

namespace Solution {

    int Q;
    
    void init() {
        pieces.push_back(Piece{"captain", {5, 1}, Team::Red});
        pieces.push_back(Piece{"captain", {5, 10}, Team::Blue});
        pieces.push_back(Piece{"car", {1, 1}, Team::Red});
        pieces.push_back(Piece{"car", {9, 1}, Team::Red});
        pieces.push_back(Piece{"car", {1, 10}, Team::Blue});
        pieces.push_back(Piece{"car", {9, 10}, Team::Blue});
        pieces.push_back(Piece{"horse", {2, 1}, Team::Red});
        pieces.push_back(Piece{"horse", {8, 1}, Team::Red});
        pieces.push_back(Piece{"horse", {2, 10}, Team::Blue});
        pieces.push_back(Piece{"horse", {8, 10}, Team::Blue});
        pieces.push_back(Piece{"elephant", {3, 1}, Team::Red});
        pieces.push_back(Piece{"elephant", {7, 1}, Team::Red});
        pieces.push_back(Piece{"elephant", {3, 10}, Team::Blue});
        pieces.push_back(Piece{"elephant", {7, 10}, Team::Blue});
        pieces.push_back(Piece{"guard", {4, 1}, Team::Red});
        pieces.push_back(Piece{"guard", {6, 1}, Team::Red});
        pieces.push_back(Piece{"guard", {4, 10}, Team::Blue});
        pieces.push_back(Piece{"guard", {6, 10}, Team::Blue});
        pieces.push_back(Piece{"duck", {1, 3}, Team::Red});
        pieces.push_back(Piece{"duck", {9, 3}, Team::Red});
        pieces.push_back(Piece{"duck", {1, 8}, Team::Blue});
        pieces.push_back(Piece{"duck", {9, 8}, Team::Blue});
        pieces.push_back(Piece{"soldier", {1, 4}, Team::Red});
        pieces.push_back(Piece{"soldier", {3, 4}, Team::Red});
        pieces.push_back(Piece{"soldier", {5, 4}, Team::Red});
        pieces.push_back(Piece{"soldier", {7, 4}, Team::Red});
        pieces.push_back(Piece{"soldier", {9, 4}, Team::Red});
        pieces.push_back(Piece{"soldier", {1, 7}, Team::Blue});
        pieces.push_back(Piece{"soldier", {3, 7}, Team::Blue});
        pieces.push_back(Piece{"soldier", {5, 7}, Team::Blue});
        pieces.push_back(Piece{"soldier", {7, 7}, Team::Blue});
        pieces.push_back(Piece{"soldier", {9, 7}, Team::Blue});

        io >> Q;
    }

    void solve() {
        init();
        Piece& kingRed = pieces[0];
        Piece& kingBlue = pieces[1];

        bool round = 0;
        bool gameOver = false;
        from(_, 1, Q) {
            // debug io.writeln("====", _, "====");
            if (gameOver) {
                io.writeln("Invalid command");
                continue;
            }
            never {
                auto tmp = pieces.filter(lambda(fk, fk.pos.x==4 and fk.pos.y==7));
                if (not tmp.empty())
                    debug io.writeln((int)tmp[0].team, tmp[0].name);
            }
            int x1, y1, x2, y2;
            io >> y1 >> x1 >> y2 >> x2;
            x1+=1, y1+=1, x2+=1, y2+=1;
            never io.writeln(x1 , y1 , x2 , y2);
            auto src = Vec2d{x1, y1};
            auto dest = Vec2d{x2, y2};

            auto selected = std::find_if(pieces.begin(), pieces.end(), 
                lambda(p, p.pos==src and p.team==round));
            if (selected == pieces.end()) {
                io.writeln("Invalid command");
                continue;
            }

            bool available = selected->reachable(dest);
            if (not available) {
                io.writeln("Invalid command");
                continue;
            }
            bool isKingKilled = false;
            io.write(selected->team==Team::Red? "red": "blue", selected->name);
            io.write(';');
            auto attackedIter = std::find_if(pieces.begin(), pieces.end(), lambda(p, p.pos==dest));
            selected->pos = dest;
            if (attackedIter == pieces.end()) {
                io.write("NA");
            } else {
                isKingKilled = (attackedIter->name=="captain");
                io.write(attackedIter->team==Team::Red? "red": "blue", attackedIter->name);
                pieces.erase(attackedIter);
            }
            io.write(';');
            bool isKingInDanger = false;
            isKingInDanger |= pieces.some(lambda(p, p.reachable(kingRed.pos) and p.team==Team::Blue));
            isKingInDanger |= pieces.some(lambda(p, p.reachable(kingBlue.pos) and p.team==Team::Red));
            isKingInDanger &= !isKingKilled;
            io.write(isKingInDanger? "yes": "no");
            io.write(';');
            io.writeln(isKingKilled? "yes": "no");
            gameOver |= isKingKilled;
            round ^= 1;
        }
    }
}


int main() {
    initDebug;
    Solution::solve();
    return 0;
}

/*
18
0 0 7 0
9 0 8 0
0 1 1 3
0 2 2 0
0 3 1 2
0 4 0 3
9 4 8 4
3 2 2 3
7 0 4 2
7 0 5 3
9 2 7 4
2 0 4 3
9 1 8 3
4 3 6 6
7 4 9 2
8 4 9 4
6 6 9 4
9 8 8 8


Invalid command
Invalid command
Invalid command
Invalid command
red guard;NA;no;no
Invalid command
blue captain;NA;no;no
red soldier;NA;no;no
Invalid command
Invalid command
blue elephant;NA;no;no
red duck;NA;no;no
blue horse;NA;no;no
red duck;blue soldier;no;no
Invalid command
blue captain;NA;yes;no
red duck;blue captain;no;yes
Invalid command
*/