/**
 * @link
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

#define FILENAME "fishing"

/*
首先有一个结论：折返一定不会更优。

F[i][j] 表示：从湖 i 开始钓鱼，经过 j 个单位时间的最优解（鱼的数量）。

转移：F[i][j] <- F[i + 1][j - t] + cur_fish
需要记录具体方案。
*/
namespace Solution_6138187558929015 {
    auto solve() -> void {
        using Clock = i32;
        i32 n;
        Clock timeLimit;
        std::cin >> n;
        if (n == 0) std::exit(0);
        std::cin >> timeLimit;

        timeLimit *= 12;  // 小时 -> 单位时间

        struct Lake {
            i32 init, decrease;

            Lake(): init(0), decrease(0) {}
            Lake(i32 init, i32 decrease): init(init), decrease(decrease) {}
        };
        std::vector<Lake> lakes(n);

        for (auto l: lakes) std::cin >> l.init;
        for (auto l: lakes) std::cin >> l.decrease;

        std::vector<Clock> dis(n - 1);
        for (auto &x: dis) std::cin >> x;
        dis.push_back(0);

        struct Item {
            i32 value = 0;
            std::pair<i32, i32> source{};

            Item(): value(0), source({}) {}
            Item(i32 value, std::pair<i32, i32> const &source): 
                value(value), source(source) {}

            auto operator< (Item const &other) const -> bool {
                return value < other.value;
            }
        };
        std::vector<std::vector<Item>> F(n + 1, std::vector<Item>(timeLimit + 1));
        for (i32 i = n; i --> 0; ) {
            for (i32 j = 0; j <= timeLimit; j++) {
                // 对应的 Item 以及在此处停留的时间
                std::pair<Item, i32> best{};

                i32 fishCount = lakes[i].init;
                i32 gotFish = 0;
                for (i32 k = 0; k <= j; k++) {
                    auto timeRemains = j - k;
                    if (timeRemains < 0) break;
                    auto newItem = [&]() -> Item {
                        if (timeRemains >= dis[i]) {
                            return Item(
                                F[i + 1][timeRemains - dis[i]].value + gotFish, 
                                std::make_pair(i + 1, timeRemains - dis[i])
                            );
                        } else {
                            return Item(gotFish, std::make_pair(n, 0));
                        }
                    }();
                    chkMax(best, std::make_pair(newItem, k));
                    gotFish += fishCount;
                    fishCount = std::max(0, fishCount - lakes[i].decrease);
                }

                F[i][j] = best.first;
            }
        }

        auto ans = F[0][timeLimit];

        auto cur = std::make_pair(0, timeLimit);
        std::vector<i32> timeOf(n);
        while (true) {
            auto val = F[cur.first][cur.second];
            if (val.source.first == n) {
                auto time = cur.second;
                timeOf[cur.first] = time;
                break;
            } else {
                auto time = cur.second - val.source.second - dis[cur.first];
                timeOf[cur.first] = time;
                cur = val.source;
            }
        }

        {
            auto first = true;
            for (auto x: timeOf) {
                if (not first) std::cout << ", ";
                std::cout << x * 5;
                first = false;
            }
        }
        std::cout << endl;
        std::cout << "Number of fish expected: " << ans.value << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    // std::freopen(FILENAME ".in", "r", stdin);
    // std::freopen(FILENAME ".out", "w", stdout);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    while (true) {
        Solution_6138187558929015::solve();
    }
    return 0;
}
