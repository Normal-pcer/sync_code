#include <bits/stdc++.h>

template <typename Iter>
auto quickSort(Iter first, Iter last) -> void {
    if (last - first <= 1) return;
    auto pivot = *first;
    auto mid = std::partition(first, last, [&](auto x) { return x < pivot; });
    quickSort(first, mid);
    quickSort(mid + 1, last);
}

template <typename Iter>
auto mergeSort(Iter first, Iter last) -> void {
    if (last - first <= 1) return;
    auto mid = first + ((last - first) >> 1);
    mergeSort(first, mid), mergeSort(mid, last);
    std::inplace_merge(first, mid, last);
}

template <typename Iter>
auto insertSort(Iter first, Iter last) -> void {
    for (auto it = first; it != last; ++it) {
        std::rotate(std::upper_bound(first, it, *it), it, std::next(it));
    }
}

using i32 = std::int32_t;
auto main() -> int {
    std::vector<i32> vec{0, 5, 4, 2, 4, 6, 7};
    insertSort(vec.begin(), vec.end());
    for (auto x: vec) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}