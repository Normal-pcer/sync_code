#include <array>
#include <deque>
#include <vector>
#include <functional>
#include <iostream>
#include <set>

// 用于存储类型 T 的未初始化内存
template <typename T>
struct Uninitialized {
    alignas(alignof(T)) std::array<std::byte, sizeof(T)> storage;

    Uninitialized() {} // 不执行 storage 的初始化

    // 构造对象
    template <typename ...Ts>
    auto construct(Ts &&...args) -> T * {
        return new (storage.data()) T(std::forward<Ts>(args)...);
    }

    auto get() -> T * {
        return std::launder(reinterpret_cast<T *>(storage.data()));
    }

    auto destroy() -> void {
        get()->~T();
    }

    Uninitialized(const Uninitialized &) = delete;
    auto operator=(const Uninitialized &) = delete;
};

// 一个数据结构，可以 O(1) 插入删除无序的数据
template <typename T>
class Pool {
    // 使用 std::deque 来作为底层容器
    std::deque<std::optional<T>> items;
    std::vector<std::optional<T> *> deleted; // 被删除的空位

    struct Iterator {
        std::optional<T> *it;
    };

public:
    Pool() {}

    // 构造一个元素，返回迭代器
    template <typename ...Ts>
    auto emplace(Ts &&...args) -> Iterator {
        if (deleted.empty()) {  // 不可以回收空间了
            auto &item = items.emplace_back(std::in_place, std::forward<Ts>(args)...);
            return {std::addressof(item)};
        }
        auto collected = deleted.back(); deleted.pop_back();
        collected->emplace(std::forward<Ts>(args)...);
        return {collected};
    }

    auto erase(Iterator it) -> void {
        it.it->reset();
        deleted.emplace_back(it.it);
    }

    template <typename Func>
    auto traverse(Func &&func) -> void {
        for (auto &x: items) {
            if (x.has_value()) {
                std::invoke(std::forward<Func>(func), *x);
            }
        }
    }
};

template <typename ...Ts>
class Signal {
    using Handler = std::function<void(Ts...)>;
    std::set<Handler> handlers;

    // 采用特殊的“锁”机制，在锁被销毁时，自动移除对应的 handler
    struct HandlerLock {
        std::set<Handler>::iterator it; // 所在的迭代器
    };
public:
    auto addHandler(Handler action) -> void {
        
    }
};

auto main() -> int {
    Pool<int> pool;
    auto _ = pool.emplace(5);
    auto p2 = pool.emplace(7);
    _ = pool.emplace(8);
    _ = pool.emplace(9);

    pool.erase(p2);
    pool.emplace(10);

    pool.traverse([&](int x) { std::cout << x << ' '; });

    return 0;
}