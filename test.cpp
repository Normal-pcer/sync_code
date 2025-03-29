#include <bits/stdc++.h>

template <typename T, typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
auto add_overflow(T x, T y, T *res) -> bool {
    return __builtin_add_overflow(x, y, res);
}

template <typename T, typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
auto inc_overflow(T *x) -> bool {
    return __builtin_add_overflow(*x, 1, x);
}

template <typename T>
struct Wrap {
    T value = 0;

    friend auto inc_overflow(Wrap *x) -> bool {
        return inc_overflow(&x->value);
    }
    friend auto add_overflow(Wrap const &x, Wrap const &y, Wrap *res) -> bool {
        return add_overflow(x.value, y.value, &res->value);
    }
    auto inplace_invert() -> Wrap & {
        value = ~value;
        return *this;
    }
};

template <typename T>
struct S {
    T low{}, high{};

    friend auto inc_overflow(S *x) -> bool {
        auto flag = inc_overflow(&x->low);
        if (flag) return inc_overflow(&x->high);
    }

    friend auto add_overflow(S const &x, S const &y, S *res) -> bool {
        auto flag = add_overflow(x.low, y.low, &res->low);
        auto flag2 = add_overflow(x.high, y.high, &res->high);
        if (flag) flag2 |= inc_overflow(&res->high);
        return flag2;
    }

    auto inplace_invert() -> S & {
        low.inplace_invert();
        high.inplace_invert();
        return *this;
    }
    auto operator~ () const -> S {
        auto tmp = *this;
        tmp.inplace_invert();
        return tmp;
    }
    auto inplace_neg() -> S & {
        inplace_invert();
        inc_overflow(this);
        return *this;
    }
    auto operator- () const -> S {
        auto tmp = *this;
        tmp.inplace_neg(this);
        return *tmp;
    }
    auto operator+= (S const &other) -> S & {
        add_overflow(*this, other, *this);
        return *this;
    }
    auto operator-= (S const &other) -> S & {
        *this += -other;
        return *this;
    }
    auto operator+ () const -> S {
        return *this;
    }
    auto operator+ (S const &other) -> S {
        auto tmp = *this;
        tmp += other;
        return tmp;
    }
    auto operator- (S const &other) -> S {
        auto tmp = *this;
        tmp -= other;
        return tmp;
    }

};

int main() {
    using u64 = Wrap<unsigned long long>;
    using u128 = S<u64>;
    u128 x, y, z;
    x.low = x.high = u64{0xEFFFFFFFFFFFFFFFULL};
    y.low = u64{0xEEEEEEEEEEEEEEEEULL};
    std::cout << add_overflow(x, y, &z) << std::endl;
    z = ~z;
    std::cout << z.high.value << " " << z.low.value << std::endl;
}
