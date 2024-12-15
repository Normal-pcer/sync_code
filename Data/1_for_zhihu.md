# C++ 算法库简述

C++ 的算法（algorithm）库包含一系列的功能多样的函数。大多数函数定义在头文件 `algorithm` 中，我们这里以 [cpp reference](https://zh.cppreference.com/w/cpp/algorithm) 上给出的为准。

大多数的算法库函数对一个范围的所有元素进行操作，通常需要用户传入一对头尾迭代器或指针 `begin` 和 `end`，表示对这个**左闭右开**区间的所有元素进行目标操作。例如要传入“所有元素”，对于 STL 容器，通常传入 `x.begin()` 和 `x.end()` 两个迭代器；对于数组，可以传入 `arr` 和 `arr + N`（N 为元素数量；特别地，如果从下标为 1 开始存储，可以传入 `arr + 1` 和 `arr + N + 1`）。

从 C++20 开始，可以使用 `ranges` 命名空间中的一些算法，避免传入头尾迭代器的麻烦。例如：
```cpp
std::sort(vec.begin(), vec.end());
```

现在可以写成：
```cpp
std::ranges::sort(vec);
```

一些 `ranges` 算法也支持“投影”特性，支持传入一个函数，用这个函数的返回值进行处理。例如，将一些对象按照 `x` 排序可以写作：
```cpp
std::ranges::sort(vec, std::ranges::less{}, [](Node x) { return x.x; });
```

下文中我们将不再特意提及这个特性。

大多数算法也允许传入一个“执行策略”以支持多线程操作，但是这个功能貌似不是很完善，在这里不多加阐述。

使用标准库算法，可以简化代码，简单地实现一些常用功能。

另外，标准库算法有很多可以传入函数，可以使用“lambda 表达式”省去函数定义的麻烦。

下面的文字中，我们会省略 `std` 命名空间。

需要 C++17 或更高标准的将会进行标注。特别地，ranges 库相关的函数不会标注。

## 最大/最小值操作

### min, max

`max(a, b)`：返回两个值中的最大值。

`min(a, b)`：返回两个值中的最小值。

时间复杂度为  <img src="https://www.zhihu.com/equation?tex=O(1)" alt="O(1)" class="ee_img tr_noresize" eeimg="1"> 。

注意，上述的两个值必须是相同类型。对不同类型的值进行操作（如 `int` 和 `long long`）会导致编译错误。一个常见的问题是 STL 容器的 `size()` 方法返回值通常为 `size_t`，这个类型在 64 位的 Windows 和 Linux 系统下的 gcc 编译器中通常分别实现为 `unsigned long long` 和 `unsigned long`，导致类似 `max(1ULL, vec.size())` 在 Linux 系统下无法通过编译。可以将 `size_t` 强制转换为 `int` 类型，或者使用 C++23 的 `UZ` 结尾数字，将这个值视为 `size_t` 类型。

`max({...})` / `min({...})`：寻找一个初始化列表的首个最大值/最小值。

初始化列表（initializer list）是 C++11 引入的特性，可以用花括号包裹若干个**相同类型**的元素。这允许我们方便地在多个元素间取最大值或最小值。该操作的时间复杂度为  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> （ <img src="https://www.zhihu.com/equation?tex=N" alt="N" class="ee_img tr_noresize" eeimg="1">  为区间长度，下同）。

`ranges::max(range)` / `ranges::min(range)`：返回范围内的首个最大/最小元素。

时间复杂度为  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

这两个函数的所有重载都支持传入一个比较函数 `cmp(a, b)`，返回何时认为 a 小于 b。例如：
```cpp
struct Node { int x, y; };
std::max(Node{1, 2}, Node{2, 1}, [](Node a, Node b) {return a.x < b.x});  // 返回 Node{2, 1}
```

事实上，本节中的函数均支持传入比较函数。


### min_element，max_element

`min_element(begin, end)` / `ranges::min_element(range)`：返回指向范围内的首个最小元素的迭代器。

`max_element(begin, end)` / `ranges::max_element(range)`：返回指向范围内的首个最大元素的迭代器。

时间复杂度为  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### minmax

`minmax(a, b)`：返回一个包括两个元素的 `pair`，如果 a 严格大于 b，这个返回值的顺序与传入参数相反，否则相同。时间复杂度  <img src="https://www.zhihu.com/equation?tex=O(1)" alt="O(1)" class="ee_img tr_noresize" eeimg="1"> 。

`minmax({...})`：返回一个 `pair`，依次包含初始化列表中的首个最小值和最后一个最大值。

`ranges::minmax(range)`：同样返回一个 `pair`，依次包含范围中的首个最小值和最后一个最大值。

后两个函数的时间复杂度均为  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> ，具体地，应用不超过  <img src="https://www.zhihu.com/equation?tex=\frac{3}{2}N" alt="\frac{3}{2}N" class="ee_img tr_noresize" eeimg="1">  次比较。

### clamp （C++17）
`clamp(min, max, x)`：将  <img src="https://www.zhihu.com/equation?tex=x" alt="x" class="ee_img tr_noresize" eeimg="1">  钳制在  <img src="https://www.zhihu.com/equation?tex=[min, max]" alt="[min, max]" class="ee_img tr_noresize" eeimg="1">  的范围中。具体地，如果  <img src="https://www.zhihu.com/equation?tex=x" alt="x" class="ee_img tr_noresize" eeimg="1">  位于该范围，返回  <img src="https://www.zhihu.com/equation?tex=x" alt="x" class="ee_img tr_noresize" eeimg="1"> ；否则返回最接近的边界。

## 排序操作

本节中的函数均支持传入比较函数。

本节的迭代器除了 `is_sorted` 和 `is_sorted_until` 均需要支持随机访问。

### sort

`sort(begin, end)` / `ranges::sort`：对范围按照升序排序，最差情况下复杂度  <img src="https://www.zhihu.com/equation?tex=O(N\cdot\log N)" alt="O(N\cdot\log N)" class="ee_img tr_noresize" eeimg="1"> 。

`sort` 通常使用“内省排序”，优先使用快速排序算法，当递归层数过深时改用堆排序，避免复杂度退化。

`sort` 排序不保证稳定性，即排序前后相等的两个元素的相对位置不确定。

### stable_sort

`stable_sort(begin, end)` / `ranges::stable_sort`：对范围按照升序排序，复杂度  <img src="https://www.zhihu.com/equation?tex=O(N\cdot\log N)" alt="O(N\cdot\log N)" class="ee_img tr_noresize" eeimg="1"> 。这个排序是稳定的，即对于两个相等的元素，原序列中靠前的排序后一定靠前。

### partial_sort

`partial_sort(begin, mid, end)` / `ranges::partial_sort(begin, mid, end)`：重排元素，使得  <img src="https://www.zhihu.com/equation?tex=[begin, mid)" alt="[begin, mid)" class="ee_img tr_noresize" eeimg="1">  区间依次包含原范围前 `distance(begin, mid)` 小的元素。

不保证稳定性，不保证  <img src="https://www.zhihu.com/equation?tex=[mid, end)" alt="[mid, end)" class="ee_img tr_noresize" eeimg="1">   中的顺序。

时间复杂度为  <img src="https://www.zhihu.com/equation?tex=O(N\cdot\log M)" alt="O(N\cdot\log M)" class="ee_img tr_noresize" eeimg="1"> 。

### is_sorted

`is_sorted(begin, end)` / `ranges::is_sorted(range)`：判断范围内元素是否为单调不降，返回布尔值。时间复杂度  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### is_sorted_until

`is_sorted_until(begin, end)` / `ranges::is_sorted_until(range)`：返回最后一个迭代器 `it`，使得区间  <img src="https://www.zhihu.com/equation?tex=[begin, it)" alt="[begin, it)" class="ee_img tr_noresize" eeimg="1">  单调不降。时间复杂度  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### nth_element

`nth_element(begin, nth, end)` / `ranges::nth_element(range, nth)`（`nth` 均为迭代器）：重排元素，使得  <img src="https://www.zhihu.com/equation?tex=[begin, nth)" alt="[begin, nth)" class="ee_img tr_noresize" eeimg="1">  区间均不大于 `nth` 指向的元素， <img src="https://www.zhihu.com/equation?tex=[nth, end)" alt="[nth, end)" class="ee_img tr_noresize" eeimg="1">  区间均不小于该元素，且假如将原序列排序，排序后 `nth` 指向的元素与该函数处理过的相同。

时间复杂度  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

## 二分查找操作

本节中的函数均支持传入比较函数，且保证序列单调不降。

本节内函数时间复杂度均为  <img src="https://www.zhihu.com/equation?tex=O(\log N)" alt="O(\log N)" class="ee_img tr_noresize" eeimg="1"> ，但要保证迭代器支持随机访问。

### lower_bound

`lower_bound(begin, end, x)` / `ranges::lower_bound(range, x)`：返回第一个大于等于 `x` 的元素的迭代器，找不到返回 end。

### upper_bound

`upper_bound(begin, end, x)` / `ranges::upper_bound(range, x)`：返回第一个大于 `x` 的元素的迭代器，找不到返回 end。

事实上，x 的 lower_bound 和 upper_bound 形成的左闭右开区间内均等于 x。

### equal_range

`equal_range(begin, end, x)` / `ranges::equal_range(range, x)`：等价于 `pair{lower_bound(...), upper_bound(...)}`。

### binary_search

`binary_search(begin, end, x)` / `ranges::binary_search(range, x)`：返回 `x` 是否出现过。

## 交换

### swap

`swap(a, b)`：交换两个元素，需要二者类型相同，且可以“移动构造”和“移动赋值”。时间复杂度为  <img src="https://www.zhihu.com/equation?tex=O(1)" alt="O(1)" class="ee_img tr_noresize" eeimg="1"> 。

`swap(a[N], b[N])`：交换两个数组间的对应元素。值得注意的是，该操作时间复杂度为  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### swap_ranges

`swap_ranges(begin1, end1, begin2)`：交换 `begin1` 和 `begin2` 开始的两个范围内的对应元素，长度均为 `distance(begin1, end1)`。

`swap_ranges(range1, range2)`：交换 `range1` 和 `range2` 内的对应元素。

时间复杂度均为  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### iter_swap

`iter_swap(a, b)`：交换两个迭代器对应元素，时间复杂度  <img src="https://www.zhihu.com/equation?tex=O(1)" alt="O(1)" class="ee_img tr_noresize" eeimg="1"> 。

## 搜索操作
我们称呼一个返回布尔类型的、接受  <img src="https://www.zhihu.com/equation?tex=N" alt="N" class="ee_img tr_noresize" eeimg="1">  个参数的函数为 “N 元谓词”。以下参数的 `p` 均为谓词。

若无特殊说明，找不到的均返回 `end` 或 `end1`。

一些返回子区间的 `ranges` 算法可能会返回 `borrowed_subrange_t` 避免垂悬指针，这里为了方便说做“子区间”。

### all_of, any_of, none_of
xxx 依次指代 `all`、`any` 或 `none`。

`xxx_of(begin, end, p)` / `ranges::xxx_of(range, p)`：区间内是否全部/包含/不含符合谓词 p 的元素，复杂度  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### ranges::contains，ranges::contains_range（C++23）
`ranges::contains(range, x)`：返回布尔类型，区间内是否包含元素 x，复杂度  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

`ranges::contains_range(range1, range2)`：返回布尔类型，区间 range2 是否为 range1 的子区间，复杂度  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### find，find_if，find_if_not
`find(begin, end, x)` / `ranges::find(range, x)`：查找第一个 x，返回其迭代器。

`find_if(begin, end, p)` / `ranges::find_if(range, x)`：查找第一个满足 p 的元素，返回其迭代器。

`find_if_not(begin, end, p)` / `ranges::find_if_not(range, x)`：查找第一个不满足 p 的元素，返回其迭代器。

时间复杂度均为  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### ranges::find_last，ranges::find_last_if，ranges::find_last_if_not（C++23）
与上述 `find` 系列用法相同，但是返回最后一个。

### find_end
`find_end(begin1, end1, begin2, end2)` / `ranges::find_end(range1, range2)`：查找第二个区间在第一个区间内最后一次出现的位置，返回其首迭代器/对应子区间。可以传入等价谓词（用于判断两个元素相等）。

时间复杂度  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### find_first_of
`find_first_of(begin1, end1, begin_s, end_s)` / `ranges::find_first_of(range1, range_s)`：查找第一个区间的第一个元素，使得它也位于第二个区间中，返回其迭代器。可以传入等价谓词。时间复杂度  <img src="https://www.zhihu.com/equation?tex=O(N\cdot M)" alt="O(N\cdot M)" class="ee_img tr_noresize" eeimg="1"> （ <img src="https://www.zhihu.com/equation?tex=N, M" alt="N, M" class="ee_img tr_noresize" eeimg="1">  分别为两个区间的长度，下同）。

### adjacent_find
`adjacent_find(begin, end)` / `ranges::adjacent_find(range)`：查找第一个元素，使得它和它后面的元素相同。时间复杂度  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> ，可以传入等价谓词。

### count，count_if
`count(begin, end, x)` / `ranges::count(range, x)`：返回 x 的出现次数。

`count_if(begin, end, p)` / `ranges::count_if(range, p)`：返回符合 p 的元素个数。

复杂度均为  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### mismatch
`mismatch(begin1, end1, begin2)` / `mismatch(begin1, end1, begin2, end2)` / `ranges::mismatch(range1, range2)`：返回两个区间首个不同元素的迭代器组成的 `pair` 或 `mismatch_result`，可以传入等价谓词。如果区间长度不同，在到达其中一个的结尾之后停止匹配。时间复杂度  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### equal
`equal(begin1, end1, begin2)` / `equal(begin1, end1, begin2, end2)` / `ranges::equal(range1, range2)`：返回布尔值，两个区间是否相同。如果长度不同返回 false。可以传入等价谓词。复杂度  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### search
`search(begin1, end1, begin2, end2)` / `ranges::search(range1, range2)`：返回第二个区间在第一个区间内第一次出现的位置，返回其首迭代器/查找到的子区间。可以传入等价谓词。复杂度  <img src="https://www.zhihu.com/equation?tex=O(N\cdot M)" alt="O(N\cdot M)" class="ee_img tr_noresize" eeimg="1"> 。

`search(begin, end, searcher)`（C++17）：利用一个搜索器进行搜索，可以达到更高的效率。STL 内置了 `boyer_moore_searcher(begin2, end2)` 和 `boyer_moore_horspool_searcher(begin2, end2)`。

### search_n
`search_n(begin, end, cnt, val)` / `ranges::search_n(range, cnt, val)`：在区间中查找连续 `cnt` 个 `val`，返回第一个元素的迭代器/查找到的子区间。复杂度  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### ranges::starts_with，ranges::ends_with（C++23）
`ranges::xxx_with(range1, range2)`：返回布尔值，第一个区间是否以第二个开头或结尾。

## 顺序变更操作
本节的函数复杂度均为  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### reverse
`reverse(begin, end)` / `ranges::reverse(range)`：翻转区间内的元素。

### rotate
`rotate(begin, mid, end)` / `ranges::rotate(range, mid)`：将  <img src="https://www.zhihu.com/equation?tex=[begin, mid)" alt="[begin, mid)" class="ee_img tr_noresize" eeimg="1">  整个区间移动到  <img src="https://www.zhihu.com/equation?tex=end" alt="end" class="ee_img tr_noresize" eeimg="1">  及以后的位置上。设 `new_begin` 表示当前指向 `begin` 元素的迭代器，返回 `new_begin` 或  <img src="https://www.zhihu.com/equation?tex=[new\_begin, end)" alt="[new\_begin, end)" class="ee_img tr_noresize" eeimg="1">  子区间。

### shift_left，shift_right（C++20）
`shift_left(begin, end, n)`：左移范围内的元素，返回迁移后区间结尾。例如  <img src="https://www.zhihu.com/equation?tex=[1, 2, 3, 4, 5]" alt="[1, 2, 3, 4, 5]" class="ee_img tr_noresize" eeimg="1">  左移两位得到  <img src="https://www.zhihu.com/equation?tex=[3, 4, 5, 4, 5]" alt="[3, 4, 5, 4, 5]" class="ee_img tr_noresize" eeimg="1"> 。
`shift_right(begin, end, n)`：右移范围内的元素，返回迁移后区间开头。

`ranges::shift_xxx`（C++23）：左移或右移返回迁移后区间与原区间的交集。

### shuffle
`shuffle(begin, end, rnd)`/`ranges::shuffle(range, rnd)`：使用 `rnd` 作为随机数生成器打乱范围内的元素。

## 变换操作
若无特殊说明，以下介绍中省略 `ranges` 算法，用输入范围代替头尾迭代器即可。

以下操作的复杂度大多为  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### transform
`transform(begin1, end1, begin_out, op)`：对每一个元素应用操作，返回值存储到目标范围。

`transform(begin1, end1, begin2, begin_out, op)`：对这两个范围内的每组对应元素应用操作，返回值存储到目标范围。

### replace，replace_if
`replace(begin, end, old_v, new_v)`：将 `old_v` 替换为 `new_v`。

`replace_if(begin, end, p, new_v)`：将满足 `p` 的元素替换为 `new_v`。

## 生成操作
### fill，fill_n
`fill(begin, end, x)`：在范围内填充 `x`。

`fill_n(begin, n, x)`：从 `begin` 开始填充 `n` 个 `x`。

### generate，generate_n
`generate(begin, end, op)`：范围内所有元素赋值为 `op` 的返回值。

`generate(begin, n, op)`：`begin` 开始的 `n` 个元素赋值为 `op` 的返回值。

## 复制操作
### copy
`copy(begin, end, begin_out)`：复制一个区间到目标区间。

### copy_if
`copy_if(begin, end, begin_out, op)`：对于区间的每个元素，如果满足 `p`，则复制到目标区间。

### copy_backward
`copy_backward(begin, end, begin_out)`：从后往前复制区间，即先复制后面的元素，但最后仍是正序排列。

### copy_n
`copy_n(begin, n, begin_out)`：复制区间的前 `n` 个元素。

### move
`move(begin, end, begin_out)`：移动一个区间，即对每个元素调用其移动赋值。

### move_backward
`move_backward(begin, end, begin_out)`：从后往前移动区间。

## 移除操作
对于这些操作，常规函数一般返回新的尾迭代器，ranges 算法一般返回  <img src="https://www.zhihu.com/equation?tex=[new_end, end)" alt="[new_end, end)" class="ee_img tr_noresize" eeimg="1">  的子区间。

值得注意的是，它们不会真的移除容器中的元素，需要手动根据返回值 resize 或者 erase。

### remove，remove_if
`remove(begin, end, x)`：移除所有的 x。

`remove_if(begin, end, p)`：移除所有满足 `p` 的元素。

### unique
`unique(begin, end, x)`：移除相邻的相同元素。

配合排序可用于去重。


## 采样操作
### sample
`sample(begin, end, out, n, rnd)`：用 `rnd` 作为随机数生成器，在原区间里随机选择 `n` 个元素写入目标区间中。

## 排列操作
### next_permutation
`next_permutation(begin, end)`： 如果存在下一个排列，则将当前序列变成下一个排列并返回 true；否则将序列升序排列并返回 false。

`ranges::next_permutation(range)`：同上，但是返回一个 `next_permutation_result` 对象。


### prev_permutation
同上，变成全排列中的上一个。

## 划分操作
“划分”操作是重排序一个范围内的所有元素，使得存在一个元素，对于一个指定的谓词 p，左侧的元素都返回 true，右侧都返回 false。复杂度  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### partition
`partition(begin, end, p)`：划分给定范围，返回第一个不满足 p 的元素的迭代器。

### stable_partition
同上，但是保证同一块内元素的相对顺序不变。

### partition_point
`partition_point(begin, end, p)`：寻找按 p 划分好的范围的划分点，返回第一个不满足 p 的元素的迭代器。复杂度  <img src="https://www.zhihu.com/equation?tex=O(\log N)" alt="O(\log N)" class="ee_img tr_noresize" eeimg="1"> 。


## 归并操作
### merge
`merge(begin1, end1, begin2, end2, begin_out)`：合并两个已经有序的序列到目标迭代器，成为一个有序的序列，时间复杂度  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

可以传入比较函数。

### inplace_merge
`inplace_merge(begin, mid, end)`：保证  <img src="https://www.zhihu.com/equation?tex=[begin, mid)" alt="[begin, mid)" class="ee_img tr_noresize" eeimg="1">  和  <img src="https://www.zhihu.com/equation?tex=[mid, end)" alt="[mid, end)" class="ee_img tr_noresize" eeimg="1">  两个区间有序的情况下合并这两段，时间复杂度  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

可以传入比较函数。

这个函数的空间复杂度也是  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1">  的。

## 堆操作
算法库支持在一个支持随机访问的容器中维护一个大根堆，类似 `priority_queue`。

用这种方法实现的堆需要自行维护尾指针。

下列函数支持传入比较函数。

### make_heap
`make_heap(begin, end)`：在指定范围上建立一个大根堆。时间复杂度  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### push_heap
`push_heap(begin, end)`：将迭代器 `end - 1` 指向的元素插入到  <img src="https://www.zhihu.com/equation?tex=[begin, end-1)" alt="[begin, end-1)" class="ee_img tr_noresize" eeimg="1">  区间构成的堆中。时间复杂度  <img src="https://www.zhihu.com/equation?tex=O(\log N)" alt="O(\log N)" class="ee_img tr_noresize" eeimg="1"> 。

### pop_heap
`pop_heap(begin, end)`：移除这个堆中最大的元素，即将 `begin` 和 `end - 1` 指向的元素交换，接着维护  <img src="https://www.zhihu.com/equation?tex=[begin, end-1)" alt="[begin, end-1)" class="ee_img tr_noresize" eeimg="1">  上的大根堆。

### is_heap
`is_heap(begin, end)`：返回布尔值，判断范围内是否为一个合法的堆。时间复杂度为  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### is_heap_until
`is_heap_until(begin, end)` / `ranges::is_heap_until(begin, end)`：判断从 begin 开始能形成的最大堆，返回其尾迭代器/子区间。时间复杂度为  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 

## 字典序
下列函数可以按字典序比较两个范围内的元素，时间复杂度均为  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### lexicographical_compare
`lexicographical_compare(begin1, end1, begin2, end2)`：返回第一个范围是否小于第二个范围。会优先比较前面的元素，仅当其中一个范围是另一个的前缀时认为短的范围字典序更小。可以传入比较函数。

### lexicographical_compare_three_way
`lexicographical_compare_three_way(begin1, end1, begin2, end2)`：按照二者的三路比较运算符进行比较，返回值类型和它们的三路比较运算符相同，只会是 `strong_ordering`，`weak_ordering` 和 `partial_ordering` 中的一种。可以传入三路比较函数。

这个函数在 ranges 中没有对应函数。

## 数值运算
这些函数在 `numeric` 头文件中定义，不过被 cppreference 归类为算法。

除特殊说明，这些函数在 ranges 中均没有对应函数。

下列函数的时间复杂度均为  <img src="https://www.zhihu.com/equation?tex=O(N)" alt="O(N)" class="ee_img tr_noresize" eeimg="1"> 。

### iota
`iota(begin, end, value)` / `ranges::iota(range, value)`：将区间赋成从 value 开始的递增序列。

### accumulate
`accumulate(begin, end, init, op)`：将序列中的元素依次“累加”到 init 上，类似  <img src="https://www.zhihu.com/equation?tex=op(op(init, a_1), a_2)\dots" alt="op(op(init, a_1), a_2)\dots" class="ee_img tr_noresize" eeimg="1">  的方式运算。不传入 `op` 默认为求和。

不要求 init 和元素类型相同。

### inner_product
`inner_product(begin1, end1, begin2, init)`：将两个序列看成  <img src="https://www.zhihu.com/equation?tex=N" alt="N" class="ee_img tr_noresize" eeimg="1">  维向量，计算二者的数量积并加上 init。即 

<img src="https://www.zhihu.com/equation?tex=init+\sum\limits_{1\le i\le N}a_ib_i" alt="init+\sum\limits_{1\le i\le N}a_ib_i" class="ee_img tr_noresize" eeimg="1">
的值。

`inner_product(begin1, end1, begin2, init, op1, op2)`：用 `op1` 替换上文的加法，`op2` 替换乘法。即对于每一次迭代：

<img src="https://www.zhihu.com/equation?tex=init\leftarrow op1\left(init, op2\left(x, y\right)\right) " alt="init\leftarrow op1\left(init, op2\left(x, y\right)\right) " class="ee_img tr_noresize" eeimg="1">


### adjacent_difference
`adjacent_difference(begin, end, begin2, op)`：计算差分数组，对于每一个元素，输出数组的对应元素将为 `op(*it, *(it-1))`；特别地，第一个元素直接赋值到输出的第一项。不传入 `op` 默认为减法。

### partial_sum
`partial_sum(begin, end, begin2, op)`：计算前缀和，输出的第  <img src="https://www.zhihu.com/equation?tex=i" alt="i" class="ee_img tr_noresize" eeimg="1">  项等于原序列前  <img src="https://www.zhihu.com/equation?tex=i" alt="i" class="ee_img tr_noresize" eeimg="1">  项通过 `op` 累加的结果。不传入 `op` 默认为加法。

### reduce
行为类似于 `accumulate`，但是不保证按顺序累加。在并行运算下可能有更高的效率。

如果操作不满足交换律和结合律将会产生未定义行为。

`reduce(begin, end)`：相当于 `reduce(begin, end, /*这一类型的 0 值*/)`。

其余用法同 `accumulate`。

### inclusive_scan
行为类似于 `partial_sum`，但是不保证按顺序累加。在并行运算下可能有更高的效率。

如果操作不满足结合律将会产生未定义行为。

用法同 `partial_sum`。

### exclusive_scan
行为类似于 `inclusive_scan`，但是输出的第  <img src="https://www.zhihu.com/equation?tex=i" alt="i" class="ee_img tr_noresize" eeimg="1">  项不会对原序列的第  <img src="https://www.zhihu.com/equation?tex=i" alt="i" class="ee_img tr_noresize" eeimg="1">  项进行累加。

例如，对 `[3, 4, 5, 6]` 求前缀和，`partial_sum`、`inclusive_scan`、`exclusive_scan` 的结果分别为 `[3, 7, 12, 18]`、`[3, 7, 12, 18]` 和 `[0, 3, 7, 12]`。

## 折叠（C++23）
折叠类似于 `accumulate`，具体地讲，对于给定运算，左折叠表示从左向右进行“求和”，右折叠则表示从右到左“求和”。

### fold_left
`fold_left(range, init, op)`：在 `init` 的基础上，通过 `op` 求和。即表达式  <img src="https://www.zhihu.com/equation?tex=op(op(init, a_1), a_2)\dots" alt="op(op(init, a_1), a_2)\dots" class="ee_img tr_noresize" eeimg="1"> 

### fold_left_first
`fold_left_first(range, op)`：在范围首项的基础上，通过 `op` 求和。返回值为 `optional`。

### fold_right
`fold_right(range, init, op)`：在 `init` 的基础上，通过 `op` 倒序求和。即表达式  <img src="https://www.zhihu.com/equation?tex=\dots op(a_{N-1}, op(a_N, init))" alt="\dots op(a_{N-1}, op(a_N, init))" class="ee_img tr_noresize" eeimg="1"> 。

### fold_right_last
`fold_right_last(range, op)`：在范围末项的基础上，通过 `op` 求和。返回值为 `optional`。

### fold_left_with_iter
`fold_left_with_iter(range, init, op)`：返回一个 `fold_left_with_iter_result` 对象，包含范围的尾迭代器和 `fold_left` 的值。

### fold_left_first_with_iter
`fold_left_first_with_iter(range, op)`：返回一个 `fold_left_first_with_iter_result` 对象，包含范围的尾迭代器和 `fold_left_first` 的值（这个值为 `optional`）。

以上应该就是 C++ 算法库的大致内容了。

此外，上文在讲 ranges 算法的时候省略了不少内容，事实上 ranges 算法也可以像常规的算法函数一样传入头尾指针，在上文中我们忽略了这一点。

但是注意，ranges 中如果要传入多个范围，每一个都需要包含尾指针，类似 `ranges::transform(begin1, end1, begin2, end2, op)`。

此外，还有一些函数可以实现先拷贝再执行，上文并没有提到。包括：
`replace_copy`，`replace_copy_if`，`remove_copy`，`remove_copy_if`，`unique_copy`，`reverse_copy`，`rotate_copy`，`partition_copy`，`partial_sort_copy`。


如果希望了解更详细的信息，可以查看 [cppreference](https://zh.cppreference.com/w/cpp/algorithm)。

文章较长，难免有笔误等疏忽，烦请告知和指正。也希望这篇文章能帮助您了解 C++ 的算法库。

感谢您的阅读。
