#include <bits/stdc++.h>

namespace Space {
    struct Foo {
        int a, b;

        operator std::string () const {
            return std::format("Foo({}, {})", a, b);
        }
    };
}
template <typename _CharT>
struct std::formatter<Space::Foo, _CharT>: std::formatter<std::string, _CharT> {
    using base_t = std::formatter<std::string, char>;

    auto format(auto const &v, auto &context) const {
        return base_t::format((std::string)v, context);
    }
};
namespace Space {
    void func() {
        Space::Foo f{1, 2};
        std::cout << std::format("It is {}", f) << std::endl;
    }
}

int main() {
    Space::func();
    return 0;
}