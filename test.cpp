#include <cstdint>
#include <iostream>

struct MagicParams {
    uint64_t magic;  // 魔术数
    int shift;       // 移位参数
};

MagicParams compute_magic(uint32_t d) {
    if (d == 0) throw std::invalid_argument("d cannot be zero");

    int k = 32; // 初始位数（32位整数）
    // 找到最小的 k 使得 2^k >= d * (2^32)
    while (((1ULL << k) / d) < (1ULL << 32)) {
        k++;
    }

    // 计算魔术数 m = ceil(2^k / d)
    uint64_t m = (1ULL << k) / d;
    if ((1ULL << k) % d != 0) m++;  // 向上取整

    // 移位参数为 k（而非 k - 32）
    return {m, k};
}

int main() {
    uint32_t d = 100;
    auto [m, shift] = compute_magic(d);
    
    std::cout << "Magic number for " << d << ": 0x" << std::hex << m 
              << " (shift " << std::dec << shift << " bits)" << std::endl;
    
    // 验证计算：n % 10
    int32_t n = 12345678;
    int32_t q = (n * m) >> shift;  // 直接使用 shift = k
    int32_t r = n - q * d;          // 余数
    if (r < 0) r += d;              // 处理负数
    
    std::cout << n << " % " << d << " = " << r << std::endl;
    return 0;
}