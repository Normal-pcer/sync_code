/**
 * @link https://www.luogu.com.cn/problem/P9016
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
用节点 t[i][ch] 表示第 i 次替换之后的字母 ch。
那么一次操作就可以转化为若干次连边，例如第一次操作是 把字母 b 替换为 "ac"，就可以转化为依次进行以下连边：
t[0][b] -> t[1][a]
        -> t[1][c]

再比如把字母 a 替换为 "aaab"：
t[0][a] -> t[1][a]
        -> t[1][a]
        -> t[1][a]
        -> t[1][b]

接下来，对于每一个节点，可以统计出来它的实际 size。
注意 size 可能会爆 double。size 足够大的时候就不用管它了。

接下来，统计答案。需要找到从左到右数第 l 个叶子节点并开始遍历。
这一步可以用类似线段树上二分的方式。

考虑做一个优化。绝大多数的节点都会只连向自己的下一个节点，于是可以直接用它来替换先前的节点。
把现在的直接算 index 改成类似离散化。

更进一步地，把整个过程倒过来做，这样只需要维护每个字母的最顶层节点。
*/
namespace Solution_5242390942628612 {
    auto stringReplace(std::string const &s, std::string const &from, std::string const &to) -> std::string {
        std::string result;
        std::boyer_moore_searcher searcher{from.begin(), from.end()};
        auto it = s.begin();
        while (it != s.end()) {
            auto found = std::search(it, s.end(), searcher);
            ranges::copy(it, found, std::back_inserter(result));
            it = found;
            if (found != s.end()) {
                ranges::copy(to, std::back_inserter(result));
                it += from.end() - from.begin();
            }
        }
        return result;
    }
    auto solveForce() -> void {
        i32 l, r, opCount;
        std::cin >> l >> r >> opCount;

        std::vector<std::pair<std::string, std::string>> ops(opCount);
        for (auto &[from, to] : ops) {
            std::cin >> from >> to;
        }

        std::string s("a");
        for (auto const &[from, to]: ops) {
            s = stringReplace(s, from, to);
        }
        std::cout << s.substr(l - 1, r - l + 1) << endl;
    }

    i32 constexpr charsetSize = 26;
    auto constexpr ord(char ch) -> i32 { return ch - 'a'; }
    auto constexpr chr(i32 index) -> char { return static_cast<char>(index + 'a'); }

    class Tree {
        struct Node {
            char ch;
            std::vector<i32> children{};
            i64 size = 0;  // 叶子节点数量
            i64 static constexpr maxSize = 0x3f3f3f3f3f3f3f3fLL;  // 大于这个 size 的将会被视为这个值
        };

        std::vector<Node> nodes;
        std::vector<i32> rootOfLetter;

        auto getSizeRecursion(i32 p, std::vector<char> &vis) -> void {
            if (vis[p]) return;
            vis[p] = true;
            // std::cerr << "getSizeRecursion " << p << std::endl;
            i64 size = 0;
            bool isLeaf = true;
            for (auto child: nodes[p].children) {
                isLeaf = false;
                getSizeRecursion(child, vis);
                size += nodes[child].size;
                if (size >= Node::maxSize) {
                    size = Node::maxSize;
                    break;
                }
            }
            nodes[p].size = size + isLeaf;
        }

        auto walkRecursion(i64 begin, i64 end, i32 p, i64 node_begin, i64 node_end, std::string &res) -> void {
            // std::cerr << "walk" << p << " " << node_begin << " " << node_end << std::endl;
            if (nodes[p].children.empty()) {
                // 叶子节点
                assert(begin <= node_begin and node_end <= end);
                res.push_back(nodes[p].ch);
                return;
            }

            i64 it = node_begin;
            for (auto child: nodes[p].children) {
                auto last = it + nodes[child].size;
                if (last >= Node::maxSize) last = Node::maxSize;
                if (last > begin and it < end) {
                    walkRecursion(begin, end, child, it, last, res);
                }
                if (last >= Node::maxSize) break;
                it = last;
            }
        }
    public:
        Tree(): rootOfLetter(charsetSize) {
            for (i32 chIndex = 0; chIndex < charsetSize; chIndex++) {
                rootOfLetter[chIndex] = static_cast<i32>(nodes.size());
                nodes.push_back({.ch = chr(chIndex)});
            }
        }

        // 执行替换逻辑
        auto add(char from, std::string const &to) -> void {
            // 只需要考虑新的 root[from]

            // 直接更换为对应的根
            if (to.size() == 1) {
                rootOfLetter[ord(from)] = rootOfLetter[ord(to.front())];
                return;
            }

            // 需要创建一个新的节点
            auto nodeId = static_cast<i32>(nodes.size());
            auto &node = nodes.emplace_back(from);
            node.children.reserve(to.size());

            for (auto ch: to) {
                node.children.push_back(rootOfLetter[ord(ch)]);
            }
            rootOfLetter[ord(from)] = nodeId;
        }

        // 为 (0, ch) 及其子树获取正确的 size
        auto getSize(char ch) -> void {
            std::vector<char> vis(nodes.size(), false);
            getSizeRecursion(rootOfLetter[ord(ch)], vis);
        }

        // 获取 ch 经过若干次替换后的 [begin, end) 区间
        auto walk(char ch, i64 begin, i64 end) -> std::string {
            std::string res;
            getSize(ch);
            auto root = rootOfLetter[ord(ch)];
            walkRecursion(begin, end, root, 0, nodes[root].size, res);
            return res;
        }
    };

    auto solve() -> void {
        i64 l, r;
        i32 opCount;
        std::cin >> l >> r >> opCount;

        Tree tr{};
        std::vector<std::pair<char, std::string>> ops(opCount);
        for (auto &[from, to]: ops) std::cin >> from >> to;

        for (auto const &[from, to]: ops | views::reverse) tr.add(from, to);

        std::cout << tr.walk('a', l - 1, r) << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_5242390942628612::solve();
    return 0;
}
