#include <stdio.h>
#define bool _Bool
enum {
false = 0,
true = 1,
};

void halfAdd(bool a, bool b, bool *base, bool *carry) {
    *base = a ^ b;
    *carry = a & b;
}

void fullAdd(bool a, bool b, bool c, bool *base, bool *carry) {
    bool base1, carry1;
    halfAdd(a, b, &base1, &carry1);

    bool base2, carry2;
    halfAdd(base1, c, &base2, &carry2);

    *base = base2;
    *carry = carry1 | carry2;
}

typedef unsigned int num;
void addNum(num a, num b, num *res, bool *overflow) {
#define getBit(x, p) ((x) & (1u << (p)))
#define setBit(x, p, val) (void)((x) |= (val << (p)))
    *res = 0;
    bool carry = false, base;
#define addAt(p) fullAdd(getBit(a, p), getBit(b, p), carry, &base, &carry), setBit(*res, p, base);
    addAt(0x00);
    addAt(0x01);
    addAt(0x02);
    addAt(0x03);
    addAt(0x04);
    addAt(0x05);
    addAt(0x06);
    addAt(0x07);
    addAt(0x08);
    addAt(0x09);
    addAt(0x0a);
    addAt(0x0b);
    addAt(0x0c);
    addAt(0x0d);
    addAt(0x0e);
    addAt(0x0f);
    addAt(0x10);
    addAt(0x11);
    addAt(0x12);
    addAt(0x13);
    addAt(0x14);
    addAt(0x15);
    addAt(0x16);
    addAt(0x17);
    addAt(0x18);
    addAt(0x19);
    addAt(0x1a);
    addAt(0x1b);
    addAt(0x1c);
    addAt(0x1d);
    addAt(0x1e);
    addAt(0x1f);
    *overflow = carry;
}

int main() {
    num a, b, res;
    bool overflow;

    scanf("%u%u", &a, &b);
    addNum(a, b, &res, &overflow);
    printf("%u\n", res);
}