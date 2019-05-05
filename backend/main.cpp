/*
 * main.cpp
 *
 *  Created on: Jul 16, 2014
 *      Author: john
 */
 
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "program.hpp"

using namespace std;
#define FIFO_READ "/tmp/pipe.in"
#define FIFO_WRITE "/tmp/pipe.out"
 
#define BUF_SIZE 1024
 
int wfd,rfd;
char ubuf[BUF_SIZE];
char resl[BUF_SIZE];

int main()
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
         printf("ubuf %d %s %c\n", len, ubuf,ubuf[0]);
         
         Program::exec(ubuf, resl);
         usleep(500000);
         
         write(wfd,resl,strlen(resl));
         printf("sent %s\n", resl);
     }
     usleep(100000);
  }
  close(wfd);
  close(rfd);
}


