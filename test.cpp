#include <bits/stdc++.h>

int const inf = 0x3f3f3f3f;

enum OperatorType {
    None = 'o', Plus = '+', Sub = '-', Mul = '*', Div = '/', Mod = '%', Positive = 'P', Negative = 'N', 
    LBracket = '(', Call = 'C', LFunctionArgsBracket = 'L', FunctionArgsComma = ',', Subscript = '[', LSquareBracket = 'S'
};
struct OperatorInfo {
    int priority;
    bool left;
};
constexpr OperatorInfo info(OperatorType type) {
    switch (type) {
    case Call:  return {2, false};
    case Subscript:  return {2, false};
    case Positive:  return {3, true};
    case Negative:  return {3, true};
    case Mul:  return {5, false};
    case Div:  return {5, false};
    case Mod:  return {5, false};
    case Plus:  return {6, false};
    case Sub:  return {6, false};
    case FunctionArgsComma:  return {17, false};
    default:  return {inf, true};
    }
}
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    std::string expr;  std::cin >> expr;
    struct OP {
        OperatorType op;
        int args_remains = 0;
    };
    auto it = expr.begin();
    while (it != expr.end()) {
        std::string ans;
        std::vector<OP> ops {{None, 1}};
        auto add = [&](OperatorType type, int args_remains) {
            if (info(type).left) {
                while (not ops.empty() and info(ops.back().op).priority < info(type).priority)  ans.push_back((char)ops.back().op), ops.pop_back();
            } else {
                while (not ops.empty() and info(ops.back().op).priority <= info(type).priority)  ans.push_back((char)ops.back().op), ops.pop_back();
            }
            ops.push_back({type, args_remains});
        };
        for (; it != expr.end(); it++) {
            char ch = *it;
            if ('0' <= ch and ch <= '9') {
                ans.push_back(ch);
                ops.back().args_remains--;
            } else {
                if (ch == '(') {
                    if (not ops.empty() and ops.back().args_remains == 0) {
                        ops.push_back({Call, 0});
                        ops.push_back({LFunctionArgsBracket, 1});
                    } else {
                        ops.back().args_remains--;
                        ops.push_back({LBracket, 1});
                    }
                } else if (ch == '+') {
                    if (not ops.empty() and ops.back().args_remains != 0) {
                        assert(ops.back().args_remains == 1);
                        ops.back().args_remains--;
                        add(Positive, 1);
                    } else {
                        add(Plus, 1);
                    }
                } else if (ch == '-') {
                    if (not ops.empty() and ops.back().args_remains != 0) {
                        assert(ops.back().args_remains == 1);
                        ops.back().args_remains--;
                        add(Negative, 1);
                    } else {
                        add(Sub, 1);
                    }
                } else if (ch == '*') {
                    add(Mul, 1);
                } else if (ch == '/') {
                    add(Div, 1);
                } else if (ch == '%') {
                    add(Mod, 1);
                } else if (ch == ')') {
                    while (info(ops.back().op).priority != inf) {
                        ans.push_back((char)ops.back().op), ops.pop_back();
                    }
                    if (ops.back().op == LFunctionArgsBracket) {
                        bool flag = ops.back().args_remains == 1;
                        ops.pop_back();
                        if (flag)  ops.push_back({None, 0});
                    } else {
                        assert(ops.back().op == LBracket and ops.back().args_remains == 0);
                        ops.pop_back();
                    }
                } else if (ch == ',') {
                    add(FunctionArgsComma, 1);
                } else if (ch == '[') {
                    ops.push_back({Subscript, 0});
                    ops.push_back({LSquareBracket, 1});
                } else if (ch == ']') {
                    while (info(ops.back().op).priority != inf) {
                        ans.push_back((char)ops.back().op), ops.pop_back();
                    }
                    assert(ops.back().op == LSquareBracket);
                    bool flag = ops.back().args_remains == 1;
                    ops.pop_back();
                    if (flag)  ops.push_back({None, 0});
                } else if (ch == ';') {
                    it++;
                    break;
                }
            }
        }
        while (ops.size() != (size_t)1) {
            ans.push_back((char)ops.back().op), ops.pop_back();
        }
        std::cout << ans << std::endl;
    }
}