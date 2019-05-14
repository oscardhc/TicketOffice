//
// Created by 傅凌玥 on 2019/5/8.
//

#include "program.hpp"
#include <iostream>

char cmd[200],ret[200];

void work() {
    sjtu::Program prog;
    while (1) {
        std::cin.getline(cmd, 200);
        prog.exec(cmd, ret);
        puts(ret);
        if (strcmp(cmd, "exit") == 0) break;
    }
}

int main(){
    srand(time(NULL));
    freopen("./stdout.txt", "w", stdout);
//    freopen("../test_kit/1/1.in", "r", stdin);
//    work();
//    freopen("../test_kit/1/2.in", "r", stdin);
//    work();
//    freopen("../test_kit/1/3.in", "r", stdin);
//    work();
//    freopen("../test_kit/1/4.in", "r", stdin);
//    work();
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
    return 0;
}