/**
 * 
 */

#include "./lib_v2.hpp"

#include "./libs/range.hpp"


#include "./libs/io.hpp"


using namespace lib;

namespace Solution_9469401034244231 {

    struct Node {
        int x, y, val1 = 0, val0 = 0;
    };

    void solve() {
        int N;  io >> N;
        std::string s[2], t[2];
        io >> s[0] >> s[1];
        io >> t[0] >> t[1];

        std::vector<std::vector<int>> string(2, std::vector<int>(N+1));
        std::vector<std::vector<int>> mask(2, std::vector<int>(N+1));

        std::transform(s[0].begin(), s[0].end(), string[0].begin(), lam(x, x=='1'));
        std::transform(t[0].begin(), t[0].end(), mask[0].begin(), lam(x, x=='1'));
        std::transform(s[1].begin(), s[1].end(), string[1].begin(), lam(x, x=='1'));
        std::transform(t[1].begin(), t[1].end(), mask[1].begin(), lam(x, x=='1'));

        std::vector<std::vector<Node>> rg(2);  // 可以交换的段；左闭右开
        int pt0 = -1, pt1 = -1;
        for (auto i: range(N+1)) {
            if (not mask[0].at(i)) {
                pt0+1 == i? void(): rg[0].push_back({pt0+1, i}), pt0 = i;
                rg[0].push_back({pt0, pt0+1});
            }
            if (not mask[1].at(i)) {
                pt1+1 == i? void(): rg[1].push_back({pt1+1, i}), pt1 = i;
                rg[1].push_back({pt1, pt1+1});
            }
        }

        std::vector count(2, std::vector<int>(N+2));  // 1 的数量
        std::partial_sum(string[0].begin(), string[0].end(), count[0].begin()+1);
        std::partial_sum(string[1].begin(), string[1].end(), count[1].begin()+1);

        for (auto i: range(2)) {
            for (auto &[x, y, val1, val0]: rg[i]) {
                val1 = count[i].at(y) - count[i].at(x);
                val0 = y - x - val1;
                debug io << std::format("{} {} {} {}", x, y, val1, val0) << endl;
            }
        }

        pt0 = 0;
        auto ans = 0;
        for (auto i: range(rg[1].size())) {
            while (rg[0][pt0].y <= rg[1][i].x) {
                pt0++;
            }
            if (rg[0][pt0].x >= rg[1][i].y)  continue;
            for (auto k = pt0; k < (int)rg[0].size() and rg[0][k].x < rg[1][i].y; k++) {
                auto max = std::min(rg[0][k].y, rg[1][i].y) - std::max(rg[1][i].x, rg[0][k].x);
                debug io << std::format("rg[{}][{}]({} {} {} {}) with rg[{}][{}]({} {} {} {})", 0, k, rg[0][k].x, rg[0][k].y, rg[0][k].val1, rg[0][k].val0, 1, i, rg[1][i].x, rg[1][i].y, rg[1][i].val1, rg[1][i].val0) << endl;
                if (rg[1][i].val1 > rg[0][k].val1)  ans += std::min(max, rg[0][k].val1), rg[1][i].val1 -= std::min(max, rg[0][k].val1), rg[0][k].val1 -= std::min(max, rg[0][k].val1), max -= std::min(max, rg[0][k].val1);
                else  ans += std::min(max, rg[1][i].val1), rg[0][k].val1 -= std::min(max, rg[1][i].val1), rg[1][i].val1 -= std::min(max, rg[1][i].val1), max -= std::min(max, rg[1][i].val1);
                if (rg[1][i].val0 > rg[0][k].val0)  ans += std::min(max, rg[0][k].val0), rg[1][i].val0 -= std::min(max, rg[0][k].val0), rg[0][k].val0 -= std::min(max, rg[0][k].val0);
                else  ans += std::min(max, rg[1][i].val0), rg[0][k].val0 -= std::min(max, rg[1][i].val0), rg[1][i].val0 -= std::min(max, rg[1][i].val0);

                // if (/*rg[0][k].x >= rg[1][i].x and */rg[0][k].y <= rg[1][i].y) {
                //     if (rg[0][k].val0)  rg[1][i].val1 -= rg[0][k].val0, rg[0][k].val0 = 0/*, chkMax(rg[1][i].val1, 0)*/;
                //     if (rg[0][k].val1)  rg[1][i].val0 -= rg[0][k].val1, rg[0][k].val1 = 0/*, chkMax(rg[1][i].val0, 0)*/;
                //     if (not (rg[1][i].val0 >= 0 and rg[1][i].val1 >= 0)) {
                //         rg[0][k].val0 -= rg[1][i].val1, rg[0][k].val1 -= rg[1][i].val0;
                //         chkMax(rg[1][i].val1, 0), chkMax(rg[1][i].val0, 0);
                //     }
                // }
            }
            debug io << ans << endl;
        }

        io << ans-1 << endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    int T;  io >> T;
    while (T --> 0)  Solution_9469401034244231::solve();
    return 0;
}