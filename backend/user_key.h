//
// Created by 傅凌玥 on 2019/5/5.
//

#ifndef TRAINTICKET_USER_KEY_H
#define TRAINTICKET_USER_KEY_H
#include "constant.h"
#include "utility.h"
#include <cstring>

namespace sjtu{
  typedef int user_ID;
  class user_val{
  public:
    char name[NAME_SIZE];
    char password[PASSWORD_SIZE];
    char email[EMAIL_SIZE];
    char phone[PHONE_SIZE];
    int privilege = -1;
  private:
    user_val(char * inp){
      int len = getNextWord(inp,name);
      inp += len;
      len = getNextWord(inp,password);
      inp += len;
      len = getNextWord(inp,email);
      inp += len;
      len = getNextWord(inp,phone);
      delete inp; //?
    };
    user_val &operator=(const user_val &other) {
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
      delete inp; //?
    }

    int getPrivilege(){
      return privilege;
    }
  };
}

#endif //TRAINTICKET_USER_KEY_H
