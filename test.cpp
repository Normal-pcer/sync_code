#include <type_traits>
#include <limits>
#include <iostream>
#include <cmath>
#include <cassert>

/**
 * 提供“可空”的数字类型。
 * 具体地，会在每个类型中去除一些值作为空值，判断是否为指定的空值。
 * 有符号整数：最小值
 * 无符号整数：最大值
 * 浮点数：NaN
 * 
 * 如同从值域中删除了对应值。如果有效值和保留的空值相同，则行为未定义。
 */
namespace NullNumber {
    // 主模板触发编译错误
    template<typename T, typename Enable = void>
    struct InvalidValueOf {
        static_assert(sizeof(T) == 0, "Unsupported type");
    };

    // 浮点特化：返回 NaN
    template<typename T>
    struct InvalidValueOf<T, std::enable_if_t<std::is_floating_point_v<T>>> {
        static constexpr T value = std::numeric_limits<T>::quiet_NaN();
    };

    // 有符号整数特化：返回最小值
    template<typename T>
    struct InvalidValueOf<T, std::enable_if_t<
        std::is_integral_v<T> && 
        std::is_signed_v<T>
    >> {
        static constexpr T value = std::numeric_limits<T>::min();
    };

    // 无符号整数特化（排除 bool）：返回最大值
    template<typename T>
    struct InvalidValueOf<T, std::enable_if_t<
        std::is_integral_v<T> && 
        std::is_unsigned_v<T> && 
        !std::is_same_v<T, bool>
    >> {
        static constexpr T value = std::numeric_limits<T>::max();
    };

    // NullType 类支持隐式转换
    class NullType {
    public:
        template<typename T>
        constexpr operator T() const {
            return InvalidValueOf<T>::value;
        }
    };

    // 全局 null 常量
    inline constexpr NullType null;

    // 判断类型是否支持空值
    template<typename T>
    struct IsNullable : std::integral_constant<bool,
        std::is_arithmetic_v<T> && 
        !std::is_same_v<T, bool>
    > {};

    // 重载 == 运算符（支持浮点 NaN 比较）
    template<typename T>
    std::enable_if_t<IsNullable<T>::value, bool>
    operator==(T val, NullType) {
        if constexpr (std::is_floating_point_v<T>) {
            return std::isnan(val);
        } else {
            return val == InvalidValueOf<T>::value;
        }
    }

    // 重载 != 运算符
    template<typename T>
    std::enable_if_t<IsNullable<T>::value, bool>
    operator!=(T val, NullType) {
        return !(val == null);
    }

    // 删除不可用类型的运算符
    template<typename T>
    std::enable_if_t<!IsNullable<T>::value, void>
    operator==(T, NullType) = delete;

    template<typename T>
    std::enable_if_t<!IsNullable<T>::value, void>
    operator!=(T, NullType) = delete;
}

namespace InfNumber {
    // 主模板触发编译错误
    template<typename T, typename Enable = void>
    struct MaxValueOf {
        static_assert(sizeof(T) == 0, "Unsupported type");
    };

    // 浮点特化：返回 inf
    template<typename T>
    struct MaxValueOf<T, std::enable_if_t<std::is_floating_point_v<T>>> {
        static constexpr T value = std::numeric_limits<T>::infinity();
    };

    // 整数特化：返回最大值
    template<typename T>
    struct MaxValueOf<T, std::enable_if_t<
        std::is_integral_v<T>
    >> {
        static constexpr T value = std::numeric_limits<T>::max();
    };

    // InfType 类支持隐式转换
    class InfType {
    public:
        template<typename T>
        constexpr operator T() const {
            return MaxValueOf<T>::value;
        }
    };

    class NegaInfType {
    public:
        template <typename T, typename std::enable_if_t<!std::is_unsigned<T>::value, int> = 0>
        constexpr operator T() const {
            return -MaxValueOf<T>::value;
        }
    };

    // 全局无穷大常量
    inline constexpr InfType inf;
    inline constexpr NegaInfType negaInf;

    // 判断类型是否支持无穷大值
    template<typename T>
    struct HasInfinity : std::integral_constant<bool,
        std::is_arithmetic_v<T>
    > {};

    template <typename T>
    struct HasNegaInfinity: std::integral_constant<bool, 
        std::is_arithmetic_v<T> && !std::is_unsigned_v<T>
    > {};

    // 重载 == 运算符
    template<typename T>
    std::enable_if_t<HasInfinity<T>::value, bool>
    operator==(T val, InfType) {
        return val == MaxValueOf<T>::value;
    }

    // 重载 != 运算符
    template<typename T>
    std::enable_if_t<HasInfinity<T>::value, bool>
    operator!=(T val, InfType) {
        return !(val == inf);
    }

    // 删除不可用类型的运算符
    template<typename T>
    std::enable_if_t<!HasInfinity<T>::value, void>
    operator==(T, InfType) = delete;

    template<typename T>
    std::enable_if_t<!HasInfinity<T>::value, void>
    operator!=(T, InfType) = delete;

    auto operator- (InfType) -> NegaInfType {
        return negaInf;
    }
    auto operator+ (InfType) -> InfType {
        return inf;
    }

    // 重载 == 运算符
    template<typename T>
    std::enable_if_t<HasNegaInfinity<T>::value, bool>
    operator==(T val, NegaInfType) {
        return val == -MaxValueOf<T>::value;
    }

    // 重载 != 运算符
    template<typename T>
    std::enable_if_t<HasNegaInfinity<T>::value, bool>
    operator!=(T val, NegaInfType) {
        return !(val == inf);
    }

    // 删除不可用类型的运算符
    template<typename T>
    std::enable_if_t<!HasNegaInfinity<T>::value, void>
    operator==(T, NegaInfType) = delete;

    template<typename T>
    std::enable_if_t<!HasNegaInfinity<T>::value, void>
    operator!=(T, NegaInfType) = delete;
}

/* 使用示例 */
signed main() {
    using namespace NullNumber;
    using namespace InfNumber;
    using namespace std;
    int x = inf;
    long long y = -inf;

    std::cout << x << " " << y << std::endl;
    assert(x == +inf);
    assert(y == -inf);
}
