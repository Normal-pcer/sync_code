/**
 * 
 */

#include "./lib"
#include "libs/range.hpp"
#include "libs/io.hpp"
using namespace lib;

namespace Solution_3507163760215941 {

    std::vector<std::vector<int>> matrix;

    void solve() {
        int H, W, N;  io >> H >> W >> N;
        matrix.resize(H, std::vector<int>(W));

        for (auto &row: matrix) {
            for (auto &i: row) {
                io >> i;
            }
        }

        
    }
}

int main() {
    initDebug;
    Solution_3507163760215941::solve();
    return 0;
}