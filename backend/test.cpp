//
// Created by 傅凌玥 on 2019/5/8.
//

#include "program.hpp"
#include <iostream>

char cmd[200],ret[200];

int main(){
  sjtu::Program prog;
  while (1) {
    std::cin.getline(cmd, 200);
    prog.exec(cmd, ret);
    printf("%s", ret);
  }
  return 0;
}