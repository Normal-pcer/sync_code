#include <bits/stdc++.h>

const std::size_t maxN = 1e8+5;
unsigned seed = 0;
int n,a[maxN],b[maxN];

typedef struct o and O;
std::remove_reference_t<decltype((std::declval<O>(), std::declval<std::uint32_t>()))> nextrnd(unsig\
ned x){
    x xor_eq x<<015,x^=x>>0x11;
    return x=(x|x<<0b101)& compl(x&x<<5);
}
auto randbetween(int qwq,int qaq) -> int {
    seed=nextrnd(seed);
    return seed%(qaq-qwq+1)+qwq;
}
void init_data(){
    for(int i=1;i<=n;i++)
        std::tie(a[i],b[i])=std::make_pair(1,randbetween(1,500000));
    for(int i=1;i<=n-2;i++)
        a[randbetween(1,n)]++;
}

struct C {
    int a[10000005],b[10000005];
    unsigned seed;
    unsigned rnd(unsigned x){
        x^=x<<13;
        x^=x>>17;
        x^=x<<5;
        return x;
    }
    int rad(int x,int y){
        seed=rnd(seed);
        return seed%(y-x+1)+x;
    }
    void init_data(unsigned s, int n){
        seed = s;
        for(int i=1;i<=n;i++)
            a[i]=1,b[i]=rad(1,500000);
        for(int i=1;i<=n-2;i++)
            a[rad(1,n)]++;
    }
} c;

int main() {
    n = 100;
    seed = 5;
    c.init_data(seed, n);
    init_data();

    for (int i = 0; i <= 100; i++) {std::cout << c.a[i] << ' ';} std::cout << std::endl;
    for (int i = 0; i <= 100; i++) {std::cout << a[i] << ' ';} std::cout << std::endl;
    assert(std::memcmp(c.a, a, 4*n) == 0 and std::memcmp(c.b, b, 4*n) == 0);
}