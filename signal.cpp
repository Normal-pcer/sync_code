#include <functional>
#include <iostream>
#include <list>
#include <random>
#include <type_traits>

template <typename T>
struct remove_ref_tuple {};

template <typename ...Ts>
struct remove_ref_tuple<std::tuple<Ts...>> {
    using type = std::tuple<std::remove_reference_t<Ts>...>;
};

template <typename T>
using remove_ref_tuple_t = typename remove_ref_tuple<T>::type;

template <typename T, typename U>
concept similar_tuple = std::is_same_v<remove_ref_tuple_t<T>, remove_ref_tuple_t<U>>;

// 模仿 qt signal 实现
namespace Signal {
    template <typename ...Args>
    class Connection;

    // 信号槽
    // 可以绑定若干个处理器函数，发射信号时会调用它们
    // 模板参数为处理器函数的参数列表
    template <typename ...Args>
    class Slot {
    public:
        using Handler = std::function<void(Args...)>;
        Slot() = default;
        
        auto connect(Handler handler) -> Connection<Args...>;
        auto disconnect(Connection<Args...> const &connection) -> void;
        
        // 发射一个信号，调用所有处理器函数
        // 如果该过程中，断开（disconnect）了任何当前槽位上的处理器，则行为未定义
        template <typename ...Ts>
        auto emit(Ts &&...args) -> void 
            requires(similar_tuple<std::tuple<Ts...>, std::tuple<Args...>>)
        {
            for (auto &handler: handlers) {
                handler(std::forward<Ts>(args)...);
            }
        }

        // 不会触发未定义行为的 emit，但是性能较低
        template <typename ...Ts>
        auto safe_emit(Args ...args) -> void 
            requires(similar_tuple<std::tuple<Ts...>, std::tuple<Args...>>)
        {
            auto copy = handlers;
            for (auto &handler: copy) {
                handler(std::move(args)...);
            }
        }
    private:
        // 处理器列表
        // 使用 list 存储以支持常数时间的删除
        std::list<Handler> handlers;
    };

    // 标明一个处理器正在和信号槽之间发生连接
    // 通过 RAII 机制管理函数的生命周期
    template <typename ...Args>
    class Connection {
        using ListIter = std::list<typename Slot<Args...>::Handler>::iterator;

        Slot<Args...> *slot{};  // 来自哪个信号槽
        ListIter iter;  // 槽中的处理器迭代器

        friend class Slot<Args...>;
    public:
        Connection() = default;
        Connection(Slot<Args...> *slot, ListIter iter)
            : slot(slot), iter(iter) {}
        Connection(Connection const &other) = delete;
        Connection(Connection &&other) noexcept
            : slot(other.slot), iter(other.iter) {
            other.slot = nullptr;
        }
        ~Connection() {
            disconnect();
        }

        auto swap(Connection &other) noexcept -> void {
            std::swap(slot, other.slot);
            std::swap(iter, other.iter);
        }

        auto disconnect() -> void {
            if (slot != nullptr) {
                slot->disconnect(*this);
                slot = nullptr;
            }
        }

        auto operator= (Connection const &other) = delete;
        auto operator= (Connection &&other) noexcept -> Connection & {
            Connection tmp{std::move(other)};
            swap(tmp);
            return *this;
        }
    };

    template <typename ...Args>
    auto Slot<Args...>::connect(Handler handler) -> Connection<Args...> {
        handlers.emplace_back(std::move(handler));
        return Connection<Args...>(this, std::prev(handlers.end()));
    }

    template <typename ...Args>
    auto Slot<Args...>::disconnect(Connection<Args...> const &connection) -> void {
        handlers.erase(connection.iter);
    }
}

using namespace Signal;

auto main() -> int {
    
}