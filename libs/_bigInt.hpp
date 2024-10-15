#include <bits/stdc++.h>
namespace lib {
    class BigInt {
    public:
        // data 下标小的存储低位
        // n = data[0] + data[1] * 4294967296 + data[2] * 18446744073709551616 + ...
        std::vector<uint32_t> data;
        bool symbol=false;  // true 为负数

        BigInt() { data = {0}; }
        BigInt(unsigned x) { data = {(uint32_t)(x)}; }
        BigInt(int x) {
            if (x < 0) {
                symbol = true;
                x = -x;
            }
            data = {(uint32_t)(x)};
        }
        BigInt(unsigned long long x) { data = {(uint32_t)(x & 4294967295), (uint32_t)((x >> 32) & 4294967295)}; }

        BigInt(const std::string& s) {
            data.reserve(s.size() / 9 + 1);
            data.push_back(0);
            for (auto c: s) {
                if (c == '-') {
                    symbol ^= true;
                    continue;
                }
                unsigned cur = c - '0';
                (*this) *= 10;
                (*this) += cur;
            }
        }

        bool operator<(const BigInt& other) const {
            if (data.size() != other.data.size()) {
                return data.size() < other.data.size();
            }
            for (int i = data.size() - 1; i >= 0; --i) {
                if (data[i] != other.data[i]) {
                    return data[i] < other.data[i];
                }
            }
            return false;
        }

        bool operator==(const BigInt& other) const {
            return data == other.data && symbol == other.symbol;
        }

        bool operator>(const BigInt& other) const { return (other<*this); }
        bool operator<=(const BigInt& other) const { return !(*this > other); }
        bool operator!=(const BigInt& other) const { return !(*this == other); }
        bool operator>=(const BigInt& other) const { return !(*this < other); }

        BigInt operator+(const BigInt& other) const {
            BigInt res = *this;
            res += other;
            return res;
        }

        void operator+=(const BigInt& other) {
            data.resize(std::max(data.size(), other.data.size()) + 1);
            size_t dest = data.size() - 1;
            for (size_t i = 0; i < dest; ++i) {
                long long sum = (long long)
                                (i < data.size() ? data[i] : 0) +
                                (i < other.data.size() ? other.data[i] : 0);
                data[i] = (uint32_t)(sum);
                data[i+1] += (uint32_t)(sum >> 32);
            }
            if (data.back() == 0)  data.pop_back();
        }

        BigInt operator-(const BigInt& other) const {
            bool symbol = *this < other;
            const BigInt &greater = symbol ? other : *this;
            const BigInt &less = symbol ? *this : other;
            
            BigInt res = greater;
            res.symbol = symbol;
            for (size_t i = 0; i < less.data.size(); ++i) {
                if (res.data[i] < less.data[i]) {
                    res.data[i+1] -= 1;
                    res.data[i] -= less.data[i];
                    res.data[i] += 4294967296;
                } else {
                    res.data[i] -= less.data[i];
                }
            }

            return res;
        }

        void operator-=(const BigInt& other) {
            if (*this < other) {
                *this = other - *this;
                return;
            }
            for (size_t i = 0; i < other.data.size(); ++i) {
                if (data[i] < other.data[i]) {
                    data[i+1] -= 1;
                    data[i] -= other.data[i];
                    data[i] += 4294967296;
                } else {
                    data[i] -= other.data[i];
                }
            }
        }

        BigInt operator*(const BigInt& other) const {
            BigInt res;
            res.data.resize(data.size() + other.data.size());
            bool symbol = this->symbol ^ other.symbol;

            for (size_t i = 0; i < data.size(); ++i) {
                BigInt curRes = 0;
                curRes.data.resize(data.size() + other.data.size());
                for (size_t j = 0; j < other.data.size(); ++j) {
                    unsigned long long product = (unsigned long long)data[i] * (unsigned long long)other.data[j] + (unsigned long long)curRes.data[i + j];
                    curRes.data[i + j] = (uint32_t)(product);
                    curRes.data[i + j + 1] += (uint32_t)(product >> 32);
                }
                res += curRes;
            }

            while (res.data.back() == 0 && res.data.size() > 1) res.data.pop_back();
            res.symbol = symbol;
            return res;
        }

        void operator*=(const BigInt& other) {
            *this = ((*this) * other);
        }



        void operator<<=( size_t shift ) {
            if (shift == 0) return;
            if (shift < 32) {
                data.push_back(data.back() >> (32-shift));
                for (long long i = data.size() - 1; i >= 0; i--) {
                    data[i] <<= shift;
                    if (i > 0) data[i] |= data[i-1] >> (32-shift);
                }
            } else {
                size_t blocks = shift >> 5;
                shift &= 31;
                data.insert(data.begin(), blocks, 0);
                (*this) <<= shift;
            }
            while (data.back() == 0)  data.pop_back();
        }

        BigInt operator<<(size_t shift) const {
            BigInt res = *this;
            res <<= shift;
            return res;
        }

        void operator>>=( size_t shift ) {
            if (shift == 0) return;
            if (shift < 32) {
                for (size_t i = 0; i < data.size() - 1; i++) {
                    data[i] >>= shift;
                    if (i < data.size() - 1) data[i] |= data[i+1] << (32-shift);
                }
                data.back() >>= shift;
            } else {
                size_t blocks = shift >> 5;
                shift &= 31;
                data.erase(data.begin(), data.begin() + blocks);
                (*this) >>= shift;
            }
        }

        BigInt operator>>(size_t shift) const {
            BigInt res = *this;
            res >>= shift;
            return res;
        }

        std::pair<BigInt, int> div_mod(const int other) const {
            long long remain = 0;
            BigInt res = 0;
            for (auto it = data.rbegin(); it != data.rend(); ++it) {
                auto i = *it;
                long long a = remain * 4294967296 + i;
                remain = a % other;
                res = (res << 32) + (unsigned long long)(a/other);
            }
            if ((other < 0) ^ (symbol))  res.symbol = true;
            while (res.data.back() == 0)  res.data.pop_back();
            return std::make_pair(res, remain);
        }

        BigInt operator/(const int other) const {
            return div_mod(other).first;
        }

        int operator%(const int other) const {
            return div_mod(other).second;
        }

        void display() const {
            if (symbol) {
                printf("-");
            }
            for (auto it = data.rbegin(); it != data.rend(); ++it) {
                printf("%u ", *it);
            }
        }

        std::string to_string() const {
            std::string res;
            BigInt copy = *this;
            res.reserve(data.size() * 8);
            
            while (copy > 0) {
                auto c = copy.div_mod(10);
                res += (c.second + '0');
                copy = c.first;
            }
            if (symbol) {
                res += '-';
            }

            std::reverse(res.begin(), res.end());
            return res;
        }
    };
}