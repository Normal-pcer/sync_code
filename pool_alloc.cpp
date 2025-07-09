#include <iostream>
#include <deque>
#include <memory>
#include <cstddef>
#include <set>
#include <iostream>
#include <set>
#include <vector>
#include <chrono>
#include <random>
#include <memory>
#include <algorithm>


template <typename T>
class PoolAllocator {
public:
    // 标准分配器类型定义
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    // 支持 rebind 机制
    template <typename U>
    struct rebind {
        using other = PoolAllocator<U>;
    };

    // 构造函数
    PoolAllocator() noexcept = default;

    // 从其他类型转换的构造函数
    template <typename U>
    PoolAllocator(const PoolAllocator<U>&) noexcept {}

    // 分配内存
    T* allocate(std::size_t n) {
        if (n == 1) {
            return pool_.allocate();
        }
        // 数组分配直接使用 new[]
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    // 释放内存
    void deallocate(T* p, std::size_t n) noexcept {
        if (n == 1) {
            pool_.deallocate(p);
        } else {
            ::operator delete(p);
        }
    }

    // 比较操作符（所有相同类型的分配器都视为相等）
    bool operator==(const PoolAllocator&) const noexcept { return true; }
    bool operator!=(const PoolAllocator&) const noexcept { return false; }

private:
    // 内部 Pool 实现（保持您原有的设计）
    struct Pool {
        union Item {
            Item* nextFree; // 指向下一个空位
            T content;      // 存储数据内容
        };

        std::deque<Item> storage_{};
        Item* firstFree_{nullptr};

        // 分配内存
        T* allocate() {
            if (firstFree_ != nullptr) {
                Item* current = firstFree_;
                firstFree_ = firstFree_->nextFree;
                return &current->content;
            }
            storage_.emplace_back();
            return &storage_.back().content;
        }

        // 释放内存
        void deallocate(T* p) {
            Item* item = reinterpret_cast<Item*>(p);
            item->nextFree = firstFree_;
            firstFree_ = item;
        }
    };

    // 每个类型拥有独立的 Pool 实例
    static Pool pool_;
};

// 静态成员初始化
template <typename T>
typename PoolAllocator<T>::Pool PoolAllocator<T>::pool_;


// 插入 PoolAllocator 实现（前面已提供）

// 性能测试函数
template <typename Alloc>
void test_set_performance(const std::string& alloc_name, size_t num_elements) {
    using SetType = std::set<int, std::less<int>, Alloc>;
    
    // 创建测试数据
    std::vector<int> data;
    data.reserve(num_elements);
    for (size_t i = 0; i < num_elements; ++i) {
        data.push_back(static_cast<int>(i));
    }
    
    // 打乱数据（避免有序插入的特殊优化）
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(data.begin(), data.end(), g);
    
    SetType test_set;
    
    // ===== 插入性能测试 =====
    auto start_insert = std::chrono::high_resolution_clock::now();
    for (int val : data) {
        test_set.insert(val);
    }
    auto end_insert = std::chrono::high_resolution_clock::now();
    auto insert_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_insert - start_insert
    );
    
    // ===== 删除性能测试 =====
    std::shuffle(data.begin(), data.end(), g); // 再次打乱
    auto start_erase = std::chrono::high_resolution_clock::now();
    for (int val : data) {
        test_set.erase(val);
    }
    auto end_erase = std::chrono::high_resolution_clock::now();
    auto erase_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_erase - start_erase
    );
    
    // ===== 结果输出 =====
    std::cout << "Allocator: " << alloc_name << "\n"
              << "  Elements: " << num_elements << "\n"
              << "  Insert time: " << insert_time.count() << " ms\n"
              << "  Erase time: " << erase_time.count() << " ms\n"
              << "  Total time: " << (insert_time + erase_time).count() << " ms\n"
              << "  Set size after test: " << test_set.size() << "\n\n";
}

int main() {
    constexpr size_t small_test = 100'000;    // 10万元素
    constexpr size_t medium_test = 1'000'000; // 100万元素
    constexpr size_t large_test = 5'000'000;  // 500万元素
    
    std::cout << "===== SMALL TEST (" << small_test << " elements) =====\n";
    test_set_performance<std::allocator<int>>("std::allocator", small_test);
    test_set_performance<PoolAllocator<int>>("PoolAllocator", small_test);
    
    std::cout << "\n===== MEDIUM TEST (" << medium_test << " elements) =====\n";
    test_set_performance<std::allocator<int>>("std::allocator", medium_test);
    test_set_performance<PoolAllocator<int>>("PoolAllocator", medium_test);
    
    std::cout << "\n===== LARGE TEST (" << large_test << " elements) =====\n";
    test_set_performance<std::allocator<int>>("std::allocator", large_test);
    test_set_performance<PoolAllocator<int>>("PoolAllocator", large_test);
    
    return 0;
}