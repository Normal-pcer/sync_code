#include <bits/stdc++.h>

constexpr auto ctz(auto x) {
    return x? __builtin_ctz(x): sizeof(x) << 3;
}
struct EOFError {};
template <size_t MaxSize> requires ((MaxSize & 7) == 0)
struct FileReadScanner {
    char buf[MaxSize+1], *p1, *p2;
    bool eofFlag = false;
    FileReadScanner(): p1(buf), p2(buf) {}

    void flush() {
        if (eofFlag)  throw EOFError{};
        p1 = buf;
        p2 = buf + std::fread(buf, sizeof(char), sizeof(char) * MaxSize, stdin);
        if (std::feof(stdin)) eofFlag = true;
    }
    char gc_fread() {
        if (p1 == p2) {
            flush();
        }
        return p1 == p2? '\0': *p1++;
    }
    char gc() { return gc_fread(); }
    template <typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
    void test(T &x) {
        x = 0;  bool sign = false;
        if (p1 == p2)  flush(); 
        while (*p1 < 48) {
            if (*p1++ == '-')  sign = true;
            if (p1 == p2)  flush();
        }
        if (sign) {
            while (*reinterpret_cast<const size_t *>(&p1) & 3) {
                x = x * 10 - (*p1++ ^ 48);
                if (p1 == p2)  flush();
                if (*p1 < 48) {
                    return;
                }
            }
            while (true) {
                if (p1 == p2)  flush();
                auto numberDigits = ctz((0x30303030 & ~*reinterpret_cast<const uint32_t *>(p1))) >> 3;
                switch (numberDigits) {
                case 4:  (x = x * 10 - (*p1++ ^ 48)); [[fallthrough]];
                case 3:  (x = x * 10 - (*p1++ ^ 48)); [[fallthrough]];
                case 2:  (x = x * 10 - (*p1++ ^ 48)); [[fallthrough]];
                case 1:  (x = x * 10 - (*p1++ ^ 48));
                };
                if (p1 == p2)  flush();
                if (*p1 < 48) {
                    return;
                }
            }
        } else {
            while (*reinterpret_cast<const size_t *>(&p1) & 3) {
                x = x * 10 + (*p1++ ^ 48);
                if (p1 == p2)  flush();
                if (*p1 < 48) {
                    return;
                }
            }
            while (true) {
                if (p1 == p2)  flush();
                auto numberDigits = ctz((0x30303030 & ~*reinterpret_cast<const uint32_t *>(p1))) >> 3;
                switch (numberDigits) {
                case 4:  (x = x * 10 + (*p1++ ^ 48)); [[fallthrough]];
                case 3:  (x = x * 10 + (*p1++ ^ 48)); [[fallthrough]];
                case 2:  (x = x * 10 + (*p1++ ^ 48)); [[fallthrough]];
                case 1:  (x = x * 10 + (*p1++ ^ 48));
                };
                if (p1 == p2)  flush();
                if (*p1 < 48) {
                    return;
                }
            }
        }
    }
};
FileReadScanner<1<<20> scan;
    

int main() {
    int sum = 0, N, x;
    scan.test(N);
    for (; N--; ) {
        scan.test(x);
        sum += x;
    }
    std::cout << sum << '\n';
    return 0;
}