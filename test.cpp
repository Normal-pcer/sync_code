#include <bits/stdc++.h>

#undef assert  // 禁用 cassert 的 assert 宏
void make_assert_message(char const *expr, char const *file, unsigned int row, char const *func) {
    std::fprintf(stderr, "Assertion failed: `%s`, in file %s line %u, function %s\n", expr, file, row, func);
    std::fflush(stderr);
    std::exit(row % 255 + 1);  // 确保返回值非 0
}
#define assert(expr) static_cast<void>(!!(expr) || (make_assert_message(#expr, __FILE__, __LINE__, __PRETTY_FUNCTION__), false))

int main() {
    assert(false);
    return 0;
}