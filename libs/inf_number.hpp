#ifndef LIB_INFINITY_NUMBER
#define LIB_INFINITY_NUMBER
#include <type_traits>
#include <limits>
#include <iostream>
#include <cmath>

namespace lib {
    namespace InfNumber {
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
    
        // 整数特化：返回最大值
        template <typename T>
        struct MaxValueOf<T, std::enable_if_t<
            std::is_integral<T>::value
        >> {
            T static constexpr value = std::numeric_limits<T>::max();
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
    
        auto operator- (InfType) -> NegaInfType {
            return negaInf;
        }
        auto operator+ (InfType) -> InfType {
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
#endif  // not def LIB_INFINITY_NUMBER
