#include <bits/stdc++.h>
#include "./libs/fixed_int.hpp"

auto calc(std::string expr) -> i32 {
    while (expr.back() == '\r')  expr.pop_back();
    struct Node {
        enum Type { Number, Operator } type;
        i32 val;
    };

    std::vector<Node> postfix;
    {
        std::vector<char> st;
        auto priority = [](char c) -> i32 {
            switch (c) {
            case '*':  return 2;
            case '/':  return 2;
            case '+':  return 1;
            case '-':  return 1;
            default:   return 0;
            }
        };
        for (auto ch : expr) {
            if (ch == '(') {
                st.push_back(ch);
            } else if (ch == ')') {
                while (not st.empty() and st.back() != '(') {
                    postfix.push_back({Node::Operator, st.back()});
                    st.pop_back();
                }
                st.pop_back();
            } else if (ch == '+' or ch == '-' or ch == '*' or ch == '/') {
                while (not st.empty() and priority(st.back()) >= priority(ch)) {
                    postfix.push_back({Node::Operator, st.back()});
                    st.pop_back();
                }
                st.push_back(ch);
            } else if (ch >= '0' && ch <= '9') {
                postfix.push_back({Node::Number, ch - '0'});
            }
        }
        while (not st.empty()) {
            postfix.push_back({Node::Operator, st.back()});
            st.pop_back();
        }
    }

    std::vector<i32> st_nums;
    for (auto node : postfix) {
        if (node.type == Node::Number) {
            st_nums.push_back(node.val);
        } else {
            auto b = st_nums.back(); st_nums.pop_back();
            auto a = st_nums.back(); st_nums.pop_back();
            if (node.val == '+')  st_nums.push_back(a + b);
            else if (node.val == '-')  st_nums.push_back(a - b);
            else if (node.val == '*')  st_nums.push_back(a * b);
            else if (node.val == '/')  st_nums.push_back(a / b);
            else  assert(false);
        }
    }
    return st_nums.back();
}

int main() {
    std::string s, s0;
    while (std::getline(std::cin, s)) s0 = s;

    std::cout << calc(s0) << std::endl;
    return 0;
}
