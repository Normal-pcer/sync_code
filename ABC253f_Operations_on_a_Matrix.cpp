/**
 * @link https://www.luogu.com.cn/problem/AT_abc253_f
 */

#include "./libs/debug_macros.hpp"


#include "./lib_v3.hpp"


#include "./libs/range.hpp"


using namespace lib;

namespace Solution_2317517576465138 {
    class BIT {
        std::vector<ll> c;
        static constexpr int lowbit(int x) { return x & -x; }
    public:
        BIT(int N): c(N+1) {}
        ll sumPrefix(int x) const {
            x++;  auto res = 0LL;
            while (x) {
                res += c.at(x);
                x -= lowbit(x);
            }
            return res;
        }
        void addAt(int x, int val) {
            x++;
            while (x < static_cast<int>(c.size())) {
                c.at(x) += val;
                x += lowbit(x);
            }
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M, Q;  std::cin >> N >> M >> Q;
        struct Assign {
            int time = -1, val;
        };

        std::vector<Assign> assigns(N+1);  // 第 i 行上的所有赋值
        struct Query {
            int index, t, x, val;
            bool nega;
        };
        std::vector<Query> queries;
        struct Update {
            int t, pos, val;
        };
        std::vector<Update> updates;
        auto max_query_index = 0;
        for (auto t: range(Q)) {
            int op, i, j, z;  std::cin >> op >> i >> j;
            if (op == 1) {
                std::cin >> z;
                updates.push_back({t, i, +z});
                updates.push_back({t, j+1, -z});
            } else if (op == 2) {
                assigns.at(i) = {t, j};  // 新赋值
            } else if (op == 3) {
                auto [t0, val] = assigns.at(i);
                // 二维数点
                // t0 < t < t_cur，x <= j 的差可以统计
                queries.push_back({max_query_index, t, j, val, false});
                queries.push_back({max_query_index, t0, j, val, true});
                max_query_index++;
            }
        }

        auto q_index = 0, u_index = 0;
        BIT bit(M+2);  // 列上的差值，diff[x] = a[x] - a[x-1]
        std::vector<ll> ans(max_query_index);
        ranges::sort(updates, ranges::less{}, lam(x, x.t));  // 递增
        ranges::sort(queries, ranges::less{}, lam(x, x.t));  // 递增
        for (; q_index != static_cast<int>(queries.size()); q_index++) {
            auto q = queries.at(q_index);
            // 对于每一次询问，插入 t < max 的更新
            while (u_index != static_cast<int>(updates.size()) and updates.at(u_index).t < q.t) {
                auto [t, pos, val] = updates.at(u_index);
                bit.addAt(pos, val);
                u_index++;
            }
            // 查询，要求 x <= q_x
            auto res = bit.sumPrefix(q.x);
            if (q.nega) {
                ans.at(q.index) -= res;
            } else {
                // 加上赋值带来的额外值
                ans.at(q.index) += res + q.val;
            }
        }

        for (auto x: ans)  std::cout << x << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_2317517576465138::solve();
    return 0;
}
