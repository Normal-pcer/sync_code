#include <stdfloat>
 
#if __STDCPP_BFLOAT16_T__ != 1
    #error "要求 64 位浮点数类型"
#endif
 
int main()
{
    std::float64_t f = 0.1f64;
}