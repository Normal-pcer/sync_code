/**
 * 
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
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL;
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
const char endl = '\n';
// #include "libs/sio.hpp"
using namespace lib;

namespace Solution_1223658170809683 {

    namespace Tokenizer {

        void raise(const std::string &message) {
            std::cout << "Tokenizer threw an exception." << endl;
            std::cout << message << endl;
            throw 32;
        }

        enum TokenType {
            Reserved,
            Number,
            EOL,
        };
        enum ReversedType {

        };
        struct Token {
            TokenType type;
            std::string origin = "";
        };

        auto tokenize(const std::string &code) {
            enum parseStatus {
                nothing,            // 空闲
                identifier,         // 读取标识符
                string,             // 字符串
                integer,            // 整数
                realNumber,         // 实数
                comment,            // 注释
            };
            std::stack<parseStatus> st;
            std::string top;

            std::vector<Token> res;

            auto ptr = 0ULL;
            auto line = 1ULL;
            st.push(nothing);
            for (; ptr < code.size(); ptr++) {
                char cur = code[ptr];

                if (cur == '\n' or cur == '\r') {  // 读取到换行符
                    res.push_back({EOL});
                    if (st.top() == string)  raise("Expected end of string.");
                    st.pop(), st.push(nothing);  // 清空所有读取
                }
            }
        }
    }

    void solve() {
        std::string code;
        std::string line;

        while (std::getline(std::cin, line))  code += line, code.push_back('\n');
        std::cout << code;
    }
}


int main() {
    initDebug;
    Solution_1223658170809683::solve();
    return 0;
}