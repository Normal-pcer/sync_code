template <typename T> auto range(const T to) {  return std::views::iota(T(), to);  }
template <typename T> auto range(const T begin, const T end) {  return std::views::iota(begin, end);  }
template <typename T, typename U> struct StepViewIterator {
    T _cur;  const U _step;
    StepViewIterator(T cur, const U step): _cur(cur), _step(step) {}
    auto operator++() { _cur+=_step; }  auto operator++(int) { _cur+=_step; }
    auto operator!=(const StepViewIterator& other) { return _cur<other._cur; }
    auto operator*() { return _cur; }
};
template <typename T, typename U> struct StepView: std::ranges::view_interface<StepView<T, U>> {
    const T _begin, _end;  const U _step;
    StepView(const T begin, const T end, const U step): _begin(begin), _end(end), _step(step) {}
    auto begin() const { return StepViewIterator<T, U>(_begin, _step); }
    auto end() const { return StepViewIterator<T, U>(_end, _step); }
};
template <typename T, typename U> auto range(T begin, T end, U step) { return StepView<T, U>(begin, end, step); }
