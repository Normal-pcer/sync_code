#include <bits/stdc++.h>
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

        template <typename Func>
        auto reduce(Func&& f) const { 
            auto result = this->front();
            for (size_t i=1; i<this->size(); i++) {
                result = f(result, this->at(i));
            }
            return result;
        }
        
        bool all(const std::function <bool(const T&)>& f) {
            for (auto& i: *this) {
                if (!f(i)) {
                    return false;
                }
            }
        }

        bool some(const std::function <bool(const T&)>& f) {
            for (auto& i: *this) {
                if (f(i)) {
                    return true;
                }
            }
            return false;
        }

        inline void push(T&& t) {
            this->push_back(t);
        }
    };
}

#define lambda(expr, args...) [&](args) {return expr;}
#define select_func(ret, name, args...) static_cast<ret(*)args>(name)