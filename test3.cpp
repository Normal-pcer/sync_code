#define MACROCAT(a, b) a##b
#define TOSTRING(x) #x

#define DECVAL_1 0
#define DECVAL_2 1
#define DECVAL_3 2
#define DECVAL_4 3
#define DECVAL_5 4
#define DECVAL_6 5
#define DECVAL_7 6
#define DECVAL_8 7
#define DECVAL_9 8
#define DECVAL( n ) DECVAL_##n

#define BIN_0(n, arg) TOSTRING ( arg )
#define BIN_1(n, arg) MACROCAT(BIN_, DECVAL(n)) (DECVAL(n), MACROCAT(arg, 0)) \
"," MACROCAT(BIN_, DECVAL(n)) (DECVAL(n), MACROCAT(arg, 1))

BIN_1(0, 1)