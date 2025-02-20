#include <bits/stdc++.h>
#include "./libs/io.hpp"


using namespace lib;
int main() {
    i32 N, s = 0, x;  io >> N;
    for (; N--; )  io >> x, s += x;
    io << s << '\n';
    return 0;
}