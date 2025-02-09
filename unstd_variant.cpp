#include <bits/stdc++.h>

namespace unstd {
    template <typename T>
    auto constexpr max(T const &x, T const &y) -> T { return x > y? x :y; }
    template <typename ...Ts>
    struct __MaxSizeof;
    template <>
    struct __MaxSizeof<> {
        static constexpr size_t value = 0;
    };
    template <typename T, typename ...Ts>
    struct __MaxSizeof<T, Ts...> {
        static constexpr size_t value = max(sizeof(T), __MaxSizeof<Ts...>::value);
    };

    template <typename ...Ts>
    struct __MaxAlignof;
    template <>
    struct __MaxAlignof<> {
        static constexpr size_t value = 1;
    };
    template <typename T, typename ...Ts>
    struct __MaxAlignof<T, Ts...> {
        static constexpr size_t value = max(alignof(T), __MaxAlignof<Ts...>::value);
    };

    template <int index, typename ...Ts>
    struct __IndexOfPack;
    template <int index, typename T, typename ...Ts>
    struct __IndexOfPack<index, T, Ts...> {
        using type = typename __IndexOfPack<index - 1, Ts...>::type;
    };
    template <typename T, typename ...Ts>
    struct __IndexOfPack<0, T, Ts...> {
        using type = T;
    };
    template <typename T, typename ...Ts>
    struct __FindInPack;
    template <typename T, typename U, typename ...Ts>
    struct __FindInPack<T, U, Ts...> {
        static constexpr int value = __FindInPack<T, Ts...>::value + 1;
    };
    template <typename T, typename ...Ts>
    struct __FindInPack<T, T, Ts...> {
        static constexpr int value = 0;
    };
    template <typename T>
    struct __FindInPack<T> {
        static constexpr int value = 0;
    };

    template <typename ...Ts>
    struct __Destroyer; 
    template <>
    struct __Destroyer<> {
        static auto destroy(int, char *) -> void {
            assert(false);
        }
    };
    template <typename T, typename ...Ts>
    struct __Destroyer<T, Ts...> {
        static auto destroy(int index, char *data) -> void {
            if (index == 0) {
                reinterpret_cast<T *>(data)->~T();
            } else {
                __Destroyer<Ts...>::destroy(index - 1, data);
            }
        }
    };
    template <typename ...Ts>
    struct __Copier;
    template <>
    struct __Copier<> {
        static auto copy(int, const char *, char *) -> void {
            assert(false);
        }
    };
    template <typename T, typename ...Ts>
    struct __Copier<T, Ts...> {
        static auto copy(int index, char const *src, char *dst) -> void {
            if (index == 0) {
                new (dst) T(*reinterpret_cast<T const *>(src));
            } else {
                __Copier<Ts...>::copy(index - 1, src, dst);
            }
        }
    };
    template <typename ...Ts>
    struct __Mover;
    template <>
    struct __Mover<> {
        static auto move(int, char *, char *) -> void {
            assert(false);
        }
    };
    template <typename T, typename ...Ts>
    struct __Mover<T, Ts...> {
        static auto move(int index, char *src, char *dst) -> void {
            if (index == 0) {
                new (dst) T(std::move(*reinterpret_cast<T *>(src)));
            } else {
                __Mover<Ts...>::move(index - 1, src, dst);
            }
        }
    };
    template <typename ...Ts>
    struct __CountPack;
    template <>
    struct __CountPack<> {
        static constexpr int value = 0;
    };
    template <typename T, typename ...Ts>
    struct __CountPack<T, Ts...> {
        static constexpr int value = 1 + __CountPack<Ts...>::value;
    };
    template <typename ...Ts> 
    struct alignas(__MaxAlignof<Ts...>::value) variant {
        char data[__MaxSizeof<Ts...>::value];
        int index = -1;  // -1 表示无效值

        static constexpr int count = __CountPack<Ts...>::value;

        variant() = default;
        variant(variant<Ts...> const &other) {
            if (other.index != -1)  __Copier<Ts...>::copy(other.index, other.data, data);
            index = other.index;
        }
        variant(variant<Ts...> &&other) {
            if (other.index != -1)  __Mover<Ts...>::move(other.index, other.data, data);
            index = other.index;
            other.index = -1;
        }
        template <typename T, typename = typename std::enable_if<not std::is_same<typename std::remove_reference<T>::type, variant<Ts...>>::value>::type>
        variant(T &&x) {
            set(std::forward<T>(x));
        }
        ~variant() {
            if (index != -1)  __Destroyer<Ts...>::destroy(index, data);
        }

        template <typename T>
        auto get() -> T & {
            static_assert(__FindInPack<T, Ts...>::value != count, "type not found");
            assert((index == __FindInPack<T, Ts...>::value));
            return *reinterpret_cast<T *>(data);
        }

        template <typename T>
        auto get() const -> T const & {
            static_assert(__FindInPack<T, Ts...>::value != count, "type not found");
            assert((index == __FindInPack<T, Ts...>::value));
            return *reinterpret_cast<T const *>(data);
        }

        template <typename T>
        auto set(T &&x) -> void {
            using rm_ref = typename std::remove_reference<T>::type;
            static_assert(__FindInPack<rm_ref, Ts...>::value != count, "type not found");
            if (index != -1) {
                __Destroyer<Ts...>::destroy(index, data);
                index = -1;
            }
            index = __FindInPack<rm_ref, Ts...>::value;
            new (data) rm_ref(std::forward<T>(x));
        }

        template <typename T, typename = typename std::enable_if<not std::is_same<typename std::remove_reference<T>::type, variant<Ts...>>::value>::type>
        auto operator= (T &&x) -> variant & {
            set(std::forward<T>(x));
            return *this;
        }
        auto operator= (variant<Ts...> const &other) -> variant & {
            if (&other == this)  return *this;
            if (other.index == -1)  __Destroyer<Ts...>::destroy(index, data);
            if (other.index != -1)  __Copier<Ts...>::copy(other.index, other.data, data);
            index = other.index;
            return *this;
        }
        auto operator= (variant<Ts...> &&other) -> variant & {
            if (&other == this)  return *this;
            if (other.index == -1)  __Destroyer<Ts...>::destroy(index, data);
            if (other.index != -1)  __Mover<Ts...>::move(other.index, other.data, data);
            index = other.index, other.index = -1;
            return *this;
        }
    };
}

template <typename T, typename ...Ts>
auto get(unstd::variant<Ts...> &v) -> T & {
    return v.template get<T>();
}

template <typename T, typename ...Ts>
auto get(unstd::variant<Ts...> const &v) -> T const & {
    return v.template get<T>();
}

int main() {}