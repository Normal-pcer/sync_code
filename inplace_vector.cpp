#include <array>
#include <cstddef>
#include <format>
#include <stdexcept>

#ifndef INCLUDED_INPLACE_VECTOR
#define INCLUDED_INPLACE_VECTOR
template <typename T, std::size_t MaxSize>
class InplaceVector {
    alignas(alignof(T)) std::array<std::byte, MaxSize * sizeof(T)> data_;
    std::size_t size_ = 0;

public:
    using Iterator = T *;
    using ConstIterator = T const *;

    InplaceVector() = default;
    explicit InplaceVector(std::size_t n): size_(n) {
        auto ptr = reinterpret_cast<T *>(data_.data());
        for (std::size_t i = 0; i != n; ++i, ++ptr) {
            new(ptr) T;
        }
    }
    InplaceVector(std::size_t n, T const &value): size_(n) {
        auto ptr = reinterpret_cast<T *>(data_.data());
        for (std::size_t i = 0; i != n; ++i, ++ptr) {
            new(ptr) T(value);
        }
    }
    ~InplaceVector() {
        auto ptr = reinterpret_cast<T *>(data_.data());
        for (std::size_t i = 0; i != size_; ++i, ++ptr) {
            ptr->~T();
        }
    }
    
    auto size() const -> std::size_t { return size_; }
    auto data() const -> T * { return reinterpret_cast<T *>(data_.data()); }
    auto begin() -> Iterator { return reinterpret_cast<T *>(data_.data()); }
    auto begin() const -> ConstIterator { return reinterpret_cast<T *>(data_.data()); }
    auto end() -> Iterator { return reinterpret_cast<T *>(data_.data()) + size(); }
    auto end() const -> ConstIterator { return reinterpret_cast<T *>(data_.data()) + size(); }

    auto at(std::size_t index) const -> T const & {
        if (index >= size()) {
            throw std::out_of_range{
                std::format("InplaceVector access out of range. Accessing index {} (size = {}).", index, size())
            };
        }
        return *(reinterpret_cast<T const *>(data_.data()) + index);
    }
    auto at(std::size_t index) -> T & {
        if (index >= size()) {
            throw std::out_of_range{
                std::format("InplaceVector access out of range. Accessing index {} (size = {}).", index, size())
            };
        }
        return *(reinterpret_cast<T *>(data_.data()) + index);
    }

    auto pushBack(T const &x) -> void {
        if (size() >= MaxSize) {
            throw std::logic_error{
                std::format("InplaceVector failed to allocate memory.")
            };
        }

        T *pos = reinterpret_cast<T *>(data_.data()) + size();
        new(pos) T(x);
        ++size_;
    }
    auto pushBack(T &&x) -> void {
        if (size() >= MaxSize) {
            throw std::logic_error{
                std::format("InplaceVector failed to allocate memory.")
            };
        }

        T *pos = reinterpret_cast<T *>(data_.data()) + size();
        new(pos) T(std::move(x));
        ++size_;
    }
    auto friend operator== (InplaceVector const &lhs, InplaceVector const &rhs) -> bool {
        return lhs.size() == rhs.size() and std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }
    auto friend operator<=> (InplaceVector const &lhs, InplaceVector const &rhs) -> std::strong_ordering {
        return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
    auto operator[] (std::size_t index) const -> T const & { return at(index); }
    auto operator[] (std::size_t index) -> T & { return at(index); }
};
#endif  // def INCLUDED_INPLACE_VECTOR
