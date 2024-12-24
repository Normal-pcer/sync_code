#define DeclareBoxOf(T) typedef struct Box_##T { \
    ##T item; \
} Box_##T; \
##T BoxGet_##T(Box_##T *this) { \
    return this->item; \
}

DeclareBoxOf(int)
DeclareBoxOf(double)

int main() {
    Box_int x = {5};
    printf("%d\n", BoxGet_int(&x));
}