//
// Created by 傅凌玥 on 2019/5/5.
//

#ifndef TRAINTICKET_UTILITY_H
#define TRAINTICKET_UTILITY_H

#include <cstdio>
#include <cstring>

namespace sjtu{
bool scanner(char *cmd,char* ret){
  sscanf(cmd,"%s",ret);
  size_t len = strlen(ret);
  cmd += len;
  if (strcmp(ret,"") == 0) return false;
  return true;
}

}
#endif //TRAINTICKET_UTILITY_H
