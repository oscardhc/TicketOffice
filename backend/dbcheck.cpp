#include <stdio.h>
#include "DataBase.hpp"
#include <new>

struct A{
public:
    int bS, cS;
    int a;
    char _s[];
    int &b(int idx) {
        return *(int*)(_s + idx * sizeof(int));
    }
    double &c(int idx) {
        return *(double*)(_s + bS * sizeof(int) + idx * sizeof(double));
    }
    A(int _bS, int _cS):bS(_bS), cS(_cS) {

    }
    ~A() {

    }
};

A* creA(int bS, int cS) {
    printf("- %lu\n", sizeof(int) + bS * sizeof(int) + cS * sizeof(double));
    A* p = (A*)operator new(3 * sizeof(int) + bS * sizeof(int) + cS * sizeof(double));
    new(p) A(bS, cS);
    return p;
}

void delA(A* p) {
    p->~A();
    operator delete(p);
}

int main() {
    printf("< %lu\n", sizeof(A));
    printf("> %lu\n", sizeof(int));
    A* a = creA(5, 5);
    for (int i = 0; i < 5; i++) {
        a->b(i) = i;
        a->c(i) = i + 0.1;
    }
    printf("s %lu\n", sizeof(a));
    for (int i = 0; i < 40; i++) {
        printf("%d\n", a->_s[i]);
    }
    for (int i = 0; i < 5; i++) {
        printf("%d %lf\n", a->b(i), a->c(i));
    }
    delA(a);
}
