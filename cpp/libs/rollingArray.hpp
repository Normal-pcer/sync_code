namespace lib {
    template <typename T, const long long sz>
    class RollingArray {  // 滚动数组
    public: 
        T arr[sz];
        long long first = 0;  // arr[0] 对应的原下标
        

        void forward() {
            for (long long i=0; i<sz-1; i++) {
                std::memcpy(arr+i, arr+i+1, sizeof(T));
            }
            first++;
        }

        void backward() {
            for (long long i=sz-1; i>0; i--) {
                std::memcpy(arr+i, arr+i-1, sizeof(T));
            }
            first--;
        }

        T& operator[](long long i) {
            long long real = i - first;
            while (real >= sz) {
                real--;
                this->forward();
            }
            while (real < 0LL) {
                real++;
                this->backward();
            }
            return arr[real];
        }
    };
}