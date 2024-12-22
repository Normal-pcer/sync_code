#include <bits/stdc++.h>

namespace Solution {
    class BIT {
        std::vector<int> c;
        static int lowbit(int x) { return x & -x; }
    public:
        BIT(int N): c(N+1) {}
        int sum(int x) {
            x++;  auto res = 0;
            while (x) {
                res += c.at(x);
                x -= lowbit(x);
            }
            return res;
        }
        void add(int x, int val) {
            x++;
            while (x < (int)c.size()) {
                c.at(x) += val;
                x += lowbit(x);
            }
        }
    };
}

int main() {

}