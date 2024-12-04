#include <bits/stdc++.h>

// std::unordered_map<int, int, custom_hash> mp;

namespace unstd {
    using size_t = unsigned long long;
    using ptrdiff_t = long long;

    class _Uint64Hash {
    public:
        static uint64_t splitmix64(uint64_t x) {
            // http://xorshift.di.unimi.it/splitmix64.c
            x += 0x9e3779b97f4a7c15;
            x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
            x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
            return x ^ (x >> 31);
        }

        size_t operator()(uint64_t x) const {
            static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
            return splitmix64(x + FIXED_RANDOM);
        }
    };

    template <
        typename T,
        typename Hash = std::hash<T>,
        typename Compare = std::less<T>
    > class UntitledSet {
        struct KeyEqual {
            bool operator() (T const &a, T const &b) const {
                return not Compare{}(a, b) and not Compare{}(b, a);
            }
        };
    public:
        using log2_t = size_t;                // size_t 的对数类型
        using iterator = std::list<T>::iterator;
        using const_iterator = std::list<T>::const_iterator;
    private:
        struct _ListWithSet {
            using _T = std::list<T>::iterator;

            struct _IteratorCompare {
                template <typename U>
                bool operator() (U const &x, U const &y) const {
                    return Compare{}(*x, *y);
                }
            };

            using SetType = std::set<_T, _IteratorCompare>;
            using ListType = std::list<_T>;

            ListType list;
            SetType *setPtr = nullptr;
            bool usingSetFlag = false;
            static const size_t ListSizeLimit = 8;

            ~_ListWithSet() {
                delete setPtr;
            }

            struct iterator {
                union InnerIterator {
                    ListType::iterator *list;
                    SetType::iterator *set;
                    int null;
                } inner;
                enum FlagType {
                    OnList, OnSet, Invalid
                } flag;

                iterator(ListType::iterator const &it): flag(OnList) {
                    inner.list = new ListType::iterator(it);
                }
                iterator(SetType::iterator const &it): flag(OnSet) {
                    inner.set = new SetType::iterator(it);
                }
                iterator(): flag(Invalid) {
                    inner.null = 0;
                }
                iterator(const iterator &other): flag(other.flag) {
                    if (flag == OnList and other.inner.list)  inner.list = new ListType::iterator(*other.inner.list);
                    if (flag == OnSet and other.inner.set)  inner.set = new SetType::iterator(*other.inner.set);
                }
                ~iterator() {
                    if (flag == OnList and inner.list)  delete inner.list;
                    if (flag == OnSet and inner.set)  delete inner.set;
                }

                _T operator* () {
                    if (flag == OnSet)  return **inner.set;
                    else  return **inner.list;
                }
                bool operator== (const iterator &other) const {
                    switch (flag) {
                    case Invalid:  return other.flag == Invalid;
                    case OnSet:  return *other.inner.set == *inner.set;
                    default:  return *other.inner.list == *inner.list;
                    }
                }

                void operator++ () {
                    if (flag == OnList)  (*inner.list)++;
                    else if (flag == OnSet)  (*inner.set)++;
                }

                void operator++ (int) { (*this)++; }

                bool operator< (const iterator &other) const {
                    return Compare{}(**this, *other);
                }
            };

            bool usingSet() const { return usingSetFlag; }

            iterator end() { 
                if (usingSet())  return iterator{setPtr->end()};
                else  return iterator{list.end()};
            }
            iterator begin() {
                if (usingSet())  return iterator{setPtr->begin()};
                else  return iterator{list.begin()};
            }

            void rebuild() {
                setPtr = new SetType();
                usingSetFlag = true;
                for (auto item: list) {
                    setPtr->insert(std::move(item));
                }
                list.clear();
            }

            bool insert(const _T &x) {
                if (usingSet()) {
                    size_t origin = setPtr->size();
                    setPtr->insert(x);
                    return setPtr->size() != origin;
                } else {
                    if (find(*x) != end())  return false;
                    if (list.size() == ListSizeLimit) {
                        rebuild();
                        size_t origin = setPtr->size();
                        setPtr->insert(x);
                        return setPtr->size() != origin;
                    } else if (find(*x) == end())  return list.push_back(x), true;
                    return false;
                }
            }

            iterator find(const T &x) {
                if (usingSet()) {
                    std::list<T> tmp{x};
                    auto it = setPtr->find(tmp.begin());
                    if (it != setPtr->end())  return iterator{it};
                    else  return {setPtr->end()};
                } else {
                    auto it = list.begin();
                    for ( ; it != list.end(); it++) {
                        if (KeyEqual{}(**it, x))  break;
                    }
                    if (it != list.end())  return iterator(it);
                    else  return {list.end()};
                }
            }

            void erase(const iterator it) {
                if (usingSet()) {
                    setPtr->erase(*it.inner.set);
                } else {
                    list.erase(*it.inner.list);
                }
            }
        };

        std::list<T> _items;

        _ListWithSet *_beginPtr = nullptr;  // 连续内存空间的起始位置
        size_t _size = 0;                   // 存放的元素个数
        log2_t _capacityLog = 0;            // 连续内存空间大小以二为底的对数
        float _maxLoadFactor = 2.0;        // _size / _capacity 的最大值

        size_t _hash(const T &other) const {
            size_t mask = ((size_t)1 << _capacityLog) - 1;
            return _Uint64Hash{}(Hash{}(other)) & mask;
        }

    private:
        // 扩展到指定大小，保证 x 为 2 的整数次幂
        void _expand(size_t x = 0) {
            log2_t new_capacity_log = std::max((size_t)4, x? std::__lg(x): _capacityLog + 1);
            _ListWithSet *new_begin_ptr = new _ListWithSet[(size_t)1<<new_capacity_log];
            _ListWithSet *legecy_ptr = _beginPtr;
            _beginPtr = new_begin_ptr, _capacityLog = new_capacity_log;
            for (auto i = _items.begin(); i != _items.end(); i++) {
                _beginPtr[_hash(*i)].insert(std::move(i));
            }

            delete[] legecy_ptr;
        }

    public:

        ~UntitledSet() {
            if (_beginPtr)  delete[] _beginPtr;
        }

        float getMaxLoadFactor() const { return _maxLoadFactor; }
        void setMaxLoadFactor(float x) { _maxLoadFactor = x; }
        float loadFactor() const { return capacity()? (float)size() / capacity(): getMaxLoadFactor() * 1.1; }

        const_iterator begin() const { return _items.begin(); }
        iterator begin() { return _items.begin(); }
        const_iterator end() const { return _items.end(); }
        iterator end() { return _items.end(); }

        size_t size() const { return _size; }
        size_t capacity() const { return _capacityLog? (size_t)1 << _capacityLog: 0; }

        void insert(T const &x) {
            if (loadFactor() > getMaxLoadFactor())  _expand();
            _items.push_back(x);
            if (_beginPtr[_hash(x)].insert(--_items.end())) {
                _size++;
            }
        }
        void insert(T &&x) {
            if (loadFactor() > getMaxLoadFactor())  _expand();
            size_t hash = _hash(x);
            _items.push_back(std::move(x));
            if (_beginPtr[hash].insert(--_items.end())) {
                _size++;
            }
        }

        void resize(size_t new_size) {
            log2_t log_size = std::__lg(new_size);
            new_size = ((size_t)1<<log_size == new_size)? new_size: (size_t)1 << log_size;
            _expand((size_t)1 << log_size);
        } 

        iterator find(const T &x) {
            if (_size == 0)  return end();
            size_t hash = _hash(x);
            auto sub_it = _beginPtr[hash].find(x);
            if (sub_it == _beginPtr[hash].end())  return end();
            return *sub_it;
        }

        void erase(iterator x) {
            _items.erase(x);
            size_t hash = _hash(x);
            auto sub_it = _beginPtr[hash].find(*x);
            if (sub_it == _beginPtr[hash].end())  return;
            _beginPtr[hash].erase(sub_it);
        }
    };

    template <
        typename Key_T, typename Value_T,
        typename Hash = std::hash<Key_T>,
        typename Compare = std::less<Key_T>
    > class UntitledMap {
        template <typename T>
        struct pairIterator {
            T it;
            using value_type = std::pair<const decltype(it->first)&, decltype(*it->second)&>;

            value_type operator* () const {
                return {it->first, *it->second};
            }

            pairIterator<T> &operator++ () {
                ++it;
                return *this;
            }

            pairIterator<T> operator++ (int) {
                pairIterator<T> it = *this;
                ++*this;
                return it;
            }

            pairIterator<T> &operator-- () {
                --it;
                return *this;
            }
            
            pairIterator<T> operator-- (int) {
                pairIterator<T> it = *this;
                --*this;
                return it;
            }

            bool operator!= (const pairIterator<T> &other) const {
                return it != other.it;
            }
        };

        
    public:
        using key_type = const Key_T;
        using value_type = std::pair<key_type, Value_T>;
    private:
        std::list<Value_T> _valueList;
        using pair_type = std::pair<key_type, typename std::list<Value_T>::iterator>;
        using pair_set_type = UntitledSet<std::pair<key_type, typename std::list<Value_T>::iterator>>;
        pair_set_type _pairSet;
    public:
        using iterator = pairIterator<typename pair_set_type::iterator>;
        using const_iterator = pairIterator<typename pair_set_type::const_iterator>;

    public:
        size_t size() const { return _pairSet.size(); }
        size_t capacity() const { return _pairSet.capacity(); }

        const_iterator begin() const { return {_pairSet.begin()}; }
        iterator begin() { return {_pairSet.begin()}; }
        const_iterator end() const { return {_pairSet.end()}; }
        iterator end() { return {_pairSet.end()}; }

    };
}

int main() {
    unstd::UntitledMap<int, int> map;
    map.begin();
}