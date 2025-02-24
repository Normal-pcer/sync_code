// Do not expand include
#include <bits/stdc++.h>
namespace Generator {
    #include "lib"
    #include "libs/range.hpp"
    using namespace lib;
    namespace Random {
        std::mt19937 random(std::random_device{}());
        std::mt19937_64 random64(std::random_device{}());

        int randint(int x, int y) { return random() % (y - x + 1) + x; }
        int rand_max(int y) { return randint(1, y); }
        long long randll(ll x, ll y) { return random64() % (y - x + 1) + x; }
    }
    using namespace Random;

    void generate(std::ostream &out) {
        int const _N = 10, _M = 10;
        
        while (true) {
            std::string s;
            int s_size = randint(1, _N);
            for (auto _: range(s_size))  s.push_back(randint('a', 'd'));

            std::string t = s;
            auto modify = randint(1, _M);
            for (auto _: range(modify)) {
                if (t.empty())  t = 'c';
                auto op = randint(1, 3);
                auto pos = randint(0, t.size() - 1);
                if (op == 1) {
                    t.insert(t.begin() + pos, randint('a', 'd'));
                } else if (op == 2) {
                    t.erase(t.begin() + pos);
                } else {
                    t.at(pos) = randint('a', 'd');
                }
            }

            if (s.empty() or t.empty())  continue;
            auto m = randint(1, _M);
            out << m << endl;
            out << s << endl;
            out << t << endl;
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}