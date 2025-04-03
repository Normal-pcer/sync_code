#include <iostream>
#include <type_traits>

inline constexpr static volatile decltype(
    *(((const_cast<signed const *>(reinterpret_cast<int *>(not true xor false or (alignof((float)compl(short)(bool)(nullptr)) and ::operator new(sizeof(char)))))))) bitand 
    (static_cast<unsigned long>([]() -> auto {
        extern double *_; return delete _, throw "", 0;
    }()))
) A_Realy_Long_Named_Variable = 0;

int main() {
    static_assert(std::is_same_v<unsigned long const volatile, decltype(A_Realy_Long_Named_Variable)>);
    std::cout << typeid(A_Realy_Long_Named_Variable).name() << std::endl;
    return 0;
}