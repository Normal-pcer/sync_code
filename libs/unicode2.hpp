#ifndef UNICODE_STRING_HEADER
#define UNICODE_STRING_HEADER

#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <cstddef>
#include <format>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace unicode {

inline auto is_blank_char(char ch) -> bool {
    std::uint8_t x = ch;
    return x <= ' ' || x == '\x7f';
}

struct unicode_char {
    // 按照 UTF-8 编码存储一个 Unicode 字符（码点）
    // 低字节的位置在高位存储，但整体始终占据最低的 width 字节
    std::uint32_t data{};

    std::size_t static constexpr max_width = sizeof(std::uint32_t);

    constexpr unicode_char() = default;
    explicit constexpr unicode_char(std::uint32_t data) : data(data) {}

    // 初始化一个 ascii 字符，允许隐式转换
    constexpr unicode_char(char data) : data(data) {}

    // 从 char32_t 转换，允许隐式转换
    constexpr unicode_char(char32_t data) {
        *this = chr(static_cast<std::uint32_t>(data));  // 直接作为 Unicode 编码转换
    }

    auto constexpr width() const -> std::size_t {
        if (data == 0) return 1;

        auto clz = std::countl_zero(data);
        return sizeof(std::uint32_t) - (clz >> 3);
    }

    // 因为 UTF-8 编码处理需要较多计算，为了方便，使用 std::uint8_t 而不是 std::byte
    auto constexpr to_bytes() const -> std::array<std::uint8_t, max_width> {
        std::array<std::uint8_t, max_width> bytes{};
        auto top = bytes.end();

        for (auto x = data; x != 0; x >>= 8) {
            *--top = x & 0xFF;
        }

        return bytes;
    }

    // 转换为 Unicode 编码
    // 由 o4-mini 生成
    auto constexpr ord() const -> std::uint32_t {
        std::uint32_t constexpr bad_char = 0xfffd;

        auto bytes = to_bytes();
        std::size_t n = width();
        if (n == 0 || n > max_width) return bad_char; // 不支持 0 或 >4 字节

        std::size_t start = max_width - n;
        auto get = [&](std::size_t i) -> std::uint8_t {
            return bytes[start + i];
        };

        // 单字节
        std::uint8_t b0 = get(0);
        if (n == 1) {
            if ((b0 & 0x80) == 0) return b0;
            return bad_char;
        }

        if (n == 2) {
            if ((b0 >> 5U) != 0b110U) return bad_char; // 110xxxxx
        } else if (n == 3) {
            if ((b0 >> 4U) != 0b1110U) return bad_char; // 1110xxxx
        } else { // n == 4
            if ((b0 >> 3U) != 0b11110U) return bad_char; // 11110xxx
        }

        std::uint32_t res = 0;

        // 首字节保留有效位
        auto first_mask = std::uint8_t((1U << (7 - n)) - 1);
        res = static_cast<std::uint32_t>(b0 & first_mask);

        for (std::size_t i = 1; i < n; ++i) {
            std::uint8_t bi = get(i);
            if ((bi >> 6U) != 0b10) return bad_char; // 续字节必须是 10xxxxxx
            res = (res << 6) | static_cast<std::uint32_t>(bi & 0x3fU);
        }

        return res;
    }

    // 工厂函数，从 Unicode 编码获取字符，ord() 的逆运算
    // 由 o4-mini 生成
    auto static constexpr chr(std::uint32_t code) -> unicode_char {
        // 找到需要的字节数 n（1..4）
        constexpr std::array<std::uint32_t, 4> limits = { 0x80U, 0x800U, 0x10000U, 0x110000U };
        std::size_t n = 1;
        for (; n <= 4; ++n) {
            if (code < limits[n - 1]) break;
        }

        // 首字节前缀（按 n 索引）
        constexpr std::array<std::uint8_t, 5> prefix = { 0, 0x00U, 0xc0U, 0xe0U, 0xf0U };

        std::array<std::uint8_t, 4> bytes{};

        // 填充续字节（从后向前）
        for (std::size_t i = n; i --> 1; ) {
            bytes[i] = static_cast<std::uint8_t>(0x80U | (code & 0x3fU));
            code >>= 6;
        }

        // 首字节（剩余的 cp 放在首字节低位，再加上前缀）
        bytes[0] = static_cast<std::uint8_t>(prefix[n] | static_cast<std::uint8_t>(code));

        // 打包为整数
        std::uint32_t data = 0;
        for (std::size_t i = 0; i < n; ++i) {
            data = (data << 8) | bytes[i];
        }
        return unicode_char{data};
    }

    auto operator== (unicode_char const &other) const -> bool {
        return ord() == other.ord();
    }

    auto operator<=> (unicode_char const &other) const -> std::strong_ordering {
        return ord() <=> other.ord();
    }

    // 重载的输入输出
    friend auto operator<< (std::ostream &os, unicode_char const &ch) -> auto & {
        std::array<std::uint8_t, sizeof(std::uint32_t)> bytes;
        auto top = bytes.end();

        for (auto data = ch.data; data != 0; data >>= 8) {
            *--top = data & 0xFF;
        }

        for (; top != bytes.end(); ++top) {
            os << static_cast<char>(*top);
        }
        return os;
    }

    friend auto operator>> (std::istream &is, unicode_char &ch) -> std::istream & {
        // 获取第一个字节
        char cur{};

        // 跳过空白字符
        for (is.get(cur); is_blank_char(cur); is.get(cur)) {
            std::cerr << "got char " << std::uint32_t(std::uint8_t(cur)) << std::endl;
            if (!is) return is; // 读取失败
        }

        ch.take_from_stream_(is, cur);
        return is;
    }

    auto take_from_stream_(std::istream &is, char first_ch) -> std::istream & {
        std::uint8_t first = first_ch;
        std::uint32_t val = first; // 完整字符的值

        char cur{};
        std::size_t byte_cnt = 1;
        if ((first >> 7) != 0) {
            // 最高位有几个“1”
            byte_cnt = std::countl_zero(std::uint8_t(~first));
        }

        for (std::size_t i = 1; i != byte_cnt; ++i) {
            if (!is.get(cur)) return is;
            // 后续字节一定是“10xxxxxx”
            std::uint8_t this_byte = cur;
            if ((this_byte >> 6) != 0b10) {
                return is.setstate(std::ios::failbit), is;
            }
            val = (val << 8) | this_byte;
        }

        data = val;

        return is;
    }
};

template <typename T>
concept char_or_char8 = std::is_same_v<T, char> || std::is_same_v<T, char8_t>;

class unicode_string_mut_wrapper;

// Unicode 字符串
class unicode_string {
    // 存储原理：
    // 内部使用 std::vector<std::uint8_t> 存储原始字节。
    // 将每个“对齐”到最宽的一个字符，如果本身长度不够，就填充 0。
    // 例如，“abc”占用 3 字节，“abc中”占用 12 字节。
private:
    std::vector<std::uint8_t> data_;
    std::size_t align_ = 1;
    std::size_t size_ = 0;

public:
    using value_type = unicode_char;
    struct const_iterator;
    struct iterator;

    unicode_string() = default;
    unicode_string(std::string const &str) {
        std::istringstream ss(str);
        from_istream_(ss, false);
    }
    unicode_string(std::u8string const &str)
        : unicode_string(u8string_to_string_(str)) {}
    unicode_string(char const *str)
        : unicode_string(std::string(str)) {}
    unicode_string(char8_t const *str)
        : unicode_string(std::u8string(str)) {}
    template <typename InputIt>
    unicode_string(InputIt first, InputIt last) {
        using value_type = typename std::iterator_traits<InputIt>::value_type;

        if constexpr (std::is_same_v<value_type, unicode_char>) {
            using category = typename std::iterator_traits<InputIt>::iterator_category;
            constexpr bool is_forward = std::is_base_of_v<std::forward_iterator_tag, category>;

            if constexpr (is_forward) {
                std::size_t size = 0, max_width = 1;
                for (auto it = first; it != last; ++it) {
                    ++size;
                    max_width = std::max(max_width, (*it).width());
                }
                data_.reserve(size * max_width);
            }

            for (auto it = first; it != last; ++it) {
                emplace_back(*it);
            }
        } else if constexpr (char_or_char8<value_type>) {
            *this = std::basic_string<value_type>(first, last);
        } else {
            static_assert(sizeof(InputIt) == 0);  // 触发编译错误
        }
    }

    std::size_t static constexpr max_align = 4;

    auto constexpr size() const -> std::size_t { return size_; }
    auto constexpr align() const -> std::size_t { return align_; }
    auto constexpr empty() const -> bool { return size() == 0; }

    // 防止误用基于 proxy 的迭代器，导致类似 vector<bool> 的隐式 bug
    // 这种迭代器必须显式触发（mut_begin, mut_end 或者 to_mut）
    auto constexpr mut_begin() -> iterator;
    auto constexpr mut_end() -> iterator;
    auto constexpr begin() const -> const_iterator;
    auto constexpr end() const -> const_iterator;

    auto constexpr access_at(std::size_t index) const -> unicode_char {
        std::uint32_t val = 0;
        std::size_t offset = index * align();

        for (std::size_t i = 0; i != align(); ++i) {
            auto x = static_cast<std::uint8_t>(data_[i + offset]);
            val = val << 8 | x;
        }

        return unicode_char{val};
    }

    auto constexpr assign_at(std::size_t index, unicode_char ch) -> void {
        if (index >= size()) throw std::out_of_range("index out of range");

        auto width = ch.width();
        if (width > align_) {
            align_to_(width);
        }

        auto bytes = ch.to_bytes();
        auto in = bytes.end() - ch.width();
        auto out = data_.begin() + std::ptrdiff_t(index * align());
        
        auto padding = align() - ch.width();
        out = std::ranges::fill_n(out, std::ptrdiff_t(padding), 0);
        std::ranges::copy(in, bytes.end(), out);
    }

    auto constexpr emplace_back(unicode_char ch) -> void {
        auto width = ch.width();
        if (width > align_) {
            align_to_(width);
        }

        // 插入一个空字符再赋值
        for (std::size_t i = 0; i != align(); ++i) {
            data_.emplace_back();
        }

        ++size_;
        assign_at(size_ - 1, ch);
    }

    auto constexpr push_back(unicode_char ch) -> void {
        emplace_back(ch); // 对于 unicode_char，二者等价
    }

    auto constexpr swap(unicode_string &other) noexcept -> void {
        using std::swap;
        swap(data_, other.data_);
        swap(align_, other.align_);
        swap(size_, other.size_);
    }
    auto constexpr friend swap(unicode_string &a, unicode_string &b) noexcept -> void {
        // ADL swap
        a.swap(b);
    }
    auto constexpr reserve(std::size_t size) -> void {
        data_.reserve(size * align_);
    }
    auto friend operator<< (std::ostream &os, unicode_string const &str) -> std::ostream & {
        for (std::size_t i = 0; i != str.size(); ++i) {
            os << str.access_at(i);
        }
        return os;
    }
    auto friend operator>> (std::istream &is, unicode_string &str) -> std::istream & {
        str.from_istream_(is, true);
        return is;
    }
    auto constexpr operator[] (std::size_t index) const -> unicode_char {
        return access_at(index);
    }
    auto constexpr string() const -> std::string {
        return to_basic_string_<char>();
    }
    auto constexpr u8string() const -> std::u8string {
        return to_basic_string_<char8_t>();
    }
    auto constexpr to_mut() -> unicode_string_mut_wrapper;
    auto debug_() -> void {
        for (auto x: data_) {
            std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(x) << ' ';
        }
        std::cout << std::endl;
    }

    auto constexpr operator== (unicode_string const &other) const -> bool;
    auto constexpr operator<=> (unicode_string const &other) const -> std::strong_ordering;
private:
    bool static windows_init_;

    auto constexpr align_to_(std::size_t new_align) -> void {
        if (new_align <= align_) return;
        std::vector<std::uint8_t> new_data(new_align * size_);
        auto padding = std::ptrdiff_t(new_align - align_);

        auto in = data_.begin();
        auto out = new_data.begin();

        while (in != data_.end()) {
            std::ranges::copy_n(in, std::ptrdiff_t(align_), out + padding);
            in += std::ptrdiff_t(align_);
            out += std::ptrdiff_t(new_align);
        }

        data_ = std::move(new_data);
        align_ = new_align;
    }

    auto from_istream_(std::istream &is, bool auto_end) -> std::istream & {
        char ch{};
        for (is.get(ch); is_blank_char(ch); is.get(ch)) {
            if (!is) return is;
        }

        while (true) {
            if (auto_end && is_blank_char(ch)) break;
            if (ch == '\0') break;
            
            unicode_char unicode_ch{};
            unicode_ch.take_from_stream_(is, ch);
            emplace_back(unicode_ch);

            if (!is.get(ch)) return is;
        }

        return is;
    }

    template <typename T>
    auto constexpr to_basic_string_() const -> std::basic_string<T> {
        static_assert(std::is_same_v<T, char> || std::is_same_v<T, char8_t>);

        if (align() == 1) {
            // 可以直接逐个字符处理
            auto begin = reinterpret_cast<T const *>(data_.data());
            auto end = begin + size();

            return std::basic_string<T>(begin, end);
        }

        std::basic_string<T> res;
        res.reserve(size() * align());

        for (std::size_t i = 0; i != size(); ++i) {
            auto ch = access_at(i);
            auto bytes = ch.to_bytes();
            auto width = ch.width();

            for (auto it = bytes.end() - width; it != bytes.end(); ++it) {
                res.push_back(static_cast<T>(*it));
            }
        }

        return res;
    }

    auto static u8string_to_string_(std::u8string const &str) -> std::string {
        auto p0 = reinterpret_cast<char const *>(str.data());
        auto p1 = p0 + str.size();
        return {p0, p1};
    }
};

struct unicode_string::const_iterator {
    // 常量迭代器，解引用直接返回 unicode_char 右值
    using value_type = unicode_string::value_type;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;
    using reference = value_type;  // 解引用返回右值
    using pointer = void;

    std::size_t index = 0;
    unicode_string const *str_ptr = nullptr;

    auto operator* () const -> value_type {
        return str_ptr->access_at(index);
    }
    auto operator++ () -> const_iterator & { return ++index, *this; }
    auto operator-- () -> const_iterator & { return --index, *this; }
    auto operator++ (int) -> const_iterator {
        auto tmp = *this;
        return ++index, tmp;
    }
    auto operator-- (int) -> const_iterator {
        auto tmp = *this;
        return --index, tmp;
    }
    auto operator+= (difference_type n) -> const_iterator & { return index += n, *this; }
    auto operator-= (difference_type n) -> const_iterator & { return index -= n, *this; }
    auto operator+ (difference_type n) const -> const_iterator {
        auto res = *this;
        return res += n;
    }
    auto operator- (difference_type n) const -> const_iterator {
        auto res = *this;
        return res -= n;
    }
    auto operator- (const_iterator const &other) const -> difference_type {
        return difference_type(index) - difference_type(other.index);
    }
    auto operator== (const_iterator const &other) const -> bool {
        return index == other.index;
    }
    auto operator<=> (const_iterator const &other) const -> std::strong_ordering {
        return index <=> other.index;
    }
    auto operator[] (difference_type n) const -> value_type {
        return str_ptr->access_at(index + n);
    }
    auto friend operator+ (difference_type n, const_iterator const &it) -> const_iterator {
        return it + n;
    }
};

struct unicode_string::iterator {
    // 迭代器，基于代理类（proxy）实现
    using value_type = unicode_string::value_type;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;
    struct reference {
        std::size_t index = 0;
        unicode_string *str_ptr = nullptr;

        reference() = default;
        reference(std::size_t idx, unicode_string *ptr) : index(idx), str_ptr(ptr) {}
        reference(const reference&) = default;

        auto get() const -> unicode_char {
            return str_ptr->access_at(index);
        }
        operator unicode_char () const {
            return str_ptr->access_at(index);
        }
        auto operator= (unicode_char ch) -> reference & {
            str_ptr->assign_at(index, ch);
            return *this;
        }
        auto operator= (reference const &other) -> reference & {
            if (std::addressof(other) == this) return *this;
            str_ptr->assign_at(index, other);
            return *this;
        }
        auto operator= (unicode_char ch) const -> reference const & {  // NOLINT
            str_ptr->assign_at(index, ch);
            return *this;
        }
        auto operator= (reference const &other) const -> reference const & {  // NOLINT
            if (std::addressof(other) == this) return *this;
            str_ptr->assign_at(index, other);
            return *this;
        }
        // 转发部分可能用到的操作
        auto friend swap(reference a, reference b) -> void {
            unicode_char tmp = a;
            a = b;
            b = tmp;
        }
        auto operator== (reference const &other) const -> bool {
            return unicode_char(*this) == unicode_char(other);
        }
        auto operator<=> (reference const &other) const -> std::strong_ordering {
            return unicode_char(*this) <=> unicode_char(other);
        }
    };
    using pointer = void;

    std::size_t index = 0;
    unicode_string *str_ptr = nullptr;

    auto operator* () const -> reference {
        return reference{index, str_ptr};
    }
    auto operator++ () -> iterator & { return ++index, *this; }
    auto operator-- () -> iterator & { return --index, *this; }
    auto operator++ (int) -> iterator {
        auto tmp = *this;
        return ++index, tmp;
    }
    auto operator-- (int) -> iterator {
        auto tmp = *this;
        return --index, tmp;
    }
    auto operator+= (difference_type n) -> iterator & { return index += n, *this; }
    auto operator-= (difference_type n) -> iterator & { return index -= n, *this; }
    auto operator+ (difference_type n) const -> iterator {
        auto res = *this;
        return res += n;
    }
    auto operator- (difference_type n) const -> iterator {
        auto res = *this;
        return res -= n;
    }
    auto operator- (iterator const &other) const -> difference_type {
        return difference_type(index) - difference_type(other.index);
    }
    auto operator== (iterator const &other) const -> bool {
        return index == other.index;
    }
    auto operator<=> (iterator const &other) const -> std::strong_ordering {
        return index <=> other.index;
    }
    auto operator[] (difference_type n) const -> reference {
        return reference{index + n, str_ptr};
    }
    auto friend operator+ (difference_type n, iterator const &it) -> iterator {
        return it + n;
    }

    auto friend iter_swap(iterator a, iterator b) -> void {
        value_type tmp = *a;
        *a = *b;
        *b = tmp;
    }
};

#ifdef _WIN32
namespace Windows {
    extern "C" {
        __declspec(dllimport) int __stdcall SetConsoleCP(unsigned int);
        __declspec(dllimport) int __stdcall SetConsoleOutputCP(unsigned int);

        __declspec(dllimport) int __stdcall MultiByteToWideChar(
            unsigned int CodePage, 
            unsigned long dwFlags,
            const char* lpMultiByteStr,
            int cbMultiByte,
            wchar_t* lpWideCharStr,
            int cchWideChar
        );
        
        __declspec(dllimport) int __stdcall WideCharToMultiByte(
            unsigned int CodePage,
            unsigned long dwFlags,
            const wchar_t* lpWideCharStr,
            int cchWideChar,
            char* lpMultiByteStr,
            int cbMultiByte,
            const char* lpDefaultChar,
            int* lpUsedDefaultChar
        );
    }

    auto inline set_console_utf8() noexcept -> bool {
        return SetConsoleCP(65001) != 0 && SetConsoleOutputCP(65001) != 0;
    }

    auto inline ansi_to_utf8(std::string const &ansi_str) -> std::string {
        unsigned int constexpr CP_ACP = 0;
        unsigned int constexpr CP_UTF8 = 65001;
        int wlen = MultiByteToWideChar(CP_ACP, 0, ansi_str.c_str(), -1, nullptr, 0);
        if (wlen <= 0) return "";

        std::wstring wbuf(wlen, L'\0');
        MultiByteToWideChar(CP_ACP, 0, ansi_str.c_str(), -1, wbuf.data(), wlen);

        int u8len = WideCharToMultiByte(CP_UTF8, 0, wbuf.data(), -1, nullptr, 0, nullptr, nullptr);

        std::string u8buf(u8len, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wbuf.data(), -1, u8buf.data(), u8len, nullptr, nullptr);
        return u8buf;
    }
} // namespace Windows
#endif
// 主函数开始前进行初始化
inline bool unicode_string::windows_init_ = [] {
#ifdef _WIN32
    Windows::set_console_utf8();    
#endif
    return true;
}();

namespace unicode_literals {
    // 后缀运算符
    auto constexpr operator""_utf8(char const *str, std::size_t len) -> unicode_string {
        return {std::string(str, len)};
    }
    auto constexpr operator""_utf8(char8_t const *str, std::size_t len) -> unicode_string {
        return {std::u8string(str, len)};
    }
    auto constexpr operator""_ansi(char const *str, std::size_t len) -> unicode_string {
    #ifdef _WIN32
        return {Windows::ansi_to_utf8(std::string(str, len))};
    #else
        return {std::string(str, len)}; // 假设其他平台不会出现非 UTF-8 编码
    #endif
    }
}

auto constexpr unicode_string::mut_begin() -> iterator {
    return {0, this};
}
auto constexpr unicode_string::mut_end() -> iterator {
    return {size(), this};
}
auto constexpr unicode_string::begin() const -> const_iterator {
    return {0, this};
}
auto constexpr unicode_string::end() const -> const_iterator {
    return {size(), this};
}

auto constexpr unicode_string::operator== (unicode_string const &other) const -> bool {
    return std::ranges::equal(*this, other);
}
auto constexpr unicode_string::operator<=> (unicode_string const &other) const -> std::strong_ordering {
    return std::lexicographical_compare_three_way(begin(), end(), other.begin(), other.end());
}

class unicode_string_mut_wrapper {
    unicode_string *str_ptr = nullptr;
public:
    unicode_string_mut_wrapper(unicode_string *ptr) : str_ptr(ptr) {}

    // begin() 和 end() 自动调用 mut 版本
    auto begin() -> unicode_string::iterator {
        return str_ptr->mut_begin();
    }
    auto end() -> unicode_string::iterator {
        return str_ptr->mut_end();
    }
};

auto constexpr unicode_string::to_mut() -> unicode_string_mut_wrapper {
    return {this};
}

} // namespace unicode
// 进行 unicode 的相关模板特化
namespace std {
template <>
struct hash<unicode::unicode_char> {
    auto operator() (unicode::unicode_char const &ch) const -> std::size_t {
        return ch.data;
    }
};

template <>
struct hash<unicode::unicode_string> {
    auto operator() (unicode::unicode_string const &str) const -> std::size_t {
        std::size_t res = 0;
        std::size_t constexpr base = 1e9 + 7;
        for (auto ch: str) {
            res = res * base + std::hash<unicode::unicode_char>{}(ch);
        }
        return res;
    }
};

template <>
struct formatter<unicode::unicode_string, char> {
    template <typename Context>
    constexpr auto parse(Context &ctx) {
        auto it = ctx.begin();
        while (it != ctx.end() && *it != '}') ++it;
        return it;
    }

    template <typename Context>
    auto format(unicode::unicode_string const &s, Context &ctx) const {
        std::stringstream ss;
        ss << s;
        return ranges::copy(std::move(ss).str(), ctx.out()).out;
    }
};
} // namespace std

#endif // UNICODE_STRING_HEADER
