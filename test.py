class Foo:
    k: int
    def __init__(self, k: int) -> "Foo":
        self.k = k
    
    def __hash__(self) -> int:
        return hash(self.k)

    def __repr__(self) -> str:
        return f"Foo({self.k})"

f = Foo(5)
dic = dict()
dic[f] = 5
f.k = 10

print(dic[Foo(5)])

print(dic)