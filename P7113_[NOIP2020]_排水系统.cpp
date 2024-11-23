/**
 * @link https://www.luogu.com.cn/problem/P7113
 */

#include "./lib_legecy.hpp"
#include "./libs/io.hpp"
using i128 = __int128;
using namespace lib;

namespace Solution_1531095818932130 {

    struct Frac {
        i128 numerator;
        i128 denominator = 1;

        static constexpr Frac simplified(const Frac& origin) {
            auto gcd = std::__gcd(origin.numerator, origin.denominator);
            if (gcd == 1)  return origin;
            return {origin.numerator / gcd, origin.denominator / gcd};
        }

        void simplify() {
            *this = simplified(*this);
        }

        Frac operator+ (const Frac& other) const {
            return simplified({
                numerator * other.denominator + other.numerator * denominator, 
                denominator * other.denominator
            });
        }
        void operator+= (const Frac& other) { *this = *this + other; }

        Frac operator/ (const int other) const {
            return simplified({numerator, other * denominator});
        }
    };

    void solve() {
        int N, M;
        io >> N >> M;

        std::vector<std::vector<int>> graph(N+1);
        std::vector<int> in(N+1), out(N+1);
        std::vector<Frac> F(N+1);
        for (auto i: std::views::iota(1, N+1)) {
            auto cnt = io.get();
            for (auto _: std::views::iota(0, cnt)) {
                auto target = io.get();
                graph.at(i).push_back(target);
                in.at(target)++, out.at(i)++;
            }
        }

        std::deque<int> q;
        for (auto i: std::views::iota(1, N+1)) {
            if (in.at(i) == 0)  q.push_back(i), F.at(i) = {1, 1};
        }

        while (not q.empty()) {
            auto x = q.front();  q.pop_front();
            auto unit = F.at(x) / out.at(x);

            for (auto dest: graph.at(x)) {
                F.at(dest) += unit;
                if (--in.at(dest) == 0)  q.push_back(dest);
            }
        }

        for (auto i: std::views::iota(1, N+1) | std::views::filter(lam(i, out.at(i)==0))) {
            auto &&res = F.at(i);
            io << res.numerator << ' ' << res.denominator << endl;
        }
    }
}

int main() {
    initDebug;
    Solution_1531095818932130::solve();
    return 0;
}