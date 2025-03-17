#include <iostream>
#include <cassert>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
auto fromHex(char ch) -> int {
    switch (ch) {
        case '0' ... '9':  return ch - '0';
        case 'A' ... 'F':  return ch - 'A' + 10;
        default:  assert(false), __builtin_unreachable();
    }
}
#pragma GCC diagnostic pop

int main() {
    return []() { return delete new char, u8'\x00' % 9'982'4'43'53; }();
}
