/**
 * @link https://www.luogu.com.cn/problem/P7210
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

/**
 * 如果 str + ch 属于 P 且不属于 Q，选择 str 即可获胜。
 * F[x] 表示选择到某个字典树节点的情况下，Nina 是否可以获胜。
 * 如果选择接下来的可选节点中有一个可以获胜，这个节点即可获胜。
 * 可以通过当前的深度判断出手玩家。
 */
namespace Solution_2987276431356100 {
    class Trie {
        struct Node {
            std::array<bool, 2> owned;
            std::array<int, 26> next;
        };

        std::vector<Node> nodes{{}};
        std::vector<char> F;  // 面对当前节点的字符串，Nina 是否可以获胜
        static auto ord(char ch) -> int { return ch - 'a'; }
        static auto chr(int x) -> char { return x + 'a'; }
    public:
        /**
         * 查找表示一个字符串的节点的下标
         * 如果找不到或给定了空串，返回 0
         */
        auto find(std::string const &s) -> int {
            auto p = 0;
            for (auto it = s.begin(); it != s.end(); it++) {
                auto ch = *it;
                if (p = nodes[p].next[ord(ch)]; p == 0) {
                    return 0;  // 没有这个节点
                }
            }
            return p;
        }
        auto insert(std::string const &s, bool tag) -> void {
            auto p = 0;
            for (auto it = s.begin(); it != s.end(); it++) {
                auto ch = *it;
                auto &next = nodes[p].next[ord(ch)];
                if (next == 0)  next = nodes.size(), nodes.emplace_back();
                p = nodes[p].next[ord(ch)];  // 可能会引用失效，重新取一下
                nodes[p].owned[tag] = true;
            }
        }
        auto dfs(int p, bool cur) -> void {
            debug  std::cout << std::format("dfs {} {}", p, cur) << std::endl;
            auto &node = nodes[p];
            // 如果在当前节点即可决定胜负
            {
                auto flag = [&]() -> bool {
                    for (auto i = 0; i < 26; i++) {
                        if (node.next[i] != 0 and nodes[node.next[i]].owned[cur]) {
                            if (not nodes[node.next[i]].owned[not cur]) {
                                return true;  // 只需选择字母 chr(i) 即可获胜
                            }
                        }
                    }
                    return false;
                }();
                if (flag)  return F[p] = true ^ cur, void();
            }

            // 如果接下来这一方可以用任意方法获胜
            for (auto i = 0; i < 26; i++) {
                if (node.next[i] == 0)  continue;
                dfs(node.next[i], not cur);
                if (F[node.next[i]] ^ cur) {
                    F[p] = true ^ cur;
                    return;
                }
            }

            // 否则，当前方必败
            F[p] = false ^ cur;
        }
        auto query() -> bool {
            F.resize(nodes.size());
            dfs(0, 0);
            debug {
                for (auto &node: nodes) {
                    std::cout << "node ";
                    std::cout << static_cast<int>(node.owned[0]) << " ";
                    std::cout << static_cast<int>(node.owned[1]) << " ";
                    for (auto i = 0; i < 26; i++) {
                        if (node.next[i] != 0) {
                            std::cout << std::format("[{}({}): {}] ", i, chr(i), node.next[i]);
                        }
                    }
                    std::cout << std::endl;
                }
                std::cout << "F: " << std::endl;
                for (auto x: F)  std::cout << static_cast<int>(x) << " ";
                std::cout << std::endl;
            }
            return F[0];
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        std::array<std::vector<std::string>, 2> words;
        Trie tr;
        int N;  std::cin >> N;
        for (words[0].resize(N); auto &s: words[0])  std::cin >> s, tr.insert(s, 0);
        int M;  std::cin >> M;
        for (words[1].resize(M); auto &s: words[1])  std::cin >> s, tr.insert(s, 1);

        auto ans = tr.query();
        std::cout << (ans? "Nina": "Emilija") << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_2987276431356100::solve();
    return 0;
}
