#define __SingleValueLog(x) #x << "=" << (x)
#define __ToOutStream_1(x) << __SingleValueLog(x)
#define __ToOutStream_2(x, ...) << __SingleValueLog(x) << ", " __ToOutStream_1(__VA_ARGS__)
#define __ToOutStream_3(x, ...) << __SingleValueLog(x) << ", " __ToOutStream_2(__VA_ARGS__)
#define __ToOutStream_4(x, ...) << __SingleValueLog(x) << ", " __ToOutStream_3(__VA_ARGS__)
#define __ToOutStream_5(x, ...) << __SingleValueLog(x) << ", " __ToOutStream_4(__VA_ARGS__)
#define __ToOutStream_6(x, ...) << __SingleValueLog(x) << ", " __ToOutStream_5(__VA_ARGS__)
#define __ToOutStream_7(x, ...) << __SingleValueLog(x) << ", " __ToOutStream_6(__VA_ARGS__)
#define __ToOutStream_8(x, ...) << __SingleValueLog(x) << ", " __ToOutStream_7(__VA_ARGS__)
#define __ToOutStream_9(x, ...) << __SingleValueLog(x) << ", " __ToOutStream_8(__VA_ARGS__)
#define __ToOutStream_overload(_1, _2, _3, _4, _5, _6, _7, _8, _9, _0, ...) __ToOutStream_##_0
#define __ToOutStream(...) __ToOutStream_overload(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1)(__VA_ARGS__)
#define printValues(...) std::cout << __LINE__ << ": " __ToOutStream(__VA_ARGS__) << std::endl
