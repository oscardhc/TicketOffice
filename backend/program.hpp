#include"tokenscanner.h"
#include "bplustree.hpp"
#include <cstring>
#include <iostream>
namespace sjtu {
 typedef int ID;
 typedef char* UserInfo;

class Program {

  Program(){};//TODO
  ~Program(){};//TODO

  static inline void exec(char *cmd, char *ret) {
    int len = strlen(cmd);
    char *command = getNextWord(cmd);
    if(strcmp(command,"exit") == 0){ret = "BYE";return;}
    else if(strcmp(command,"clean") == 0){ret = clean();return;}
      /*User command:*/
    else if(strcmp(command,"register") == 0){ret = execute_register();return;}
    else if(strcmp(command,"login") == 0){ret = execute_login();return;}
    else if(strcmp(command,"query_profile") == 0){ret = execute_queryProfile();return;}
    else if(strcmp(command,"modify_profile") == 0){ret = execute_modifyProfile();return;}
    /*Ticket command:*/
    else if(strcmp(command,"query_ticket") == 0){ret = execute_queryTicket();return;}
    else if(strcmp(command,"query_transfer") == 0){ret = execute_queryTransfer();return;}
    else if(strcmp(command,"buy_ticket") == 0){ret = execute_buyTicket();return;}
    else if(strcmp(command,"query_order") == 0){ret = execute_queryOrder();return;}
    else if(strcmp(command,"refund_ticket") == 0){ret = execute_refundTicket();return;}
    /*Train command:*/
    else if(strcmp(command,"add_train") == 0){ret = execute_addTrain();return;}
    else if(strcmp(command,"sale_train") == 0){ret = execute_saleTrain();return;}
    else if(strcmp(command,"query_train") == 0){ret = execute_queryTrain();return;}
    else if(strcmp(command,"delete_train") == 0){ret = execute_deleteTrain();return;}
    else if(strcmp(command,"modify_train") == 0){ret = execute_modifyTrain();return;}
    /*invalid command*/
    else ret = "invaild command\n";
    return;
  }
private:
  tokenscanner scanner;
  bplustree<ID,
  //TODO build bplustree
  char *line;
  char *getNextWord(&char *cmd){
    int pos = 0,len = strlen(cmd);
    char *ret;
    while(pos < len){
      if(cmd[pos] == ' ' || cmd[pos] == '\n' || cmd[pos] == "\t")
        break;
    }
    ret = new char[pos];
    ret = strcpy(cmd,pos)
    //TODO
  }
  /*User command:*/
  char* execute_register(){
    //TODO
  }
  char* execute_login(){
    //TODO
  }
  char* execute_queryProfile(){
    //TODO
  }
  char* execute_modifyProfile(){
    //TODO
  }
  char* execute_modifyPrivilege(){
    //TODO
  }
  /*Ticket command*/
  char* execute_queryTicket(){
    //TODO
  }
  char* execute_queryTransfer(){
    //TODO
  }
  char execute_buyTicket(){
    //TODO
  }
  char* execute_queryOrder(){
    //TODO
  }
  char* execute_refundTicket(){
    //TODO
  }
  /*Train command*/
  char execute_addTrain(){
    //TODO
  }
  char execute_saleTrain(){
    //TODO
  }
  char execute_queryTrain(){
    //TODO
  }
  char execute_deleteTrain(){
    //TODO
  }
  char execute_modifyTrain(){
    //TODO
  }
  /*Administrate*/
  char clean{
    //TODO
  };

};
}
