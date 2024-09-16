from typing import List

class A:
    foo: List["B"]

    def __init__(self, *args: "B"):
        self.foo = list(args)

class B:
    foo: A
    bar: int

    def __init__(self, foo: A, bar: int):
        self.foo = foo
        self.bar = bar

a = A()
b = B(a, 114514)
a.foo.append(b)
print(a.foo[0].bar)