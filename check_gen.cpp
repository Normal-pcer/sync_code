// Do not expand include
#include <bits/stdc++.h>
namespace Generator {
    #include "lib"
    #include "libs/range.hpp"
    #include "libs/fixed_int.hpp"
    using namespace lib;
    namespace Random {
        std::mt19937 random(std::random_device{}());
        std::mt19937_64 random64(std::random_device{}());

        int randint(int x, int y) { return std::uniform_int_distribution<int>{x, y}(random); }
        long long randll(ll x, ll y) { return std::uniform_int_distribution<long long>{x, y}(random64); }
    }
    using namespace Random;

    void generate(std::ostream &out) {
        while (true) {
            static_cast<void>(out);
            std::system("test.exe");
            break;
        }
    }
}

int main() {
    Generator::generate(std::cout);
    return 0;
}