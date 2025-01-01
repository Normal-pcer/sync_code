struct A {
    virtual ~A() = default;
};
struct B: public A {
    int x[1048576];
};

int main() {
    auto pt = dynamic_cast<A *>(new B);
    delete pt;
}