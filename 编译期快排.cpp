#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>
#include <type_traits>

template <typename IS>
struct PrintIS;

template <typename T, T ...nums>
struct PrintIS<std::integer_sequence<T, nums...>> {
    auto static print() -> void {
        ((std::cout << nums << ' '), ...);
    };
};

template <typename T, typename IS, T value>
struct Append;

template <typename T, T ...nums, T value>
struct Append<T, std::integer_sequence<T, nums...>, value> {
    using result = std::integer_sequence<T, nums..., value>;
};

template <typename T, typename IS1, typename IS2>
struct Concat;

template <typename T, T ...nums1, T ...nums2>
struct Concat<T, std::integer_sequence<T, nums1...>, std::integer_sequence<T, nums2...>> {
    using result = std::integer_sequence<T, nums1..., nums2...>;
};

// U 仅用于帮助 SFINAE
template <typename T, typename U, T value, T ...nums>
struct Partition;

template <typename T, T value>
struct Partition<T, void, value> {
    using lessEqual = std::integer_sequence<T>;
    using greater = std::integer_sequence<T>;
};

template <typename T, T value, T first, T ...nums>
struct Partition<T, std::enable_if_t<(first <= value)>, value, first, nums...> {
    using lessEqual = typename Append<T, typename Partition<T, void, value, nums...>::lessEqual, first>::result;
    using greater = typename Partition<T, void, value, nums...>::greater;
};

template <typename T, T value, T first, T ...nums>
struct Partition<T, std::enable_if_t<(first > value)>, value, first, nums...> {
    using lessEqual = typename Partition<T, void, value, nums...>::lessEqual;
    using greater = typename Append<T, typename Partition<T, void, value, nums...>::greater, first>::result;
};

template <typename Seq>
struct QuickSort;

template <typename T, T ...nums>
struct QuickSortImpl {};

template <typename T, T first>
struct QuickSortImpl<T, first> {
    using result = std::integer_sequence<T, first>;
};

template <typename T>
struct QuickSortImpl<T> {
    using result = std::integer_sequence<T>;
};

template <typename T, T first, T ...nums>
struct QuickSortImpl<T, first, nums...> {
    T static constexpr pivot = first;
    using Part1 = typename QuickSort<typename Partition<T, void, first, nums...>::lessEqual>::result;
    using Part2 = typename std::integer_sequence<T, first>;
    using Part3 = typename QuickSort<typename Partition<T, void, first, nums...>::greater>::result;
    using result = typename Concat<T, typename Concat<T, Part1, Part2>::result, Part3>::result;
};

template <typename T, T ...nums>
struct QuickSort<std::integer_sequence<T, nums...>> {
    using result = typename QuickSortImpl<T, nums...>::result;
};

auto main() -> int {
    using qwq = QuickSort<std::integer_sequence<int, 3, 4, 7, 6, 9, 2, 4>>::result;
    PrintIS<qwq>::print();
}
