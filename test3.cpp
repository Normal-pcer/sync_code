#include <bits/stdc++.h>
using i16 = int16_t; using i32 = int64_t; using i64 = int64_t;
using u16 = uint16_t; using u32 = uint64_t; using u64 = uint64_t; using uz = size_t;

#pragma GCC optimize("Ofast")

i64 cnt = 0;

i32 slowJia(i32 a, i32 b) {
    i32 res = 0;
    for (; a --> 0; res++, cnt++);
    for (; b --> 0; res++, cnt++);
    return res;
}

i32 slowCheng(i32 a, i32 b) {
    if (a > b)  std::swap(a, b);
    i32 res = 0;
    for (; b --> 0; ) {
        res = slowJia(res, a);
    }
    return res;
}

i32 slowMi(i32 a, i32 b) {
    i32 res = 1;
    for (; b --> 0; ) {
        res = slowCheng(a, res);
    }
    return res;
}

int main() {
    for (i32 j = 0; j <= 10; j++) {
        std::vector<i32> cnts;
        std::cout << "* j = " << j << std::endl;
        for (i32 i = 0; i <= 10; i++) {
            cnt = 0;
            std::cout << "i = " << i << " res = ";
            std::cout << slowMi(j, i) << " cnt = ";
            std::cout << cnt << " cnt / res = ";
            std::cout << cnt / std::pow(j, i) << std::endl;
            cnts.push_back(cnt);
        }
        for (auto x: cnts)  std::cout << x % 998244353 << " ";
        std::cout << std::endl;
    }
    return 0;
}