#include <bits/stdc++.h>

namespace MyAlloc {
    auto aligned_alloc(size_t align, size_t size) -> void * {
        static_assert(sizeof(void *) == sizeof(size_t));
    
        align = std::max(alignof(size_t), align);  // 对齐要求至少和 size_t 一样严格
        size_t alloc_size = size + align + sizeof(size_t);  // 需要向系统申请的空间
        void *ptr = std::malloc(alloc_size);
        std::cout << "ptr = " << ptr << std::endl;
        if (ptr == nullptr)  throw std::bad_alloc{};
    
        // 寻找 ptr 之后第一个成功对齐的位置
        size_t ptr_value = 0; std::memcpy(&ptr_value, &ptr, sizeof(ptr_value));
        size_t first_aligned = (ptr_value + align - 1) / align * align;
        // 确保留有八个字节的空间来存储 size_t
        if (first_aligned - ptr_value < sizeof(size_t))  first_aligned += align;
    
        assert(first_aligned % align == 0);
        void *ret = nullptr;  std::memcpy(&ret, &first_aligned, sizeof(ret));  // 返回给用户的地址
        auto *source_pointer = static_cast<size_t *>(ret);
        source_pointer--;
    
        std::memcpy(source_pointer, &ptr, sizeof(size_t));
        return ret;
    }
    
    auto aligned_free(void *ptr) -> void {
        auto source_pointer = static_cast<size_t *>(ptr);
        source_pointer--;
        
        void *allocated = nullptr;  std::memcpy(&allocated, source_pointer, sizeof(allocated));
        std::cout << "allocated = " << allocated << std::endl;
        std::free(allocated);
    }

    template <typename T, typename ...Ts>
    auto aligned_new(size_t align, Ts &&...args) -> T * {
        T *ptr = static_cast<T *>(aligned_alloc(align, sizeof(T)));
        new (ptr) T (std::forward<Ts...>(args...));
        return ptr;
    }

    template <typename T>
    auto aligned_new_array(size_t align, size_t count) -> T * {
        T *ptr = static_cast<T *>(aligned_alloc(align, sizeof(T) * count));
        return ptr;
    }

    template <typename T>
    struct aligned_array {
        using iterator = T *;
        using const_iterator = T const *;

        T *data = nullptr;
        size_t _size = 0;
        aligned_array(size_t align, size_t count): _size(count) {
            data = aligned_new_array<T>(align, count);
        }
        ~aligned_array() {
            aligned_free(data);
        }

        auto size() const -> size_t { return _size; }

        auto operator[] (size_t index) -> T & {
            return data[index];
        }
        auto operator[] (size_t index) const -> T const & {
            return data[index];
        }

        auto begin() -> iterator { return data; }
        auto end() -> iterator { return data + size(); }
        auto begin() const -> const_iterator { return data; }
        auto end() const -> const_iterator { return data + size(); }

        operator T * () { return begin(); }
    };
}

using u32 = unsigned int;
int main() {
    MyAlloc::aligned_array<u32> arr(32, 8);
    std::mt19937 rng{std::random_device{}()};

    for (auto &x: arr)  x = rng();
    std::sort(arr + 0, arr + 8);
    for (int i = 0; i < 8; i++)  std::cout << arr[i] << std::endl;
    return 0;
}