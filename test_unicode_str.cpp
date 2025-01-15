#include <bits/stdc++.h>

namespace Test {
    static constexpr auto clz(unsigned int x) -> unsigned int {
        return __builtin_clz(x);
    }

    template <typename T>
    struct UnicodeCharBase {
        // 按照 UTF-8 编码
        // 低字节位置在高位存储，但始终占据低 8*size 位
        T data;
        UnicodeCharBase(): data(0) {}
        UnicodeCharBase(T x): data(x) {}
        UnicodeCharBase(char x): data(x) {}
        template <typename U>
        UnicodeCharBase(UnicodeCharBase<U> x): data(x.data) {
            if constexpr (sizeof(U) > sizeof(T)) {
                assert(x.size() <= sizeof(U));
            }
        }
        auto size() const -> size_t {
            unsigned int x = clz(data);
            if (x == 0U)  return 1;
            else  return 4 - (x >> 3);
        }

        auto operator<=>(UnicodeCharBase<T> const &) const noexcept = default;
        auto operator<=>(char const &x) const noexcept {
            return data <=> x;
        }
        friend auto operator<< (std::ostream &st, UnicodeCharBase const &ch) -> std::ostream & {
            T data = ch.data;
            std::array<char, 4> bytes;
            for (int i = 0; i < 4; i++) {
                bytes[3 - i] = data & 255U;
                data >>= 8;
            }
            for (int i = 4 - ch.size(); i < 4; i++) {
                st << bytes[i];
            }
            return st;
        }
    };
    using UnicodeChar = UnicodeCharBase<uint32_t>;

    struct UnicodeStringAccessor;
    struct UnicodeString {
        struct iterator {
            using value_type = UnicodeChar;
            using difference_type = ptrdiff_t;
            using iterator_category = std::forward_iterator_tag;
            size_t index = 0;
            UnicodeString const *str_ptr = nullptr;
            auto operator* () const -> UnicodeChar {
                return (*str_ptr)[index];
            }
            auto operator++ () noexcept -> iterator & { return ++index, *this; }
            auto operator++ (int) noexcept -> iterator {
                iterator tmp = *this;
                index++;
                return tmp;
            }
            auto operator-- () noexcept -> iterator & { return --index, *this; }
            auto operator-- (int) noexcept -> iterator {
                iterator tmp = *this;
                index--;
                return tmp;
            }
            auto operator- (iterator other) const noexcept -> ptrdiff_t { return index - other.index; }
            auto operator+= (size_t other) noexcept -> iterator & { return index += other, *this; }
            auto operator+ (size_t other) const noexcept -> iterator { iterator res = *this; return res += other; }
            auto operator-= (size_t other) noexcept -> iterator & { return index -= other, *this; }
            auto operator- (size_t other) const noexcept -> iterator { iterator res = *this; return res -= other; }
            // auto operator<=> (iterator const &other) const noexcept -> std::strong_ordering {
            //     if (auto cmp = index <=> other.index; cmp != 0)  return cmp;
            //     return str_ptr <=> other.str_ptr;
            // }
            auto operator<=> (iterator const &) const noexcept = default;
            auto operator[] (size_t idx) const -> UnicodeChar {
                return *(*this + idx);
            }
        };
        size_t _align = 1;              // 对齐长度
        size_t _size = 0;               // 字符数量
        size_t _capacity = 0;           // base 数组能存储的字符数
        char *storage_ptr = nullptr;    // 存储区域，按照 UTF-8 编码（用 0 补全到 align）

        static constexpr const int MaxAlign = 4;
        UnicodeString() {}
        UnicodeString(const UnicodeString &other): _align(other.align()), _size(other.size()), _capacity(other.capacity()) {
            size_t storage_bytes = other.capacity() * other.align();
            storage_ptr = new char[storage_bytes];
            std::memcpy(storage_ptr, other.storage_ptr, storage_bytes);  // 复制字符串内容
        }
        UnicodeString(UnicodeString &&other) noexcept {
            swap(other);
        }
        ~UnicodeString() {
            delete[] storage_ptr;  // 删除存储区
        }

        auto end() const noexcept -> iterator;
        auto size() const noexcept -> size_t { return _size; }
        auto data() -> char *;
        auto swap(UnicodeString &other) noexcept -> void {
            std::swap(_align, other._align);
            std::swap(_size, other._size);
            std::swap(_capacity, other._capacity);
            std::swap(storage_ptr, other.storage_ptr);
        }
        auto begin() const noexcept -> iterator { return iterator{static_cast<size_t>(0), this}; }
        auto align() const noexcept -> size_t { return _align; }
        auto assign(size_t pos, UnicodeChar ch) -> void {
            auto char_size = ch.size();
            if (char_size > align()) {
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
            std::memcpy(&storage_ptr[pos * align()], bytes.begin(), align());
        }
        auto capacity() const noexcept -> size_t { return _capacity; }
        auto pushBack(UnicodeChar ch) -> void {
            auto char_size = ch.size();
            if (char_size > align()) {
                // 计算新的缓冲区大小
                size_t new_align = char_size;  // 对齐到当前字符宽度
                size_t new_storage_bytes = new_align * capacity();  // 存储区的字符数量不变
                char *new_storage_ptr = new char[new_storage_bytes];  // 新的存储区指针
                // std::memset(new_storage_ptr, 0, new_storage_bytes);
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
                // std::memset(new_storage_ptr, 0, new_storage_bytes);
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

        auto operator[] (size_t pos) const noexcept -> UnicodeChar {
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
                uint32_t val = 0;
                if (not st)  break;
                if (head <= 32 or head == 127)  break;
                val = (val << 8) + head;
                int byte_cnt = (head >> 7? __builtin_clz(~static_cast<unsigned int>(head) << 24): 1);
                for (int i = 0; i < byte_cnt - 1; i++) {
                    unsigned char ch = st.get();
                    assert(static_cast<unsigned char>(ch) >> 7);
                    val = (val << 8) + ch;
                }
                u_str.pushBack({val});
            }
            return st;
        }
        friend auto operator<< (std::ostream &st, UnicodeString const &u_str) -> std::ostream & {
            for (auto ch: u_str)  st << ch;
            return st;
        }
    };
    auto UnicodeString::end() const noexcept -> UnicodeString::iterator {
        return {size(), this};
    }
    // 返回存储区，为一个合法的 C 风格字符串（\0 结尾）
    // 如果 align() 不为 1，则行为未定义
    auto UnicodeString::data() -> char * {
        assert(align() == static_cast<size_t>(1));
        if (size() >= capacity()) {
            size_t new_capacity = capacity()? capacity() << 1: 1;  // 新的存储区字符数
            size_t new_storage_bytes = new_capacity * align();
            size_t data_bytes = size() * align();
            char *new_storage_ptr = new char[new_storage_bytes];
            if (storage_ptr != nullptr) {
                std::memcpy(new_storage_ptr, storage_ptr, data_bytes);
            }
            _capacity = new_capacity;
            delete[] storage_ptr, storage_ptr = new_storage_ptr;
        }
        storage_ptr[size()] = '\0';
        return storage_ptr;
    }

    auto operator""_utf8(const char *s, size_t) -> UnicodeString {
        UnicodeString res;
        std::istringstream st{s};
        while (true) {
            char c = st.get();
            unsigned char head = c;
            uint32_t val = 0;
            if (not st)  break;
            val = (val << 8) + head;
            int byte_cnt = (head >> 7? __builtin_clz(~static_cast<unsigned int>(head) << 24): 1);
            for (int i = 0; i < byte_cnt - 1; i++) {
                unsigned char ch = st.get();
                assert(static_cast<unsigned char>(ch) >> 7);
                val = (val << 8) + ch;
            }
            res.pushBack({val});
        }
        return res;
    }
}

template <>
struct std::iterator_traits<Test::UnicodeString::iterator> {
    using Iter = Test::UnicodeString::iterator;
    using difference_type = Iter::difference_type;
    using value_type = Iter::value_type;
    using iterator_category = Iter::iterator_category;
};

using namespace Test;

int main() {
    std::vector lines { 
        "做个文明中国人"_utf8,
        "长江 黄河 还有黑龙江"_utf8,
        "男人 女人 还有变性人"_utf8,
        "拉屎不洗手 根本不是人"_utf8,
        "随地吐痰 吐中爸爸有可能"_utf8,
        "塞车 插队"_utf8,
        "他妈的过分"_utf8,
        "电梯抽烟"_utf8,
        "小心性无能"_utf8,
        "下雨收衣服"_utf8,
        "上下爽一爽"_utf8,
        "不要逃税 我们共建新香港"_utf8
    };
    std::mt19937 rnd{745184};
    for (auto &line: lines) {
        std::vector<UnicodeChar> chars(line.begin(), line.end());
        std::vector<UnicodeChar> no_space(chars);
        std::erase(no_space, ' ');
        std::shuffle(no_space.begin(), no_space.end(), rnd);
        int i = 0;
        for (auto &x: chars) {
            if (x == ' ')  continue;
            x = no_space[i], i++;
        }
        for (auto ch: chars)  std::cout << ch;
        std::cout << std::endl;
    }
    return 0;
}