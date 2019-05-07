// #include"tokenscanner.h"
// #include "bplustree.hpp"
#include <cstring>
#include <iostream>
namespace sjtu {
 typedef int ID;
 typedef char* UserInfo;

class Program {

public:
  Program(){};//TODO
  ~Program(){};//TODO

  inline void exec(char *_cmd, char *_ret) {
    cmd = _cmd;
    ret = _ret;
    int len = strlen(cmd);
    char word[30];
    commandLength = getNextWord(cmd, word);
    printf("cmd is *%s*\n", word);
    if(strcmp(word,"exit") == 0){sprintf(ret, "BYE");return;}
    else if(strcmp(word,"clean") == 0){clean();return;}
      /*User command:*/
    else if(strcmp(word,"register") == 0){execute_register();return;}
    else if(strcmp(word,"login") == 0){execute_login();return;}
    else if(strcmp(word,"query_profile") == 0){execute_queryProfile();return;}
    else if(strcmp(word,"modify_profile") == 0){execute_modifyProfile();return;}
    /*Ticket command:*/
    else if(strcmp(word,"query_ticket") == 0){execute_queryTicket();return;}
    else if(strcmp(word,"query_transfer") == 0){execute_queryTransfer();return;}
    else if(strcmp(word,"buy_ticket") == 0){execute_buyTicket();return;}
    else if(strcmp(word,"query_order") == 0){execute_queryOrder();return;}
    else if(strcmp(word,"refund_ticket") == 0){execute_refundTicket();return;}
    /*Train command:*/
    else if(strcmp(word,"add_train") == 0){execute_addTrain();return;}
    else if(strcmp(word,"sale_train") == 0){execute_saleTrain();return;}
    else if(strcmp(word,"query_train") == 0){execute_queryTrain();return;}
    else if(strcmp(word,"delete_train") == 0){execute_deleteTrain();return;}
    else if(strcmp(word,"modify_train") == 0){execute_modifyTrain();return;}
    /*invalid word*/
    else{sprintf(ret,"invaild word");}
    return;
  }

private:
  char *cmd;
  char *ret;
  int commandLength;

  // bplustree<ID,
  //TODO build bplustree

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
  /*User word:*/
  void execute_register(){
    //TODO
  }
  void execute_login(){
    //TODO
    char username[20], password[20];
    char* cur = cmd + commandLength;
    int userlen = getNextWord(cur, username);
    cur += userlen;
    int passlen = getNextWord(cur, password);
    if (strcmp(username, "asd") == 0 && strcmp(password, "asd1") == 0) sprintf(ret, "1");
    else sprintf(ret, "0"); 
  }
  void execute_queryProfile(){
    //TODO
  }
  void execute_modifyProfile(){
    //TODO
  }
  void execute_modifyPrivilege(){
    //TODO
  }
  /*Ticket word*/
  void execute_queryTicket(){
    //TODO
  }
  void execute_queryTransfer(){
    //TODO
  }
  void execute_buyTicket(){
    //TODO
  }
  void execute_queryOrder(){
    //TODO
  }
  void execute_refundTicket(){
    //TODO
  }
  /*Train word*/
  void execute_addTrain(){
    //TODO
  }
  void execute_saleTrain(){
    //TODO
  }
  void execute_queryTrain(){
    //TODO
  }
  void execute_deleteTrain(){
    //TODO
  }
  void execute_modifyTrain(){
    //TODO
  }
  /*Administrate*/
  void clean(){
    //TODO
  };

};
}
