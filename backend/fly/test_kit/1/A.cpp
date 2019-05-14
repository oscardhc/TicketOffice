#include <stdio.h>

class A {
    int a;
    int b;
public:
    virtual void fun(){};
};

class B: public A {
public:
    void fun() {
        printf("fun B 2333\n");
    }
};

int main() {
    A *bb = new B();
    A *aa = new B();
    printf("%lu\n", sizeof(A));
    printf("%lu\n", sizeof(B));
    aa->fun();
    bb->fun();
    return 0;
}
