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
        std::vector<std::pair<int, int>> queries(N);
        for (auto &[x, y]: queries)  std::cin >> x >> y;

        // 记录每一位上的修改
        std::array<ModifyType, maxBits> bits;
        bits.fill(None);

        i32 ans = C;
        for (auto [type, value]: queries) {
            if (type == 1) {
                // and
                // & 0 是无条件赋值为 0，& 1 不变
                for (i32 i = 0; i < maxBits; i++) {
                    if ((value & (1U << i)) == 0)  bits[i] = Assign0;
                }
            } else if (type == 2) {
                // or
                // | 1 为无条件赋值为 1
                for (i32 i = 0; i < maxBits; i++) {
                    if (value & (1U << i))  bits[i] = Assign1;
                }
            } else if (type == 3) {
                // xor
                for (i32 i = 0; i < maxBits; i++) {
                    if (value & (1U << i)) {
                        bits[i] = [&]() -> ModifyType {
                            switch (bits[i]) {
                            case Assign0:  return Assign1;
                            case Assign1:  return Assign0;
                            case Invert:  return None;
                            case None:  return Invert;
                            default:  assert(false), __builtin_unreachable();
                            }
                        }();
                    }
                }
            }

            for (i32 i = 0; i < maxBits; i++) {
                if (bits[i] == Assign0)  ans &= ~(1U << i);
                else if (bits[i] == Assign1)  ans |= (1U << i);
                else if (bits[i] == Invert)  ans ^= (1U << i);
            }
            
            debug {
                for (i32 i = 0; i < maxBits; i++)  std::cout << static_cast<int>(bits[i]) << " ";
                std::cout << endl;
            }
            std::cout << ans << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1043123852495168::solve();
    return 0;
}
