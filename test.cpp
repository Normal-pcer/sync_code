/**
 * @link http://localhost/
 */

#include "./lib"

#include "./libs/io.hpp"

using namespace lib;

namespace Solution_5736322220169565 {

    struct Node {
        int pos, val;

        bool operator< (const Node &other) const noexcept {
            if (pos == other.pos)  return val < other.val;
            else  return pos < other.pos;
        }
    };

    std::vector<Node> a;
    std::vector<int> F;

    void cdq(int begin, int end) {
        if (begin + 1 == end)  return void(chkMax(F.at(begin), 1));
        auto mid = (begin + end) >> 1;
        cdq(begin, mid);
        
        auto with_val = lam(x, y, x.val < y.val);
        std::sort(a.begin() + begin, a.begin() + mid, with_val);
        std::sort(a.begin() + mid, a.begin() + end, with_val);

        auto i = begin, j = mid;
        
        auto max = 0;  // 左侧 F 的最大值
        while (j != end) {
            while (i != mid and a.at(i).val <= a.at(j).val)  chkMax(max, F.at(a.at(i++).pos));
            if (i != begin)  i--;
            chkMax(F.at(a.at(j).pos), max + 1);
            j++;
        }
        std::sort(a.begin() + mid, a.begin() + end);
        cdq(mid, end);
    }

    void solve() {
        int N;  io >> N;
        N++;  a.resize(N), F.resize(N);

        for (auto i = 0; i < N - 1; i++) {
            a.at(i).pos = i;
            io >> a.at(i).val;
        }

        a.back() = {N-1, inf};

        cdq(0, N);

        io << F.at(N-1) - 1 << endl;
    }
}

int main() {
    initDebug;
    Solution_5736322220169565::solve();
    return 0;
}
