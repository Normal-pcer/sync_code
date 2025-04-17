#ifndef LIB_INFINITY_NUMBER_LESS
#define LIB_INFINITY_NUMBER_LESS
#include <type_traits>
#include <limits>
#include <cmath>

namespace lib {
    /**
     * 提供相对较小的极大值，保证二倍不超过类型上限，并且方便使用 std::memset 填充数组。
     * 具体地，极大值相当于对于类型 T 按照如下值调用 std::memset
     * 有符号整数：0x3f
     * 无符号整数：0x7f
     * 浮点数：使用浮点数的 inf。浮点数希望做到如同调用 std::memset 的效果，
     *  依赖于 C++20 的 std::bit_cast 来安全地进行类型双关，所以不提供支持。
     */
    namespace InfNumber {
        template <typename T>
        auto constexpr memset_integer(unsigned char byte) -> T {
            static_assert(std::is_integral<T>::value);
            T res = 0;
            for (std::size_t i = 0; i != sizeof(T); i++) {
                res = (res << 8) | byte;
            }
            return res;
        }

        // 主模板触发编译错误
        template <typename T, typename Enable = void>
        struct MaxValueOf {
            static_assert(sizeof(T) == 0, "Unsupported type");
        };
    
        // 浮点特化：返回 inf
        template <typename T>
        struct MaxValueOf<T, std::enable_if_t<std::is_floating_point<T>::value>> {
            T static constexpr value = std::numeric_limits<T>::infinity();
        };
    
        // 有符号整数特化：返回 0x3f
        template <typename T>
        struct MaxValueOf<T, std::enable_if_t<
            std::is_integral<T>::value &&
            std::is_signed<T>::value
        >> {
            T static constexpr value = memset_integer<T>(0x3f);
        };

        // 无符号整数特化：返回 0x7f
        template <typename T>
        struct MaxValueOf<T, std::enable_if_t<
            std::is_integral<T>::value &&
            std::is_unsigned<T>::value
        >> {
            T static constexpr value = memset_integer<T>(0x7f);
        };
    
        // InfType 类支持隐式转换
        class InfType {
        public:
            template <typename T>
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
        InfType static constexpr inf;
        NegaInfType static constexpr negaInf;
    
    
        // 判断类型是否支持无穷大值
        template<typename T>
        struct HasInfinity: std::integral_constant<bool,
            std::is_arithmetic<T>::value
        > {};
    
        template <typename T>
        struct HasNegaInfinity: std::integral_constant<bool, 
            std::is_arithmetic<T>::value && !std::is_unsigned<T>::value
        > {};
    
        // 重载 == 运算符
        template <typename T> auto operator== (T val, InfType) -> std::enable_if_t<HasInfinity<T>::value, bool> {
            return val == MaxValueOf<T>::value;
        }
    
        // 重载 != 运算符
        template <typename T> auto operator!= (T val, InfType) -> std::enable_if_t<HasInfinity<T>::value, bool> {
            return !(val == inf);
        }
    
        // 删除不可用类型的运算符
        template <typename T> auto operator== (T, InfType) -> std::enable_if_t<!HasInfinity<T>::value, void> = delete;
    
        template <typename T> auto operator!= (T, InfType) -> std::enable_if_t<!HasInfinity<T>::value, void> = delete;
    
        auto constexpr operator- (InfType) -> NegaInfType {
            return negaInf;
        }
        auto constexpr operator+ (InfType) -> InfType {
            return inf;
        }
    
        // 重载 == 运算符
        template <typename T> auto operator==(T val, NegaInfType) -> std::enable_if_t<HasNegaInfinity<T>::value, bool> {
            return val == -MaxValueOf<T>::value;
        }
        // 重载 != 运算符
        template <typename T> auto operator!=(T val, NegaInfType) -> std::enable_if_t<HasNegaInfinity<T>::value, bool>{
            return !(val == inf);
        }
    
        // 删除不可用类型的运算符
        template <typename T> auto operator==(T, NegaInfType) -> std::enable_if_t<!HasNegaInfinity<T>::value, void> = delete;
        template <typename T> auto operator!=(T, NegaInfType) -> std::enable_if_t<!HasNegaInfinity<T>::value, void> = delete;
    }

    using InfNumber::inf;
}
#endif  // not def LIB_INFINITY_NUMBER_LESS
