/**
 * @link https://www.luogu.com.cn/problem/P9127
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_5100562260362022 {
    auto solve() -> void {
        i32 n;
        std::cin >> n;

        std::vector<i64> a(n);
        for (auto &x: a) std::cin >> x;

        struct Segment {
            i32 begin, end;
            i64 sum;
        };

        std::vector<Segment> segs;
        segs.reserve(n * (n + 1) / 2);
        for (i32 begin = 0; begin < n; begin++) {
            i64 sum = 0;
            for (i32 end = begin + 1; end <= n; end++) {
                sum += a[end - 1];
                segs.push_back({begin, end, sum});
            }
        }
        
        ranges::sort(segs, std::less{}, lam(x, x.sum));
        for (i32 p = 0; p < n; p++) {
            auto contained = [&](Segment x) -> bool { return x.begin <= p and p < x.end; };

            auto prevHas = segs.end();
            auto prevNone = segs.end();
            auto ans = std::numeric_limits<i64>::max();
            for (auto it = segs.begin(); it != segs.end(); ++it) {
                auto seg = *it;

                if (contained(seg)) {
                    if (prevNone != segs.end()) {
                        auto prev = *prevNone;
                        chkMin(ans, std::abs(seg.sum - prev.sum));
                    }
                    prevHas = it;
                } else {
                    if (prevHas != segs.end()) {
                        auto prev = *prevHas;
                        chkMin(ans, std::abs(seg.sum - prev.sum));
                    }
                    prevNone = it;
                }
            }

            std::cout << ans << endl;
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_5100562260362022::solve();
    return 0;
}
