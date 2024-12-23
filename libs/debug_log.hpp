#define __SingleValueLog(x) #x << "=" << (x)
#define __ToOutStream_1(x) << __SingleValueLog(x)
#define __ToOutStream_2(x, args...) << __SingleValueLog(x) << ", " __ToOutStream_1(args)
#define __ToOutStream_3(x, args...) << __SingleValueLog(x) << ", " __ToOutStream_2(args)
#define __ToOutStream_4(x, args...) << __SingleValueLog(x) << ", " __ToOutStream_3(args)
#define __ToOutStream_5(x, args...) << __SingleValueLog(x) << ", " __ToOutStream_4(args)
#define __ToOutStream_6(x, args...) << __SingleValueLog(x) << ", " __ToOutStream_5(args)
#define __ToOutStream_7(x, args...) << __SingleValueLog(x) << ", " __ToOutStream_6(args)
#define __ToOutStream_8(x, args...) << __SingleValueLog(x) << ", " __ToOutStream_7(args)
#define __ToOutStream_9(x, args...) << __SingleValueLog(x) << ", " __ToOutStream_8(args)
#define __ToOutStream_overload(_1, _2, _3, _4, _5, _6, _7, _8, _9, _0, ...) __ToOutStream_##_0
#define __ToOutStream(...) __ToOutStream_overload(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1)(__VA_ARGS__)
#define printValues(...) std::cout << __LINE__ << ": " __ToOutStream(__VA_ARGS__) << std::endl