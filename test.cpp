/**
 * 
 */

#include "./lib"#include "libs/io.hpp"
using namespace lib;


namespace Solution_1469017748636242 {
    void solve() {
        std::vector<std::vector<int>> matrix(4, std::vector<int>(4));

        for (auto i: std::views::iota(0, 4)) {
            for (auto j: std::views::iota(0, 4)) {
                io >> matrix.at(i).at(j);
            }
        }

        
    }
}


int main() {
    initDebug;
    int T;  io >> T;
    while (T --> 0)  Solution_1469017748636242::solve();
    return 0;
}