//
// Created by 傅凌玥 on 2019/5/5.
//

#ifndef TRAINTICKET_USER_KEY_H
#define TRAINTICKET_USER_KEY_H
#include "constant.h"
#include "utility.cpp"
#include <cstring>

namespace sjtu{
typedef int user_ID;
class User_val{
public:
  char name[NAME_SIZE];
  char password[PASSWORD_SIZE];
  char email[EMAIL_SIZE];
  char phone[PHONE_SIZE];
  int privilege = 0;

  User_val(char * inp){
    int len = getNextWord(inp,name);
    inp += len;
    len = getNextWord(inp,password);
    inp += len;
    len = getNextWord(inp,email);
    inp += len;
    len = getNextWord(inp,phone);
    privilege = 1;
    delete inp; //?
  };
  User_val &operator=(const User_val &other) {
    privilege = other.privilege;
    strcpy(name, other.name);
    strcpy(password, other.password);
    strcpy(email, other.email);
    strcpy(phone, other.phone);
    return *this;
  }
  void setInfo(char* inp){
    int len = getNextWord(inp,name);
    inp += len;
    len = getNextWord(inp,password);
    inp += len;
    len = getNextWord(inp,email);
    inp += len;
    len = getNextWord(inp,phone);
  }

};
}

#endif //TRAINTICKET_USER_KEY_H

