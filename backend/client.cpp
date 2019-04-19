/*
 * main.cpp
 *
 *  Created on: Jul 16, 2014
 *      Author: john
 */
 
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<iostream>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
using namespace std;
#define FIFO_WRITE "/tmp/pipe.out"
#define FIFO_READ "/tmp/pipe.in"
 
#define BUF_SIZE 1024
 
int main()
{
   int rfd;
   char ubuf[BUF_SIZE]={0};
   umask(0);
   rfd=open(FIFO_READ,O_RDONLY);
   wfd=open(FIFO_WRITE,O_WRONLY);
  int nCount=0;
  while(1)
  {
     int  len=read(rfd,ubuf,BUF_SIZE);
     if(len>0)
     {
       ubuf[len]='\0';
       cout<<"server:"<<ubuf<<endl;
       cout<<"read.."<<nCount<<endl;
       nCount++;
     }
  }
 
}


