//
// Created by Haichen Dong on 2019-05-10.
//

#include "IOManager.hpp"
#include <assert.h>

int clk = 0;

class A {
public:
    int a[23];
    A() {
        for (int i = 0; i < 23; i++) {
            a[i] = clk * 23 + i;
        }
        clk++;
    }
};

const int N = 100000;
A aa[N];

int main() {
//    freopen("stdout.txt", "w", stdout);
    srand(time(NULL));
    sjtu::IOManager dataBase("./out.txt");
    size_t sz = sizeof(A);
    for (int i = 0; i < N; i++) {
        size_t offset = dataBase.createElement(sz);
        dataBase.setElement((char*)(&aa[i]), offset, sz);
    }
    A tmp[10];
    int p[10],pp[10];
    for (int k = 0; k < 100000; k++) {
        for (int j = 0; j < 10; j++) {
            int n = rand();
            if (j == 0) p[j] = rand() % 100;
            else p[j] = p[j - 1] + rand() % 89 + 1;
            pp[j] = rand() % 23;
            aa[p[j]].a[pp[j]] = n;
            dataBase.getElement((char*)&tmp[j], sz * p[j], sz);
            tmp[j].a[pp[j]] = n;
        }
        for (int j = 0; j < 10; j++) {
            dataBase.setElement((char*)&tmp[j], sz * p[j], sz);
        }
    }
    for (int pos = 0; pos < N; pos++) {
        dataBase.getElement((char*)&tmp[0], sz * pos, sz);
        for (int i = 0; i < 23; i++) {
//            printf("%d ", tmp[0].a[i] - aa[pos].a[i]);
            assert(tmp[0].a[i] - aa[pos].a[i] == 0);
        }
//        printf("\n");
    }
    printf("%lu\n", clock());
    return 0;
}
