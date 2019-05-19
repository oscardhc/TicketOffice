//
// Created by 傅凌玥 on 2019/5/8.
//

#include "program.hpp"
#include "utility.hpp"

char cmd[1000],ret[100000];

void work() {
    sjtu::Program prog;
    while (1) {
        fgets(cmd, 200, stdin);
        cmd[strlen(cmd) - 1] = 0;
//        puts(cmd);
//        fprintf(stderr, "%s\n", cmd);
        int _t = clock();
        prog.exec(cmd, ret);
        sjtu::exe += (clock() - _t);
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
    freopen("./stderr.txt", "w", stderr);
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


#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define FIFO_READ "/tmp/pipe.in"
#define FIFO_WRITE "/tmp/pipe.out"

#define BUF_SIZE 1024

int wfd,rfd;
char ubuf[BUF_SIZE];
char resl[BUF_SIZE];

void realWork()
{
    sjtu::Program prog;
    freopen("./out.txt", "r", stdout);
    umask(0);
    wfd=open(FIFO_WRITE,O_SYNC | O_WRONLY, 0777);
    rfd=open(FIFO_READ,O_RDONLY);
    if(wfd==-1 || rfd==-1)
    {
        cout<<"open named pipe error"<<FIFO_WRITE<<strerror(errno)<<endl;
        exit(1);
    }
    printf("%d %d\n", wfd,rfd);
    printf(".....begin\n");
    int nCount=0;
    while(1)
    {
        int len = read(rfd,ubuf,BUF_SIZE);
        if(len>0){
            ubuf[len]=0;

            prog.exec(ubuf, resl);
            usleep(500000);

            write(wfd,resl,strlen(resl));
            printf("read *%s* sent *%s*\n", ubuf, resl);
        }
        usleep(100000);
    }
    close(wfd);
    close(rfd);
}

int main(){
    work();
//    test2();
//    test1();
//    test4();
//    test5();
//    test7();
//    test8();
//    realWork();
    return 0;
}