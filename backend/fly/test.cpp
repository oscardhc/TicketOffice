//
// Created by 傅凌玥 on 2019/5/8.
//

#include "program.hpp"
#include "utility.hpp"

char cmd[200],ret[200];

void work() {
    sjtu::Program prog;
    while (1) {
        fgets(cmd, 200, stdin);
//        gets(cmd);
        cmd[strlen(cmd) - 1] = 0;
        int _t = clock();
        prog.exec(cmd, ret);
        sjtu::exe += (clock() - _t);
        puts(ret);
        if (strcmp(cmd, "exit") == 0) break;
    }
}

inline void test1() {
//    freopen("./stdout.txt", "w", stdout);
    freopen("../test_kit/1/1.in", "r", stdin);
    work();
    fprintf(stderr, "1\n");
    freopen("../test_kit/1/2.in", "r", stdin);
    work();
    fprintf(stderr, "2\n");
    freopen("../test_kit/1/3.in", "r", stdin);
    work();
    fprintf(stderr, "3\n");
    freopen("../test_kit/1/4.in", "r", stdin);
    work();
    fprintf(stderr, "4\n");
}

inline void test2() {
    freopen("./stdout.txt", "w", stdout);
    freopen("../test_kit/2/1.in", "r", stdin);
    work();
    fprintf(stderr, "1\n");
    freopen("../test_kit/2/2.in", "r", stdin);
    work();
    fprintf(stderr, "2\n");
    freopen("../test_kit/2/3.in", "r", stdin);
    work();
    fprintf(stderr, "3\n");
    freopen("../test_kit/2/4.in", "r", stdin);
    work();
    fprintf(stderr, "4\n");
    freopen("../test_kit/2/5.in", "r", stdin);
    work();
    fprintf(stderr, "5\n");
    freopen("../test_kit/2/6.in", "r", stdin);
    work();
    fprintf(stderr, "6\n");
}

int main(){
//    work();
    test2();
//    test1();
    return 0;
}