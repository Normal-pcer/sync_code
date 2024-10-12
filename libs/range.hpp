namespace lib {
    template <class T>
    class rangeIterator: public std::iterator<
        std::random_access_iterator_tag,
        T,
        T
    > {
    public:
        T b;
        const T e, s;
        rangeIterator(T b, T e, T s) : b(b), e(e), s(s) {}
        T get(long long idx) {
            return b + s*idx;
        }
        T operator*() {
            return b;
        }
        bool operator== (const rangeIterator &other) {
            return (this->s>0?this->e-this->b:this->b-this->e) <= 0 && this->e==other.e;
        }
        bool operator!= (const rangeIterator &other) {
            return !(*this == other);
        }
        T operator- (const rangeIterator &other) {
            return (this->b-other.b)/s;
        }
        template <class len_t=int>
        rangeIterator<T> operator- (const len_t len) {
            return rangeIterator{b-len*s, e, s};
        }
        template <class len_t=int>
        rangeIterator<T> operator+ (const len_t len) {
            return rangeIterator{b+len*s, e, s};
        }
        void operator++ () {
            b+=s;
        }
        void operator-- () {
            b-=s;
        }
        void operator++ (int) {
            b+=s;
        }
        void operator-- (int) {
            b-=s;
        }
        void operator+= (const T len) {
            b+=len*s;
        }
    };

    template <class T>
    class range {
    public:
        const T b, e, s=1;
        range(T e): e(e) {}
        range(T b,T e): b(b), e(e) {}
        range(T b,T e,T s): b(b), e(e), s(s) {}

        rangeIterator<T> begin() { return rangeIterator<T>(b, e, s); }
        rangeIterator<T> end() { return rangeIterator<T>(e, e, s); }
    };
}