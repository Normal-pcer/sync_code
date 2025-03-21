template <typename T, size_t size>
class RollingContainer {
    using value_type = T::value_type;
    value_type init;
    T storage;
    const value_type *initSourcePtr = nullptr;

    std::array<size_t, size> last;

    const value_type &initItem() const {
        if (initSourcePtr)  return *initSourcePtr;
        else  return init;
    }
public:
    RollingContainer(): init(value_type{}), storage(T(size)) {
        last.fill(~(size_t)0);
    }
    RollingContainer(value_type &&init): init(std::move(init)), storage(T(size)) {
        last.fill(~(size_t)0);
    }
    RollingContainer(value_type const &init): init(value_type{}), storage(T(size)), initSourcePtr(&init) {
        last.fill(~(size_t)0);
    }
    RollingContainer(T &&storage): init(value_type{}), storage(std::move(storage)) {
        last.fill(~(size_t)0);
    }
    RollingContainer(T &&storage, value_type &&init): init(std::move(init)), storage(std::move(storage)) {
        last.fill(~(size_t)0);
    }
    RollingContainer(T &&storage, value_type const &init): init(init), storage(std::move(storage)) {
        last.fill(~(size_t)0);
    }
    value_type &at(size_t x) {
        auto idx = x % size;
        if (x == last[idx])  return storage.at(idx);
        else {
            storage[idx] = initItem();
            last.at(idx) = x;
            return storage[idx];
        }
    }
    value_type &operator[] (size_t x) {
        auto idx = x % size;
        if (x == last[idx])  return storage[idx];
        else {
            storage[idx] = initItem();
            last[idx] = x;
            return storage[idx];
        }
    }
};
