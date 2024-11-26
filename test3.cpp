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

    template <typename T>
    concept with_size = requires (T) {
        {T::size()} -> std::convertible_to<size_t>;
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
            _capacity = x? x: std::max(16ULL, _capacity<<2);
            T *new_begin = (T*)std::malloc(_capacity * sizeof(T));
            size_t copy_count = std::min(_capacity, _size);
            for (size_t i = 0; i < copy_count; i++)  new (new_begin + i) T(std::move(_begin_ptr[i]));
            std::free(_begin_ptr);
            _begin_ptr = new_begin;
        }
    public:
        vector(): _begin_ptr(nullptr), _size(0), _capacity(0) {}
        template <typename U, typename=typename std::enable_if_t<std::is_integral_v<U>>>
        vector(U size): _begin_ptr(nullptr), _size(0), _capacity(0) {
            resize(size);
        }
        vector(size_t size, T &&x): _begin_ptr(nullptr), _size(0), _capacity(0) {
            resize(size);
            for (size_t i = 0; i < size; i++)  _begin_ptr[i] = x;
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
        vector(const vector<T> &other): _begin_ptr(nullptr), _size(0), _capacity(0) {
            reserve(other.size());
            for (auto it = other.begin(); it != other.end(); it++) {
                push_back(*it);
            }
        }

        ~vector() { 
            if (_begin_ptr != nullptr)  std::free(_begin_ptr);
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
        void resize(size_t x) {
            _expand(x);
            for (size_t i = _size; i < _capacity; i++)  new (_begin_ptr+i) T;
            _size = x;
        }
        void reserve(size_t x) {
            if (x >= _capacity)  _expand(x);
        }

        void insert(iterator pos, T &&value) {
            if (_size + 1 >= _capacity)  _expand();
            for (auto it = end(); it != pos; it--)  *it = std::move(*(it-1));
            *pos = value;
            _size++;
        }

        iterator begin() const { return _begin_ptr; }
        iterator end() const { return _begin_ptr + _size; }
    };
}

struct Element {
    int val;

    Element(int val = 0): val(val) {}
    Element(const Element &other): val(other.val) {
        // std::cout << "Copy " << val << std::endl;
    }
    Element(Element&& other) noexcept: val(other.val) {
        // std::cout << "Move " << val << std::endl;
    }

    Element &operator= (const Element &other) {
        val = other.val;
        // std::cout << "Assign(copy) " << val << std::endl;
        return *this;
    }
    Element &operator= (Element &&other) {
        val = other.val;
        // std::cout << "Assign(move) " << val << std::endl;
        return *this;
    }
};

int main() {
    unstd::vector<Element> vec;
    vec.reserve(1000000);
    for (auto i = 0; i < 1000000; i++)  vec.emplace_back(i);
    vec.insert(vec.begin(), Element(100));
    std::cout << vec.at(999999).val << std::endl;
    return 0;
}