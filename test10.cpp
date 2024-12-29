/**
 * @link https://www.luogu.com.cn/problem/P1175
 */

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_1836603151363745 {
    struct OperatorInfo {
        int priority;
        bool leftAssociative;
    };
    constexpr OperatorInfo info(char op) {
        switch (op) {
        case '\x00':  return {3, true};  // Unary +
        case '\x01':  return {3, true};  // Unary -
        case '*':  return {5, false};
        case '/':  return {5, false};
        case '%':  return {5, false};
        case '+':  return {6, false};
        case '-':  return {6, false};
        case '(':  return {inf, false};
        case ')':  return {inf, false};
        default:   return assert(false), OperatorInfo{0, false};
        }
    }
    void solve() {
        std::string expr;
        std::cin >> expr;

        std::deque<char> op;
        std::string ans;
        for (auto i = 0; i != int(expr.size()); i++) {
            char c = expr[i];
            if ('0' <= c and c <= '9') {
                ans.push_back(c);
            } else if (c == '(') {
                op.push_back(c);
            } else if (c == ')') {
                while (not op.empty() and op.back() != '(') {
                    if (info(op.back()).priority != inf)  ans.push_back(op.back());
                    op.pop_back();
                }
                op.pop_back();
            } else {
                auto [priority, left] = info(c);
                if (left) {
                    while (not op.empty() and info(op.back()).priority < priority) {
                        if (info(op.back()).priority != inf)  ans.push_back(op.back());
                        op.pop_back();
                    }
                    op.push_back(c);
                } else {
                    while (not op.empty() and info(op.back()).priority <= priority) {
                        if (info(op.back()).priority != inf)  ans.push_back(op.back());
                        op.pop_back();
                    }
                    op.push_back(c);
                }
            }
        }
        while (not op.empty() and info(op.back()).priority) {
            if (info(op.back()).priority != inf)  ans.push_back(op.back());
            op.pop_back();
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1836603151363745::solve();
    return 0;
}
