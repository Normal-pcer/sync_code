#include <bits/stdc++.h>
#define FILENAME "T3"

using i32 = std::int32_t; using i64 = std::int64_t;
using u32 = std::uint32_t; using u64 = std::uint64_t; using uz = std::size_t;
char constexpr endl = '\n';

/**
 * 打个暴力。
 * 先写一个表达式解析。发现这个其实就是前缀表达式。
 * 
 * next_permutation 枚举数字顺序。
 * 期望得分：10
 */
namespace Solution {
    struct Node {
        enum Type {
            None, Min, Max
        } type;
        std::unique_ptr<Node> left = nullptr;
        std::unique_ptr<Node> right = nullptr;

        auto string() const -> std::string {
            if (type == Min)  return left->string() + " " + right->string() + " min";
            if (type == Max)  return left->string() + " " + right->string() + " max";
            return "?";
        }
    };
    // auto parse(std::string const &s) -> std::unique_ptr<Node> {
    //     std::vector<std::unique_ptr<Node>> st;
    //     for (auto it = s.rbegin(); it != s.rend(); ) {
    //         auto ch = *it++;
    //         if (ch == 'n') {
    //             it++, it++;
    //             auto left = std::move(st.back());  st.pop_back();
    //             auto right = std::move(st.back());  st.pop_back();
    //             auto new_node = std::make_unique<Node>(Node::Min, std::move(left), std::move(right));
    //             st.push_back(std::move(new_node));
    //         } else if (ch == 'x') {
    //             it++, it++;
    //             auto left = std::move(st.back());  st.pop_back();
    //             auto right = std::move(st.back());  st.pop_back();
    //             auto new_node = std::make_unique<Node>(Node::Max, std::move(left), std::move(right));
    //             st.push_back(std::move(new_node));
    //         } else if (ch == '?') {
    //             st.push_back(std::make_unique<Node>(Node::None));
    //         }
    //     }
    //     assert(st.size() == 1);
    //     return std::move(st.back());
    // }
    // 使用给定顺序的数字替换所有问号，进行表达式求值
    auto calcWith(std::string const &expr, std::vector<i32> const &nums) -> i32 {
        std::vector<i32> st;
        i32 question_index = 0;  // 当前问号下标
        for (auto it = expr.rbegin(); it != expr.rend(); ) {
            auto ch = *it++;
            if (ch == 'n') {
                it++, it++;
                auto left = st.back();  st.pop_back();
                auto right = st.back();  st.pop_back();
                st.push_back(std::min(left, right));
            } else if (ch == 'x') {
                it++, it++;
                auto left = st.back();  st.pop_back();
                auto right = st.back();  st.pop_back();
                st.push_back(std::max(left, right));
            } else if (ch == '?') {
                st.push_back(nums[question_index++]);
            }
        }
        return st.back();
    }
    std::mt19937 rng{std::random_device{}()};
    auto rand() -> std::string {
        if (rng() % 4 == 0)  return "?";
        if (rng() & 1)  return "min(?," + rand() + ")";
        else  return "max(" + rand() + ",?)";
    }
    auto solve() -> void {
        std::string s;
        std::cin >> s;
        auto count = std::count(s.begin(), s.end(), '?');  // 数问号
        std::vector<i32> nums_seq(count);  // 数字顺序
        std::iota(nums_seq.begin(), nums_seq.end(), 1);

        std::unordered_set<i32> possible;
        do {
            auto ans = calcWith(s, nums_seq);
            possible.insert(ans);
        } while (std::next_permutation(nums_seq.begin(), nums_seq.end()));

        std::cout << possible.size() << std::endl;
    }
}

int main() {
    std::freopen(FILENAME ".in", "r", stdin);
    std::freopen(FILENAME ".out", "w", stdout);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution::solve();
    return 0;
}
