#include <bits/stdc++.h>

namespace Test {
    static constexpr auto clz(unsigned int x) -> unsigned int {
        return __builtin_clz(x);
    }
    struct UnicodeChar {
        uint32_t data;
        auto size() const -> size_t {
            unsigned int x = clz(data);
            if (x == 0U)  return 1;
            else  return 4 - (x >> 3);
        }
    };
    struct UnicodeString {
        size_t _align = 1;              // 对齐长度
        size_t _size = 0;               // 字符数量
        size_t _capacity = 0;           // base 数组能存储的字符数
        char *storage_ptr = nullptr;    // 存储区域，按照 UTF-8 编码（用 0 补全到 align）

        static constexpr const int MaxAlign = 4;
        ~UnicodeString() {
            delete[] storage_ptr;  // 删除存储区
        }

        auto size() const noexcept -> size_t { return _size; }
        auto align() const noexcept -> size_t { return _align; }
        auto capacity() const noexcept -> size_t { return _capacity; }
        auto pushBack(UnicodeChar ch) -> void {
            auto char_size = ch.size();
            if (char_size > align()) {
                // 计算新的缓冲区大小
                size_t new_align = char_size;  // 对齐到当前字符宽度
                size_t new_storage_bytes = new_align * capacity();  // 存储区的字符数量不变
                char *new_storage_ptr = new char[new_storage_bytes];  // 新的存储区指针
                std::memset(new_storage_ptr, 0, new_storage_bytes);
                for (size_t i = 0, src = 0, dest = 0; i < size(); i++, src += align(), dest += new_align) {
                    std::memcpy(&new_storage_ptr[dest] + (new_align - align()), &storage_ptr[src], align());
                }
                _align = new_align;
                delete[] storage_ptr, storage_ptr = new_storage_ptr;
            }
            if (size() >= capacity()) {
                size_t new_capacity = capacity()? capacity() << 1: 1;  // 新的存储区字符数
                size_t new_storage_bytes = new_capacity * align();
                size_t data_bytes = size() * align();
                char *new_storage_ptr = new char[new_storage_bytes];
                std::memset(new_storage_ptr, 0, new_storage_bytes);
                if (storage_ptr != nullptr) {
                    std::memcpy(new_storage_ptr, storage_ptr, data_bytes);
                }
                _capacity = new_capacity;
                delete[] storage_ptr, storage_ptr = new_storage_ptr;
            }
            // 取 ch 数字的低位
            std::array<char, MaxAlign> bytes;  // 分字节存储
            {
                std::memset(bytes.begin(), 0, sizeof(bytes));
                uint32_t val = ch.data;
                char *pt = bytes.begin();
                for (size_t i = 0; i < char_size; i++) {
                    *pt++ = val & 255U;
                    val >>= 8;
                }
                std::reverse(bytes.begin(), bytes.begin() + align());
            }
            std::memcpy(&storage_ptr[size() * align()], bytes.begin(), align());  // 复制到存储区末尾
            _size++;
        }

        auto operator[] (size_t pos) const -> UnicodeChar {
            uint32_t val = 0;
            size_t base_offset = pos * align();
            for (size_t i = 0; i < align(); i++) {
                unsigned char x = storage_ptr[i + base_offset];
                val = (val << 8) + x;
            }
            return UnicodeChar{val};
        }
        friend auto operator>> (std::istream &st, UnicodeString &u_str) -> std::istream & {
            while (true) {
                char c;  st >> c;
                unsigned char head = c;
                // std::cout << std::fixed << std::hex << static_cast<unsigned>(head) << "*" << std::endl;
                uint32_t val = 0;
                if (not st)  break;
                if (head <= 32 or head == 127)  break;
                val = (val << 8) + head;
                int byte_cnt = (head >> 7? __builtin_clz(~static_cast<unsigned int>(head) << 24): 1);
                for (int i = 0; i < byte_cnt - 1; i++) {
                    unsigned char ch = st.get();
                    // std::cout << std::fixed << std::hex << static_cast<unsigned>(static_cast<unsigned char>(ch)) << "*" << std::endl;
                    assert(static_cast<unsigned char>(ch) >> 7);
                    val = (val << 8) + ch;
                }
                u_str.pushBack({val});
            }
            return st;
        }
    };
}

using namespace Test;

int main() {
    std::string s = "鸡cccπ";
    UnicodeString str;
    std::stringstream(s) >> str;
    size_t sz = str.size() * str.align();
    for (size_t i = 0; i < sz; i++) {
        char x = str.storage_ptr[i];
        std::cout << std::bitset<8>(x) << std::endl;
    }
    for (size_t i = 0; i < str.size(); i++) {
        std::cout << str[i].data << std::endl;
    }
    std::cout << str.size() << std::endl;
    return 0;
}