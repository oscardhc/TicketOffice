//
// Created by 傅凌玥 on 2019/5/5.
//

#ifndef TRAINTICKET_UTILITY_H
#define TRAINTICKET_UTILITY_H

#include <cstdio>
#include <cstring>
#include "constant.h"

namespace sjtu{

  int getNextWord(char *str, char *word){
    int pos = 0, len = strlen(str);
    // printf("len = %d\n", len);
    while (pos < len && (str[pos] == ' ' || str[pos] == '\n' || str[pos] == '\t')) pos++;
    int base = pos;
    while (pos < len) {
      if(str[pos] == ' ' || str[pos] == '\n' || str[pos] == '\t')
        break;
      pos++;
    }
    // printf("%d %d\n", base, pos);
    memcpy(word, str + base, pos - base);
    word[pos - base] = 0;
    return pos;
  }
  int getTrainID(char* trainname){
    int ret = 0;
    int len = strlen(trainname);
    for (int i = 0; i < len; i++)
      ((ret *= BIT) += trainname[i]) %= MOD;
    return ret;
  }
  int dateToInt(char *date){

  }
  void intToDate(const int &inte,char *date){

  }

  int timeToInt(char *time){

  }
  void intToTime(const int &inte,char *time){

  }

  double priceTodouble(char *price){

  }

}
#endif //TRAINTICKET_UTILITY_H
