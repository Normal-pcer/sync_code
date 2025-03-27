#ifndef LIB_NULL_NUMBER
#define LIB_NULL_NUMBER
#include <cmath>
#include <iostream>
#include <limits>
#include <type_traits>

namespace lib {
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
        template <typename T, typename Enable = void>
        struct InvalidValueOf {
            static_assert(sizeof(T) == 0, "Unsupported type");
        };

        // 浮点特化：返回 NaN
        template <typename T>
        struct InvalidValueOf<T, std::enable_if_t<std::is_floating_point<T>::value>> {
            static constexpr T value = std::numeric_limits<T>::quiet_NaN();
        };

        // 有符号整数特化：返回最小值
        template <typename T>
        struct InvalidValueOf<T, std::enable_if_t<
            std::is_integral<T>::value && 
            std::is_signed<T>::value
        >> {
            T static constexpr value = std::numeric_limits<T>::min();
        };

        // 无符号整数特化（排除 bool）：返回最大值
        template<typename T>
        struct InvalidValueOf<T, std::enable_if_t<
            std::is_integral<T>::value && 
            std::is_unsigned<T>::value && 
            !std::is_same<T, bool>::value
        >> {
            T static constexpr value = std::numeric_limits<T>::max();
        };

        // NullType 类支持隐式转换
        class NullType {
        public:
            template <typename T>
            constexpr operator T() const {
                return InvalidValueOf<T>::value;
            }
        };

        // 全局 null 常量
        NullType static constexpr null;

        // 判断类型是否支持空值
        template<typename T>
        struct IsNullable: std::integral_constant<bool,
            std::is_arithmetic<T>::value && 
            !std::is_same<T, bool>::value
        > {};

        // 重载 == 运算符（支持浮点 NaN 比较）
        template <typename T>
        auto operator== (T val, NullType) -> std::enable_if_t<IsNullable<T>::value && std::is_floating_point<T>::value, bool> {
            return std::isnan(val);
        }
        template <typename T>
        auto operator== (T val, NullType) -> std::enable_if_t<IsNullable<T>::value && std::is_integral<T>::value, bool> {
            return val == InvalidValueOf<T>::value;
        }
        // 重载 != 运算符
        template <typename T> auto operator!= (T val, NullType) -> std::enable_if_t<IsNullable<T>::value, bool> {
            return !(val == null);
        }
        // 删除不可用类型的运算符
        template <typename T> auto operator== (T, NullType) -> std::enable_if_t<!IsNullable<T>::value, void> = delete;
        template <typename T> auto operator!= (T, NullType) -> std::enable_if_t<!IsNullable<T>::value, void> = delete;
    }
    
    using NullNumber::null;
}
#endif  // not def LIB_NULL_NUMBER
