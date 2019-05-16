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
sjtu::Program prog;

void realWork()
{
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
//    work();
//    test2();
//    test1();
    realWork();
    return 0;
}