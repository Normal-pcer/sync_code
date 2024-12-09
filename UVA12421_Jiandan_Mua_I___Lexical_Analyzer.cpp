/**
 * @link https://www.luogu.com.cn/problem/UVA12421
 */

#include "./lib_v2.hpp"

using namespace lib;

namespace Solution_1223658170809683 {
    namespace Tokenizer {

        void raise(const std::string &message) {
            std::cout << "Tokenizer threw an exception." << std::endl;
            std::cout << message << std::endl;
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

        bool isd(char ch) { return '0' <= ch and ch <= '9'; }

        auto tokenize(const std::string &code) {
            enum parseStatus {
                nothing,            // 空闲
                identifier,         // 读取标识符
                string,             // 字符串
                integer,            // 整数
                realNumber,         // 实数
                comment,            // 注释
            };
            std::stringstream stream(code);
            auto line = 1;
            char ch;
            while (stream.get(ch)) {

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

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1223658170809683::solve();
    return 0;
}