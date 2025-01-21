#include <compare>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <random>

namespace Test {
    template <typename T>
    class Deque {
        template <typename ContainerType, typename ItemType>
        struct IteratorBase;
        using Iterator = IteratorBase<Deque<T>, T>;
        using ConstIterator = IteratorBase<Deque<T> const, T const>;
        T *_storage_begin_ptr = nullptr;    // 存储区的头指针
        T *_data_begin_ptr = nullptr;       // 数据头指针
        size_t _capacity = 0;               // 存储区元素数量
        size_t _size = 0;                   // 实际元素数量

        auto dataEndPtr() const noexcept -> T *;
        auto dataBeginPtr() const noexcept -> T *;
        auto storageEndPtr() const noexcept -> T *;
        auto storageBeginPtr() const noexcept -> T *;
    public:
        Deque() noexcept;
        Deque(Deque &&) noexcept;
        Deque(Deque const &);
        ~Deque() noexcept;

        auto end() noexcept -> Iterator;
        auto end() const noexcept -> ConstIterator;
        auto size() const noexcept -> size_t;
        auto swap(Deque &) noexcept -> void;
        auto clear() -> void;
        auto begin() noexcept -> Iterator;
        auto begin() const noexcept -> ConstIterator;
        auto popBack() -> void;
        auto capacity() const noexcept -> size_t;
        auto popFront() -> void;
        auto pushBack(T const &) -> void;
        auto pushFront(T const &) -> void;
        auto operator= (Deque &&) noexcept -> Deque &;
        auto operator= (Deque const &) -> Deque &;
        auto operator[] (size_t) -> T &;
        auto operator[] (size_t) const -> T const &;
    };
    
    template <typename T>
    template <typename ContainerType, typename ItemType>
    struct Deque<T>::IteratorBase {
        using Self = Deque<T>::IteratorBase<ContainerType, ItemType>;
        // 标准库迭代器要求的接口
        using difference_type = ptrdiff_t;
        using value_type = T;
        using reference = ItemType &;
        using pointer = ItemType *;
        using iterator_category = std::random_access_iterator_tag;
        ContainerType *deque_ptr;
        size_t index;

        auto operator* () const -> ItemType & { return (*deque_ptr)[index]; }
        auto operator+ (size_t x) const noexcept -> Self { auto res = *this; return res += x; }
        auto operator- (size_t x) const noexcept -> Self { auto res = *this; return res -= x; }
        auto operator- (Self other) const noexcept -> ptrdiff_t { return index - other.index; }
        auto operator+= (size_t x) noexcept -> Self & { return index += x, *this; }
        auto operator-= (size_t x) noexcept -> Self & { return index -= x, *this; }
        auto operator++ () noexcept -> Self & { return ++index, *this; }
        auto operator++ (int) noexcept -> Self { Self res = *this; return index++, res; } 
        auto operator-- () noexcept -> Self & { return --index, *this; }
        auto operator-- (int) noexcept -> Self { Self res = *this; return index--, res; }
        auto operator== (Self other) const noexcept { return index == other.index; }
        auto operator<=> (Self other) const noexcept { return index <=> other.index; }
    };

    template <typename T>
    auto Deque<T>::storageBeginPtr() const noexcept -> T * {
        return _storage_begin_ptr;
    }
    template <typename T>
    auto Deque<T>::storageEndPtr() const noexcept -> T * {
        return storageBeginPtr() + capacity();
    }
    template <typename T>
    auto Deque<T>::dataBeginPtr() const noexcept -> T * {
        return _data_begin_ptr;
    }
    template <typename T>
    auto Deque<T>::dataEndPtr() const noexcept -> T * {
        T *ptr = dataBeginPtr() + size();
        if (ptr > storageEndPtr())  ptr -= capacity();
        return ptr;
    }
    template <typename T>
    Deque<T>::Deque() noexcept {}
    template <typename T>
    Deque<T>::Deque(Deque const &other) {
        size_t bytes = other.capacity() * sizeof(T);
        T *new_storage = reinterpret_cast<T *>(std::malloc(bytes));
        if (new_storage == nullptr)  throw std::bad_alloc{};
        T *ptr = dataBeginPtr();
        for (size_t i = 0; i != size(); i++, ptr++) {
            if (ptr == storageEndPtr())  ptr = storageBeginPtr();
            new (new_storage + i) T(*ptr);
        }
        _storage_begin_ptr = _data_begin_ptr = new_storage;
        _capacity = other.capacity(), _size = other.size();
    }
    template <typename T>
    Deque<T>::Deque(Deque &&other) noexcept {
        swap(other);
    }
    template <typename T>
    Deque<T>::~Deque() noexcept {
        clear();
        std::free(_storage_begin_ptr);
    }
    template <typename T>
    auto Deque<T>::size() const noexcept -> size_t {
        return _size;
    }
    template <typename T>
    auto Deque<T>::capacity() const noexcept -> size_t {
        return _capacity;
    }
    template <typename T>
    auto Deque<T>::operator[](size_t idx) const -> T const & {
        auto ptr = dataBeginPtr() + idx;
        if (ptr > storageEndPtr())  ptr -= capacity();
        return *ptr;
    }
    template <typename T>
    auto Deque<T>::operator[](size_t idx) -> T & {
        auto ptr = dataBeginPtr() + idx;
        if (ptr > storageEndPtr())  ptr -= capacity();
        return *ptr;
    }
    template <typename T>
    auto Deque<T>::pushBack(T const &x) -> void {
        // 如果当前存储区已满
        if (size() >= capacity()) {
            // 进行一次扩容
            size_t new_capacity = (capacity() == 0? 1: capacity() << 1);
            T *new_storage = reinterpret_cast<T *>(std::malloc(new_capacity * sizeof(T)));  // 新的存储区
            if (new_storage == nullptr)  throw std::bad_alloc{};  // 内存分配失败
            // 重新整理存储区
            // |********|
            //        ^ data_begin
            // |********........|
            //  ^ new_data_begin
            T *ptr = dataBeginPtr();
            for (size_t i = 0; i != size(); i++, ptr++) {
                if (ptr == storageEndPtr())  ptr = storageBeginPtr();
                new (new_storage + i) T(std::move(*ptr));
            }
            std::free(_storage_begin_ptr);
            _capacity = new_capacity;
            _storage_begin_ptr = _data_begin_ptr = new_storage;
        }
        T *new_ptr = dataEndPtr();
        if (new_ptr == storageEndPtr())  new_ptr = storageBeginPtr();
        new (new_ptr) T(x);
        _size++;
    }
    template <typename T>
    auto Deque<T>::popBack() -> void {
        T *ptr = dataEndPtr();
        if (ptr == storageBeginPtr())  ptr = storageEndPtr();
        ptr--;  // 此时 ptr 指向最后一个元素
        ptr->~T(), _size--;
    }
    template <typename T>
    auto Deque<T>::pushFront(T const &x) -> void {
        if (size() >= capacity()) {
            size_t new_capacity = (capacity() == 0? 1: capacity() << 1);
            T *new_storage = reinterpret_cast<T *>(std::malloc(new_capacity * sizeof(T)));
            if (new_storage == nullptr)  throw std::bad_alloc{};
            T *ptr = dataBeginPtr();
            for (size_t i = 0; i != size(); i++, ptr++) {
                if (ptr == storageEndPtr())  ptr = storageBeginPtr();
                new (new_storage + i) T(std::move(*ptr));
            }
            std::free(_storage_begin_ptr);
            _capacity = new_capacity;
            _storage_begin_ptr = _data_begin_ptr = new_storage;
        }
        T *new_ptr = dataBeginPtr();
        if (new_ptr == storageBeginPtr())  new_ptr = storageEndPtr();
        new_ptr--;  // 此时 ptr 指向即将放置的位置
        new (new_ptr) T(x);
        _size++, _data_begin_ptr = new_ptr;
    } 
    template <typename T>
    auto Deque<T>::popFront() -> void {
        dataBeginPtr()->~T();
        _data_begin_ptr++, _size--;
        if (dataBeginPtr() == storageEndPtr())  _data_begin_ptr = storageBeginPtr();
    }
    template <typename T>
    auto Deque<T>::begin() const noexcept -> Deque<T>::ConstIterator {
        return {this, 0};
    }
    template <typename T>
    auto Deque<T>::begin() noexcept -> Deque<T>::Iterator {
        return {this, 0};
    }
    template <typename T>
    auto Deque<T>::clear() -> void {
        for (auto ptr = dataBeginPtr(); ptr != dataEndPtr(); ptr++) {
            if (ptr == storageEndPtr())  ptr = storageBeginPtr();
            ptr->~T();
        }
        _size = 0, _data_begin_ptr = _storage_begin_ptr;
    }
    template <typename T>
    auto Deque<T>::end() const noexcept -> Deque<T>::ConstIterator {
        return {this, size()};
    }
    template <typename T>
    auto Deque<T>::end() noexcept -> Deque<T>::Iterator {
        return {this, size()};
    }
    template <typename T>
    auto Deque<T>::swap(Deque &other) noexcept -> void {
        std::swap(_size, other._size), std::swap(_capacity, other._capacity);
        std::swap(_data_begin_ptr, other._data_begin_ptr);
        std::swap(_storage_begin_ptr, other._storage_begin_ptr);
    }
    template <typename T>
    auto Deque<T>::operator= (Deque const &other) -> Deque<T> & {
        ~Deque<T>();
        new (this) T(other);
        return *this;
    }
    template <typename T>
    auto Deque<T>::operator= (Deque &&other) noexcept -> Deque<T> & {
        swap(other);
    }
}
int main() {
    using namespace Test;
}