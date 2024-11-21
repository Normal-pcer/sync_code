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

#include "./libs/sio.hpp"
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