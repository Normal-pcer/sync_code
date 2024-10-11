#include <bits/stdc++.h>
namespace lib {
    template <typename T, const unsigned long long sz>
    class MapArray {
    public:
        T arr[sz];
        MapArray() = default;
        MapArray(const MapArray<T, sz>&) = default;
        MapArray(std::initializer_list<T>& il) {
            std::copy(il.begin(), il.end(), arr);
        }

        T& operator[](const unsigned long long idx) {
            return arr[idx];
        }
        T& operator()(const unsigned long long idx) {
            return arr[idx];
        }
    };
}