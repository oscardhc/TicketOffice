//
// Created by 傅凌玥 on 2019/5/8.
//


#include "utility.cpp"
#include "constant.h"
#include <cstring>
namespace sjtu{
class station_val{
public:
  //char name[NAME_SIZE];
  unsigned int passby_train[186] = {0};//one bit save a passby_train

  station_val() = default;
  void add(int train_num){
    int index = train_num / 32;
    int bit = train_num % 32;
    passby_train[index] | (1 << bit);//need verify
  }

  void del(int train_num){
    int index = train_num >> 5;
    int bit = train_num % 32;
    passby_train[index] &( (unsigned int)(-1) - (unsigned int)(1 << bit));//TODO need verify
  }

  bool getval(int train_num){
    int index = train_num >> 5;
    int bit = train_num % 32;
    unsigned int tmp = (passby_train[index] >> bit) & (unsigned int) 1;
    return tmp;//need verify
  }



};
}



