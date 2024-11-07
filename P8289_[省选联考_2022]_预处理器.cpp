/**
 * @link https://www.luogu.com.cn/problem/P8289
 */

#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define rev(i,e,b) for(int i=(e);i>=(b);i--)
#define main() main(int argc, char const *argv[])
#define chkMax(base,cmp...) (base=std::max({(base),##cmp}))
#define chkMin(base,cmp...) (base=std::min({(base),##cmp}))
#define never if constexpr(0)
#define always if constexpr(1)
#define bitOr(x,y) (((x)&(y))^(((x)^(y))|(~(x)&(y))))
#define Infinity 2147483647
bool DEBUG_MODE=false;
typedef long long ll; typedef unsigned long long ull;

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

#include <bits/stdc++.h>
#define USE_FREAD
// #undef USE_FREAD
// 取消注释上一行会使用 getchar() 替代 fread，可以不使用 EOF 结束读入，但是降低性能 
namespace lib{
    using std::string;
    struct __IO {
        virtual inline char gc() = 0;
        inline bool blank(char ch) { return ch<=' ' or ch==127; }
        inline bool isd(char x) {return (x>='0' and x<='9');}
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
        virtual inline void push(const char c) = 0;
        
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
        template <class T> inline auto& operator>>(T&x) {  read(x); return *this; }
        template <class T> inline auto& operator<<(const T&x) {  write(x); return *this; }
    };

    template <const long long MAXSIZE, const long long MAX_ITEM_SZ=500>
    struct IO: __IO {
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
        inline void push(const char c) {
            if (pp-pbuf==MAXSIZE) sync();
            *pp++=c;
        }
#else
        inline void gc() { return getchar(); }
        inline void push(const char c) { putchar(c); }
#endif
    };

    struct SIO: __IO {
        const string& src;
        size_t index = 0;

        SIO(const string& src): src(src) { index=0; }
        inline char gc() {
            return index>=src.size()? '\0': src[index++];
        }
        inline void push(const char c) { putchar(c); }
    };

    struct SIO_w: __IO {
        string& src;
        size_t index = 0;

        SIO_w(string& src): src(src) { index=0; }
        inline char gc() {
            return index>=src.size()? '\0': src[index++];
        }
        inline void push(const char c) { src.push_back(c); }
    };

    IO<1<<20> io;
    const char endl[] = "\n";

}
using namespace lib;


namespace Solution {

    int N;
    std::unordered_map<std::string, std::string> names;
    std::vector<std::string> code;
    // std::vector<std::string> gen;

    ull hash(const std::string& s) {
        ull res = 0;
        for (auto ch: s) {
            res = res * 31 + ch;
        }
        debug io.writeln(std::format("{} => {}", s, res));
        return res;
    }
    
    void init() {
        io >> N;
        from(_, 1, N) {
            code.push_back("");
            io.readln(code.back());
            // code.back().push_back('\n');
        }
    }

    inline bool check(char ch) {
        return ch == '_' or ('0' <= ch and ch <= '9') or ('a' <= ch and ch <= 'z') or ('A' <= ch and ch <= 'Z');
    }

    /**
     * 展开 src 中的宏定义，最终结果写入 dest
     */
    void expandAndWrite(const std::string &src, std::string &dest) {
        static std::unordered_set<std::string> vis;
        debug {
            io.writeln(std::format("eAW {} {}", src, dest));
            for(auto i: vis)  io << i << ' ';
            io << endl;
        }
        SIO sio(src);
        char ch = sio.gc();
        std::string cur;
        while (ch) {
            // io << ch << endl;
            if (check(ch)) {  // 标识符
                cur.clear();
                while (check(ch)) {
                    cur.push_back(ch), ch = sio.gc();
                }
                // io << cur;
                if (ch)  sio.index--;

                if (names.find(cur)==names.end() or vis.find(cur)!=vis.end()) {  // 不是宏定义
                    dest += cur;
                } else {
                    vis.insert(cur);
                    expandAndWrite(names[cur], dest);
                    vis.erase(vis.find(cur));
                }
            } else {  // 常规字符
                dest.push_back(ch);
            }
            ch = sio.gc();
        }
    }

    void solve() {
        init();
        from(i, 0, N-1) {
            // gen.push_back("");
            SIO sio(code[i]);
            // auto &target = gen.back();
            auto &line = code[i];
            std::string target;

            if (line[0] == '#') {  // 预处理器命令
                std::string command;
                sio >> command;

                if (command[1] == 'd') {  // #define
                    std::string name;
                    sio >> name;
                    sio.readln(names[name]);
                    // io << names[name];
                } else {  // #undef
                    std::string name;
                    sio >> name;
                    names.erase(names.find(name));
                }

                // target 保留为空
            } else {  // 常规命令
            // io << line << endl;
                expandAndWrite(line, target);
            }

            io << target << endl;
        }
    }
}


int main() {
    initDebug;
    Solution::solve();
    return 0;
}