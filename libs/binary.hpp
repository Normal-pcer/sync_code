namespace lib {
    namespace binary {  // 二分
        template <class T, class U, class Func>
        T lower_bound_mapping(
            T begin,
            T end,
            U val,
            Func &&mapping,
            T eps = 1
        ) {
            while (end-begin >= eps) {
                T mid = begin + (end-begin)/2;
                if (mapping(mid) < val) {
                    begin = mid + eps;
                } else {
                    end = mid;
                }
            }
            return begin;
        }

        template <class T, class U, class Func>
        T upper_bound_mapping(
            T begin,
            T end,
            U val,
            Func &&mapping,
            T eps = 1
        ) {
            while (end-begin >= eps) {
                T mid = begin + (end-begin)/2;
                if (mapping(mid) <= val) {
                    begin = mid + eps;
                } else {
                    end = mid;
                }
            }
            return begin;
        }
    }
}