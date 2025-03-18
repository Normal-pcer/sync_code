/**
 * 
 */
#include "lib"
using namespace lib;

namespace Solution_/*hashName*/ {
    void solve() {

    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_/*hashName*/::solve();
    return 0;
}
