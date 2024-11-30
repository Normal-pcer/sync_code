#include <bits/stdc++.h>
namespace unstd {
    using size_t = unsigned long long;
    using ptrdiff_t = long long;

    template <typename T>
    concept is_pointer = requires (T a) {
        ++a;  *a;
    };

    template <typename T>
    concept iterable = requires (T a) {
        a.begin() != a.end();
        requires is_pointer<decltype(a.begin())>;
    };

    template <typename T, typename U>
    concept convertible_to = requires (T a) {
        static_cast<U>(a);
    };

    template <typename T>
    concept with_size = requires (T) {
        {T::size()} -> convertible_to<size_t>;
    };

    template <typename T>
    class vector {
        T *_begin_ptr;
        size_t _size, _capacity;

        void _range_check(size_t x) const {
            if (x >= _size) {
                throw std::out_of_range("Vector index out of range.");
            }
        }

        void _expand(size_t x = 0) {
            _capacity = x? x: std::max((size_t)16, _capacity<<2);
            T *new_begin = (T*)std::malloc(_capacity * sizeof(T));
            size_t copy_count = std::min(_capacity, _size);
            for (size_t i = 0; i < copy_count; i++)  new (new_begin + i) T(std::move(_begin_ptr[i]));
            std::free(_begin_ptr);
            _begin_ptr = new_begin;
        }
    public:
        vector(): _begin_ptr(nullptr), _size(0), _capacity(0) {}
        template <typename U> requires (convertible_to<U, size_t>)
        vector(U size): _begin_ptr(nullptr), _size(0), _capacity(0) { resize(size); }
        vector(size_t size, T &&x): _begin_ptr(nullptr), _size(0), _capacity(0) {
            reserve(size);
            for (size_t i = 0; i < size; i++)  push_back(x);
        }
        template <iterable U> requires (!with_size<U>)
        vector(U &&other): _begin_ptr(nullptr), _size(0), _capacity(0) {
            for (auto it = other.begin(); it != other.end(); it++) {
                push_back(*it);
            }
        }
        template <iterable U> requires (with_size<U>)
        vector(U &&other): _begin_ptr(nullptr), _size(0), _capacity(0) {
            reserve(other.size());
            for (auto it = other.begin(); it != other.end(); it++) {
                push_back(*it);
            }
        }
        vector(vector<T> &&other) noexcept: _begin_ptr(nullptr), _size(0), _capacity(0) {
            swap(other);
        }
        vector(const std::initializer_list<T> &list): _begin_ptr(nullptr), _size(0), _capacity(0) {
            reserve(list.size());
            for (auto it = list.begin(); it != list.end(); it++) {
                push_back(*it);
            }
        }

        ~vector() {
            if (_begin_ptr != nullptr) {
                for (auto it = begin(); it != end(); it++)  it->~T();
                std::free(_begin_ptr);
            }
        }
        using value_type = T;
        using iterator = T*;
        T const &operator[] (int x) const { return *(_begin_ptr + x); }
        T &operator[] (int x) { return *(_begin_ptr + x); }
        
        T const &at(int x) const { _range_check(x);  return *(_begin_ptr + x); }
        T &at(int x) { _range_check(x);  return *(_begin_ptr + x); }

        void push_back(const T &x) {
            if (_size >= _capacity)  _expand();
            _begin_ptr[_size++] = x;
        }
        template <typename ...Types>
        void emplace_back(Types ...args) {
            if (_size >= _capacity)  _expand();
            new (_begin_ptr+(_size++)) T(std::forward<Types>(args)...);
        }
        int size() const { return _size; }
        T* data() const { return _begin_ptr; }
        void resize(size_t x) {
            if (x >= _capacity)  _expand(x);
            for (size_t i = _size; i < _capacity; i++)  new (_begin_ptr+i) T();
            _size = x;
        }
        void reserve(size_t x) {
            if (x >= _capacity)  _expand(x);
        }

        void insert(iterator pos, const T &value) {
            auto index = pos - begin();
            if (_size >= _capacity)  _expand();
            for (ptrdiff_t i = _size; i != index; i--)  _begin_ptr[i] = std::move(_begin_ptr[i-1]);
            _begin_ptr[index] = value;
            _size++;
        }

        void swap(vector<T> &other) noexcept {
            std::swap(_begin_ptr, other._begin_ptr);
            std::swap(_size, other._size);
            std::swap(_capacity, other._capacity);
        }

        void erase(iterator pos) {
            for (auto it = pos; it != end(); it++)  *it = std::move(*(it+1));
            _size--;
        }

        void clear() {
            vector<T>().swap(*this);
        }

        iterator begin() const { return _begin_ptr; }
        iterator end() const { return _begin_ptr + _size; }

        vector<T> &operator= (vector<T> &&other) noexcept {
            swap(other);
            return *this;
        }
        vector<T> &operator= (const vector<T> &other) {
            reserve(other._size);
            for (auto it = other.begin(); it != other.end(); it++) {
                push_back(*it);
            }
            return *this;
        }

        operator iterator () const { return begin(); }

    };

}

int main() {
    unstd::vector<int> vec {9, 8, 7, 6, 5, 4, 3, 2, 1};
    auto N = 8;
    std::sort(vec+1, vec+N+1);
    for (auto i: vec)  std::cout << i << ' ';
    std::cout << std::endl;
    return 0;
}