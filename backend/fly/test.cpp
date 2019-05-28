//
// Created by 傅凌玥 on 2019/5/8.
//

#include "program.hpp"
#include "utility.hpp"
#include "station_val.hpp"
#include "constant.hpp"

char cmd[1000],ret[50000];

void work() {
    sjtu::Program prog;
    while (1) {
        sjtu::myget(cmd);
        prog.exec(cmd, ret);
        puts(ret);
        if (strcmp(cmd, "exit") == 0) break;
    }
}

inline void test1() {
//    freopen("./stdout.txt", "w", stdout);
    freopen("../../../test_kit/1/1.in", "r", stdin);
    work();
    fprintf(stderr, "1\n");
    freopen("../../../test_kit/1/2.in", "r", stdin);
    work();
    fprintf(stderr, "2\n");
    freopen("../../../test_kit/1/3.in", "r", stdin);
    work();
    fprintf(stderr, "3\n");
    freopen("../../../test_kit/1/4.in", "r", stdin);
    work();
    fprintf(stderr, "4\n");
}
inline void test2() {
    freopen("./stdout.txt", "w", stdout);
    freopen("../../../test_kit/2/1.in", "r", stdin);
    work();
    fprintf(stderr, "1\n");
    freopen("../../../test_kit/2/2.in", "r", stdin);
    work();
    fprintf(stderr, "2\n");
    freopen("../../../test_kit/2/3.in", "r", stdin);
    work();
    fprintf(stderr, "3\n");
    freopen("../../../test_kit/2/4.in", "r", stdin);
    work();
    fprintf(stderr, "4\n");
    freopen("../../../test_kit/2/5.in", "r", stdin);
    work();
    fprintf(stderr, "5\n");
    freopen("../../../test_kit/2/6.in", "r", stdin);
    work();
    fprintf(stderr, "6\n");
}
inline void test4() {
    freopen("./stdout.txt", "w", stdout);
    freopen("../../../test_kit/4/1.in", "r", stdin);
    work();
    fprintf(stderr, "1\n");
    freopen("../../../test_kit/4/2.in", "r", stdin);
    work();
    fprintf(stderr, "2\n");
    freopen("../../../test_kit/4/3.in", "r", stdin);
    work();
    fprintf(stderr, "3\n");
    freopen("../../../test_kit/4/4.in", "r", stdin);
    work();
    fprintf(stderr, "4\n");
    freopen("../../../test_kit/4/5.in", "r", stdin);
    work();
    fprintf(stderr, "5\n");
}
inline void test5() {
//    freopen("./stdout.txt", "w", stdout);
    freopen("../../../test_kit/5/1.in", "r", stdin);
    work();
    fprintf(stderr, "1\n");
    freopen("../../../test_kit/5/2.in", "r", stdin);
    work();
    fprintf(stderr, "2\n");
}
inline void test6() {
//    freopen("./stdout.txt", "w", stdout);
    freopen("../../../test_kit/6/1.in", "r", stdin);
    work();
    fprintf(stderr, "1\n");
    freopen("../../../test_kit/6/2.in", "r", stdin);
    work();
    fprintf(stderr, "2\n");
}
inline void test7() {
    freopen("./stdout.txt", "w", stdout);
    freopen("../../../test_kit/7/1.in", "r", stdin);
    work();
    fprintf(stderr, "1\n");
    freopen("../../../test_kit/7/2.in", "r", stdin);
    work();
    fprintf(stderr, "2\n");
}
inline void test8() {
    freopen("./stdout.txt", "w", stdout);
//    freopen("./stderr.txt", "w", stderr);
    freopen("../../../test_kit/8/1.in", "r", stdin);
    work();
    fprintf(stderr, "complete 1\n");
    freopen("../../../test_kit/8/2.in", "r", stdin);
    work();
    fprintf(stderr, "complete 2\n");
    freopen("../../../test_kit/8/3.in", "r", stdin);
    work();
    fprintf(stderr, "complete 3\n");
    freopen("../../../test_kit/8/4.in", "r", stdin);
    work();
    fprintf(stderr, "complete 4\n");
    freopen("../../../test_kit/8/5.in", "r", stdin);
    work();
    fprintf(stderr, "complete 5\n");
    freopen("../../../test_kit/8/6.in", "r", stdin);
    work();
    fprintf(stderr, "complete 6\n");
}
inline void preRun() {
    freopen("./stdout.txt", "w", stdout);
//    freopen("./stderr.txt", "w", stderr);
    freopen("../test_kit/8/1.in", "r", stdin);
    work();
    fprintf(stderr, "complete 1\n");
    freopen("../test_kit/8/2.in", "r", stdin);
    work();
    fprintf(stderr, "complete 2\n");
    freopen("../test_kit/8/3.in", "r", stdin);
    work();
    fprintf(stderr, "complete 3\n");
    freopen("../test_kit/8/4.in", "r", stdin);
    work();
    fprintf(stderr, "complete 4\n");
    freopen("../test_kit/8/5.in", "r", stdin);
    work();
    fprintf(stderr, "complete 5\n");
    freopen("../test_kit/8/6.in", "r", stdin);
    work();
    fprintf(stderr, "complete 6\n");
}


//#include <sys/types.h>
//#include <sys/stat.h>
//#include <string.h>
//#include <iostream>
//#include <errno.h>
//#include <fcntl.h>
//#include <unistd.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <zmq.hpp>
//
//void realWork() {
//    sjtu::Program prog;
//    void *context =  zmq_ctx_new();
//    void *responder = zmq_socket(context, ZMQ_REP);
//    int rc = zmq_bind(responder, "tcp://*:5555");
//    fprintf(stderr, "%d\n", rc);
//    while (true) {
//        memset(cmd, 0, sizeof(cmd));
//        zmq_recv(responder, cmd, 1000, 0);
//        fprintf(stderr, "server received %s\n", cmd);
//        prog.exec(cmd, ret);
//        fprintf(stderr, "server respond %s\n", ret);
//        zmq_send(responder, ret, strlen(ret), 0);
//    }
//}

int main(int argc, char** argv){
    test8();
//    work();
    return 0;
//    if (argc > 1) {
//        preRun();
//        realWork();
//    } else {
//        work();
//    }
//    test6();
//    test2();
//    test1();
//    test4();
//    test5();
//    test7();
    return 0;
}