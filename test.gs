var ret: int;
def qpow(a: int, b: int, mod: int) {
    if (b == 0) {
        ret = 1;
    } 
    if (b != 0) {
        qpow(a, b / 2, mod);
        var half: int;
        half = ret;
        if (b % 2 == 0) {
            ret = (half * half) % mod;
        }
        if (b % 2 == 1) {
            ret = (half * half * a) % mod;
        }
    }
}

var i: int;
var a: int;
var b: int;
var mod: int;
mod = 998244353;

for (i = 0; i < 10; i = i + 1) {
    scan(a);
    scan(b);
    qpow(a, b, mod);
    print(ret);
    print("\n");
}