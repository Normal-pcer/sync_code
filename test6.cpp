#include <bits/stdc++.h>

namespace unstd {
    using size_t = unsigned long long;
    using ptrdiff_t = long long;

    template <typename T, typename U>  concept ConvertibleTo = requires (T x) {
        static_cast<U>(x);
    };

    // 可以通过迭代器向后遍历
    template <typename T>  concept OutputIterable = requires (T x) {  ++x.begin(), *x.begin();  };
    // 可以通过迭代器向后遍历，并且判断边界
    template <typename T>  concept Iterable = requires (T x) {  requires OutputIterable<T>; x.end(); };

    void print(Iterable auto const &x) {
        for (auto i: x)  std::cout << i << ' ';
        std::cout << std::endl;
    }

    void transform(Iterable auto const &origin, OutputIterable auto &&target, auto const &op) {
        auto it1 = origin.begin();
        auto it2 = target.begin();
        for (; it1 != origin.end(); ++it1, ++it2) {
            *it2 = op(*it1);
        }
    }

    template <typename OutType>
    auto transform(Iterable auto const &origin, auto const &op) {
        auto target = OutType();
        for (auto it = origin.begin(); it != origin.end(); ++it) {
            target.push_back(op(*it));
        }
        return target;
    }

    template <OutputIterable T>
    struct BackInserter {
        T &target;

        auto begin() { return std::back_inserter(target); }
    };

    template <Iterable OriginalType>
    struct Transform {
        const OriginalType &origin;
    };
}
 
int main() {
    std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> vec2;
    unstd::transform(vec, unstd::BackInserter(vec2), [&](int x) { return x % 3; });
    unstd::print(vec2);
    std::vector vec3 = unstd::transform<std::vector<int>>(vec, [&](int x) { return x % 4; });
    unstd::print(vec3);
    return 0;
}