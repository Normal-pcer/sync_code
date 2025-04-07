#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

#define FILENAME "T3"

/*
拆位考虑。
每一个限制可以变为 两个数相等或者不相等。

把 A1, ..., An, inv A1, ..., inv An 看作若干个节点。
如果两个数一定相同，可以合并两个块。
Ai != Aj 可以看作 Ai = invAj。
最后遍历所有的块。看情况分配是 1 还是 0。
显然，每一位的和最小，总和也会是最小的。
*/
namespace Solution_1003255212752722 {
    class DSU {
    public:
        i32 n;
        std::vector<i32> fa;
        std::vector<i32> size;

        DSU(i32 n): n(n), fa(n + 1), size(n + 1, 1) {
            std::iota(fa.begin(), fa.end(), 0);
        }
        auto find(i32 x) -> i32 {
            if (fa[x] == x) return x;
            else return fa[x] = find(fa[x]);
        }
        auto add(i32 x, i32 y) {
            auto a = find(x), b = find(y);
            if (a == b) return;
            if (size[a] < size[b]) {
                std::swap(x, y), std::swap(a, b);
            }
            size[a] += size[b], fa[b] = a;
        }
    };
    auto solve() -> void {
        i32 n, m;
        std::cin >> n >> m;

        struct Limit {
            i32 x, y, z;
        };
        std::vector<Limit> limits(m);
        for (auto &[x, y, z]: limits) {
            std::cin >> x >> y >> z;
        }

        struct BitLimit {
            i32 x, y;
            bool isEqual;
        };
        // 在一位上填充 0/1 返回最优解
        auto work = [&](std::vector<BitLimit> const &limits) -> std::vector<char> {
            // 一个数 x 本身对应节点 x，取反对应节点 x + n
            auto inv = lam(x, x + n);
            DSU dsu{n + n};
            for (auto [x, y, isEqual]: limits) {
                if (isEqual) {
                    dsu.add(x, y);
                    dsu.add(inv(x), inv(y));
                } else {
                    dsu.add(x, inv(y));
                    dsu.add(y, inv(x));
                }
            }

            // 判断无解
            for (i32 i = 1; i <= n; i++) {
                if (dsu.find(i) == dsu.find(inv(i))) {
                    return {};
                }
            }

            // return std::vector<char>(n + 1);
            
            std::vector<std::vector<i32>> blocks(n + n + 1);
            std::vector<i32> sizeOfBlock(n + n + 1);
            for (i32 i = 1; i <= n + n; i++) {
                sizeOfBlock[dsu.fa[i]]++;
            }
            for (i32 i = 1; i <= n + n; i++) {
                blocks[i].reserve(sizeOfBlock[i]);
            }
            for (i32 i = 1; i <= n + n; i++) {
                blocks[dsu.fa[i]].push_back(i);
            }

            
            std::vector<char> res(n + 1);
            std::vector<char> filled(n + 1);
            for (i32 i = 1; i <= n + n; i++) {
                if (blocks[i].empty()) continue;
                auto isInv = lam(x, x > n);  // 是否为反向点

                i32 cnt = 0, cntInv = 0;
                for (auto x: blocks[i]) {
                    if (isInv(x)) cntInv++;
                    else cnt++;
                }
                // 使得正向的数为 1，反向的数为 0
                bool flag = cntInv > cnt;
                for (auto x: blocks[i]) {
                    if (isInv(x)) {
                        if (filled[x - n]) {
                            // assert(blocks[i].size() == 1 or res[x - n] == not flag);
                            continue;
                        }
                        res[x - n] = not flag;
                        filled[x - n] = true;
                    } else {
                        if (filled[x]) {
                            // assert(blocks[i].size() == 1 or res[x] == flag);
                            continue;
                        }
                        res[x] = flag;
                        filled[x] = true;
                    }
                }
            }
            return res;
        };

        std::vector<i32> ans(n + 1);
        for (i32 i = 0; i < 32; i++) {
            std::vector<BitLimit> bitLimits;
            bitLimits.reserve(m);
            for (auto lim: limits) {
                if (lim.z & (1u << i)) {
                    bitLimits.push_back({lim.x, lim.y, false});
                } else {
                    bitLimits.push_back({lim.x, lim.y, true});
                }
            }

            auto bitRes = work(bitLimits);
            if (bitRes.empty()) {
                // 无解
                std::cout << -1 << endl;
                return;
            }
            for (i32 j = 1; j <= n; j++) {
                ans[j] += static_cast<i32>(bitRes[j]) << i;
            }
        }

        for (i32 i = 1; i <= n; i++) {
            std::cout << ans[i] << " ";
        }
        std::cout << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    // std::freopen(FILENAME ".in", "r", stdin);
    // std::freopen(FILENAME ".out", "w", stdout);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    auto t0 = std::chrono::system_clock::now();
    Solution_1003255212752722::solve();
    auto t1 = std::chrono::system_clock::now();
    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << std::endl;
    return 0;
}
