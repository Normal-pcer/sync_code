/**
 * @link https://www.luogu.com.cn/problem/P4838
 */
#include "libs/debug_macros.hpp"
#include "lib"
#include "libs/fixed_int.hpp"
using namespace lib;

/**
 * 状压记录最后的三个数
 * 
 */
namespace Solution_3005915539153714 {
    i32 constexpr mod = 19260817;
    class Matrix {
        i32 height, width;
        std::vector<std::vector<i32>> data;
    public:
        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<i32>(width)) {}

        auto operator* (Matrix const &other) const -> Matrix {
            assert(width == other.height);
            Matrix res{height, other.width};

            
        }
    };
    void solve() {

    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_3005915539153714::solve();
    return 0;
} 
