#include <bits/stdc++.h>

// 辅助模板：根据 index_sequence 构造 std::tuple
template<typename Indices, typename T>
struct TupleHelper;

// 特化版本：处理 std::index_sequence<Is...>
template<std::size_t... Is, typename T>
struct TupleHelper<std::index_sequence<Is...>, T> {
    using type = std::tuple<decltype(Is, T())...>; // 使用逗号运算符生成 N 个 T 类型
};

// 主模板：根据 N 动态生成 std::tuple<T, T, ..., T>
template<typename T, std::size_t N>
using TupleOf = typename TupleHelper<std::make_index_sequence<N>, T>::type;

int main() {
    static_assert(std::is_same<TupleOf<int, 4>, std::tuple<int, int, int, int>>::value);
    TupleOf<int, 4> x;
    return 0;
}