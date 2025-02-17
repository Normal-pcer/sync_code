朴素的做法是拆位考虑。若干次操作之后，一个位上可能的变化有以下几种：
- 不变。
- 反转。
- 赋值为 $0$。
- 赋值为 $1$。

于是有以下的拆位做法：
```cpp
if (type == 1) {
    // and
    // & 0 是无条件赋值为 0，& 1 不变
    for (i32 i = 0; i < maxBits; i++) {
        if ((value & (1U << i)) == 0)  bits[i] = Assign0;
    }
} else if (type == 2) {
    // or
    // | 1 为无条件赋值为 1
    for (i32 i = 0; i < maxBits; i++) {
        if (value & (1U << i))  bits[i] = Assign1;
    }
} else if (type == 3) {
    // xor
    for (i32 i = 0; i < maxBits; i++) {
        if (value & (1U << i)) {
            bits[i] = [&]() -> ModifyType {
                switch (bits[i]) {
                case Assign0:  return Assign1;
                case Assign1:  return Assign0;
                case Invert:  return None;
                case None:  return Invert;
                default:  assert(false), __builtin_unreachable();
                }
            }();
        }
    }
}
```

我们可以使用两个整数的每一位 `assignBit`、`invertBit` 来表示这一位的变化方式。
- `00`：不变。
- `01`：反转。
- `10`：赋 $0$。
- `11`：赋 $1$。

修改只需要按照如下方式：
```cpp
if (type == 1) {
    // value 所有为 0 的位赋 0
    assignBits |= ~value;
    invertBits &= value;
} else if (type == 2) {
    // value 为 1 的位赋 1
    assignBits |= value;
    invertBits |= value;
} else if (type == 3) {
    invertBits ^= value;
}
```

接下来获取答案。需要赋值为 $1$ 和 $0$ 的位（`assignBit` 对应位为 $1$）分别进行赋值，其他的（`assignBit` 对应位为 $0$，`invertBit` 对应位为 $1$）通过异或反转即可。

代码如下：
```cpp
using i32 = int;
void solve() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 N, C;  std::cin >> N >> C;

    // 记录每一位上的修改
    i32 assignBits = 0;
    i32 invertBits = 0;
    // 00：不变；01：反转；10：赋 0；11：赋 1

    i32 ans = C;
    for (i32 _ = 0; _ < N; _++) {
        i32 type, value;  std::cin >> type >> value;
        if (type == 1) {
            // value 所有为 0 的位赋 0
            assignBits |= ~value;
            invertBits &= value;
        } else if (type == 2) {
            // value 为 1 的位赋 1
            assignBits |= value;
            invertBits |= value;
        } else if (type == 3) {
            invertBits ^= value;
        }

        ans |= assignBits & invertBits;
        ans &= ~(assignBits & ~invertBits);
        ans ^= (~assignBits & invertBits);
        std::cout << ans << endl;
    }
}
```
