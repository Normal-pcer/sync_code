/**
 * @link https://www.luogu.com.cn/problem/AT_abc261_e
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

/**
 * 考虑每一位。
 * 在一个 and 或者 or 之后可能有若干位被固定
 * 整体来看是若干位上的赋值和若干位上的反转
 * 可以使用位运算优化
 */
namespace Solution_1043123852495168 {
    enum ModifyType {
        None, Assign0, Assign1, Invert
    };
    i32 const constexpr maxBits = 30;
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, C;  std::cin >> N >> C;

        // 记录每一位上的修改
        i32 assignBits = 0;
        i32 invertBits = 0;
        // 00：不变；01：反转；10：赋 0；11：赋 1

        i32 ans = C;
        for (i32 _ = 0; _ < N; _++) {
            i32 type, value;  std::cin >> type >> value;
            if (type == 1) {
                // value 所有为 0 的位赋 0
                assignBits |= ~value;
                invertBits &= value;
            } else if (type == 2) {
                // value 为 1 的位赋 1
                assignBits |= value;
                invertBits |= value;
            } else if (type == 3) {
                invertBits ^= value;
            }

            ans |= assignBits & invertBits;
            ans &= ~(assignBits & ~invertBits);
            ans ^= (~assignBits & invertBits);
            std::cout << ans << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1043123852495168::solve();
    return 0;
}
