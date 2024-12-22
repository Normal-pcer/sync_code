/**
 * @link https://www.luogu.com.cn/problem/P7113
 */

#include <bits/stdc++.h>
using i128 = __int128;
const char endl = '\n';

namespace Solution_1531095818932130 {
    auto &operator<< (std::ostream &stream, i128 x) {
        static char sta[40]; int top=0;
        if (x<0) {
            stream.put('-'), sta[top++] = (-(x%10))^48, x = -(x/10);
            if (x==0) return stream.put(sta[--top]);
        }
        do {  sta[top++] = x%10^48, x /= 10;  } while (x);
        while (top) stream.put(sta[--top]);
        return stream;
    }
    struct Frac {
        i128 numerator;
        i128 denominator = 1;

        static constexpr Frac simplified(const Frac& origin) {
            auto gcd = std::__gcd(origin.numerator, origin.denominator);
            if (gcd == 1)  return origin;
            return {origin.numerator / gcd, origin.denominator / gcd};
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
        friend auto &operator<< (std::ostream &stream, Frac x)  {
            return stream << x.numerator << ' ' << x.denominator;
        }
    };

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        int N, M;
        std::cin >> N >> M;

        std::vector<std::vector<int>> graph(N+1);
        std::vector<int> in(N+1), out(N+1);
        std::vector<Frac> F(N+1);
        for (auto i: std::views::iota(1, N+1)) {
            int cnt;  std::cin >> cnt;
            for (auto _: std::views::iota(0, cnt)) {
                int target;  std::cin >> target;
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

        for (auto i: std::views::iota(1, N+1)) {
            if (out.at(i) == 0) {
                auto res = F.at(i);
                std::cout << res << endl;
            }
        }
    }
}

int main() {
    Solution_1531095818932130::solve();
    return 0;
}