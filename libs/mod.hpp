#include <bits/stdc++.h>
namespace lib {
    template <typename T, const int p>
    class mod {
    public:
        T data;

        mod() = default;
        mod(T num) : data(positive(num % p)) {}

        T positive(T num) { 
            if (num>=0)  return num;
            if (num+p>=0)  return num+p;
            return num + p * ((-num) / p + 1);
        }

        mod operator+(const mod &other) { return mod(positive(data + other.data) % p); }
        mod operator-(const mod &other) { return mod(positive(data - other.data) % p); }
        mod operator*(const mod &other) { return mod(positive(data * other.data) % p); }
        mod operator/(const mod &other) { return mod((data * other.data)); }
    };
}