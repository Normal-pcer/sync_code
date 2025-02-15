namespace lib {
    template <typename T>
    auto constexpr qpow(std::int64_t a, std::int64_t b, T const p) -> T {
        std::int64_t res = 1;
        for (; b; b >>= 1, a = a * a % p) {
            if (b & 1)  res = res * a % p;
        }
        return res;
    }
    template <typename T, T mod, bool isPrime = false>
    struct ModInt {
        using Self = ModInt<T, mod, isPrime>;
        T value;  // 保证这个值在 [0, mod)

        ModInt(): value(0) {}
        ModInt(T x): value(x % mod) {}

        auto constexpr operator+ (Self other) const -> Self {
            T new_value = value + other.value;
            if (new_value >= mod)  new_value -= mod;
            return {new_value};
        }
        auto constexpr operator- (Self other) const -> Self {
            T new_value = value - other.value;
            if (new_value < 0)  new_value += mod;
            return {new_value};
        }
        auto constexpr operator* (Self other) const -> Self {
            std::int64_t new_value = value;
            new_value *= other.value;
            return {static_cast<T>(new_value % mod)};
        }
        auto constexpr operator/ (Self other) const -> Self requires(isPrime) {
            std::int64_t new_value = value;
            new_value *= qpow(other.value, mod - 2, mod);
            return {static_cast<T>(new_value % mod)};
        }
    };
    template <typename T, size_t Tag>
    struct TagModInt {
        using Self = TagModInt<T, Tag>;
        static inline T mod = 1;
        static inline bool isPrime = false;
        int value;

        TagModInt(): value(0) {}
        TagModInt(T x): value(x % mod) {}
        static auto constexpr setMod(T p, bool prime = false) -> void {
            mod = p;
            isPrime = prime;
        }
        auto constexpr operator+ (Self other) const -> Self {
            T new_value = value + other.value;
            if (new_value >= mod)  new_value -= mod;
            return {new_value};
        }
        auto constexpr operator- (Self other) const -> Self {
            T new_value = value - other.value;
            if (new_value < 0)  new_value += mod;
            return {new_value};
        }
        auto constexpr operator* (Self other) const -> Self {
            std::int64_t new_value = value;
            new_value *= other.value;
            return {static_cast<T>(new_value % mod)};
        }
        auto constexpr operator/ (Self other) const -> Self {
            assert(isPrime);
            std::int64_t new_value = value;
            new_value *= qpow(other.value, mod - 2, mod);
            return {static_cast<T>(new_value % mod)};
        }
    };
}