/**
 * @link https://www.luogu.com.cn/problem/AT_abc268_g
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

using namespace lib;

/**
 * 对于两个字符串：
 * ABCDEF
 * ABCDFE
 *     ^ 它们的第一个不匹配的位置决定相对排名
 * 显然，对于完全随机的字母表，分别有一半的概率靠前
 * 否则，如果 S 是 T 的前缀或者反过来，前者靠前的概率为 1 或 0。
 * 
 * 假设 S 是 a 个串的前缀，b 个串是 S 的前缀
 * 它比另一个串靠前的概率是多少？
 * p = a / N * 100% + (N - a - b) / N * 50%
 * 对于每一个串，超过它的概率是 p，所以期望超过 Np 个串。
 */
namespace Solution_1425418012431424 {
    constexpr const int mod = 998244353;
    constexpr auto qpow(ll a, ll b, const int mod) -> int {
        auto res = 1LL;
        for (; b; b >>= 1, a = a * a % mod)  if (b & 1) res = res * a % mod;
        return res;
    }
    class Trie {
        struct Node {
            std::array<Node *, 26> next;
            int count;
            bool end;  // 有字符串在此结束
        };
        std::deque<Node> nodes;
        Node *root = nullptr;

    public:
        Trie(): nodes({Node{}}), root(&nodes.back()) {}

        auto insert(std::string const &s) -> void {
            auto *cur = root;
            cur->count++;
            for (auto ch: s) {
                if (cur->next[ch - 'a'] == nullptr) {
                    nodes.push_back({});
                    cur->next[ch - 'a'] = &nodes.back();
                }
                cur = cur->next[ch - 'a'];
                cur->count++;
            }
            cur->end = true;
        }
        auto count(std::string const &s) -> std::pair<int, int> {
            // 有多少个串以 s 为前缀，有多少个串是 s 的前缀
            auto *cur = root;
            auto res1 = 0;
            for (auto ch: s) {
                res1 += cur->end;
                cur = cur->next[ch - 'a'];
                if (cur == nullptr)  return {0, res1};
            }
            return {cur->count, res1};
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<std::string> s(N);
        Trie trie;
        for (auto &x: s)  std::cin >> x, trie.insert(x);

        for (auto i = 0; i < N; i++) {
            auto [less, greater] = trie.count(s[i]);
            auto other = N - greater - less;

            auto constexpr inv2 = qpow(2, mod - 2, mod);
            
            std::cout << (1 + greater + static_cast<ll>(other) * inv2 % mod) % mod << std::endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1425418012431424::solve();
    return 0;
}
