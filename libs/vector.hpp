#define LIB_VECTOR
namespace lib {
    template <typename T>
    class vector: public std::vector<T> {
    public:
        vector() = default;
        vector(const std::initializer_list<T>& list): std::vector<T>(list) {}
        vector(const vector<T>& other): std::vector<T>(other) {}
        vector(const size_t size): std::vector<T>(size) {}

        void filtered(const std::function <bool(const T&)>& f) {
            auto ptr1 = this->begin();
            auto ptr2 = this->begin();

            for (; ptr1!=this->end(); ptr1++) {
                if (f(*ptr1))  *(ptr2++) = *ptr1;
            }

            this->resize(ptr2 - this->begin());
        }

#if __cplusplus >= 201702L

        vector<T> filter(const std::function <bool(const T&)>& f) {
            vector<T> res;
            for (auto& i: *this) {
                if (f(i)) {
                    res.push_back(i);
                }
            }
            return res;
        }
        template <typename Func>  
        auto map(Func&& f) const {  
            vector<decltype(std::invoke(std::forward<Func>(f), std::declval<const T&>()))> result;
            for (auto &i: *this) {
                result.push_back(std::invoke(std::forward<Func>(f), i));
            }
            return result;  
        }

#endif

        template <typename Func>  
        void foreach(Func&& f) const {  
            for (auto &i: *this) {
                f(i);
            }
        }

        template <typename Func>
        auto reduce(Func&& f) const { 
            auto result = this->front();
            for (size_t i=1; i<this->size(); i++) {
                result = f(result, this->at(i));
            }
            return result;
        }
        
        template <class Func>
        bool all(Func&& f) {
            for (auto& i: *this) {
                if (!f(i)) {
                    return false;
                }
            }
            return true;
        }

        bool all() { return all([](auto&x){return x;}); }

        template <class Func>
        bool some(const Func&& f) {
            for (auto& i: *this) {
                if (f(i)) {
                    return true;
                }
            }
            return false;
        }

        bool some() { return some([](auto&x){return x;}); }

        inline void push(T&& t) {
            this->push_back(t);
        }
    };
}

#define select_func(ret, name, args...) static_cast<ret(*)(args)>(name)
