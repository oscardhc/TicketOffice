#include <cstring>
#include <iostream>
#include "utility.cpp"
#include "user_key.hpp"
#include "train_val.hpp"
#include "station_val.hpp"
#include "constant.h"
#include "ticket_value.hpp"

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
    else if(strcmp(word,"modify_privilege") == 0){execute_modifyPrivilege();return;}
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
  int trainID[MAX_TRAIN_NUM];
  short cnt_train = 0;

  // bplustree<ID,
  //TODO build bplustree

  /*User word:*/
  void execute_register(){
    char* cur = cmd + commandLength;
    sjtu::User_val val(cur);
    //insert
  }
  void execute_login(){
    //TODO
    char username[20], password[20];
    char* cur = cmd + commandLength;
    int userlen = getNextWord(cur, username);
    cur += userlen;
    int passlen = getNextWord(cur, password);
    //find(getID(username))
    User_val val;
    if (strcmp(val.password,password) == 0) {
      sprintf(ret,"1");
      return;
    }
    sprintf(ret,"0");

    if (strcmp(username, "2020") == 0 && strcmp(password, "asd") == 0) sprintf(ret, "1");
    else sprintf(ret, "0");
  }
  void execute_queryProfile(){
    //TODO
    char *cur = cmd + commandLength;
    int id;
    skipWhiteSpace(cur);
    sscanf(cur, "%d", &id);
    int idlen = 0, tmp = id;
    while(tmp){
      tmp /= 10;
      idlen++;
    }
    cur += idlen;
    //find (id) if failed sprintf(ret,"0");
    User_val val;
    sprintf(ret,"%s %s %s %d\n",val.name,val.email,val.phone,val.privilege);
    sprintf(ret, "a b c d");
  }
  void execute_modifyProfile(){
    //TODO
    char* cur = cmd + commandLength;
    char id[4];
    int idlen = getNextWord(cur,id);
    cur += idlen;
    int ID = 0;
    for(int i = 0 ;i < 4; ++i)
      (ID *= 10) += (id[i] - '0');
    //find(ID)
    User_val val(cur);
    //modify in database

  }
  void execute_modifyPrivilege(){
    //TODO
    char* cur = cmd + commandLength;
    char id1[4],id2[4];
    int privil;
    int p_id1,p_id2;
    int idlen = getNextWord(cur,id1);
    cur += idlen;
    idlen = getNextWord(cur,id2);
    cur += idlen;
    int ID1 = 0,ID2 = 0;
    for(int i = 0 ;i < 4; ++i)
      (ID1 *= 10) += (id1[i] - '0');
    //find (id1)
    if (p_id1 == 1 || p_id1 == 0){
      sprintf(ret,"0");
      return;
    }
    for(int i = 0 ;i < 4; ++i)
      (ID2 *= 10) += (id2[i] - '0');
    idlen = skipWhiteSpace(cur);
    cur += idlen;
    sscanf(cur,"%d",&privil);
    //find(id2)
    if(p_id2 == 2){
      if(privil == 2){
        sprintf(ret,"1");
        return;
      }
    }
    if(p_id2 == 1){
      if(privil == 2){
        //setprivilege
        sprintf(ret,"1");
        return;
      }
    }
    sprintf(ret,"0");
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
    char* cur = cmd + commandLength;
    char user_id[ID_SIZE], train_id[TRAIN_ID_SIZE], sta1[LOCATION_SIZE], sta2[LOCATION_SIZE];
    int num;
    char dateStr[DATE_SIZE];
    char ticketKind[TRAIN_ID_SIZE];
    int len = getNextWord(cur,user_id);
    cur += len;
    skipWhiteSpace(cur);
    scanf(cur,"%d", &num);
    len = 0;
    int tmp = num;
    while(tmp){
      tmp /= 10;
      len ++;
    }
    cur += len;
    len = getNextWord(cur,train_id);
    cur += len;

    int hashid = getID(train_id);
    //b+tree find(hashid)
    Train_val val;
    if (!val.if_sale || val.if_delete){
      sprintf(ret,"0");
      return;
    }
    int bit_id = val.order;
    int hashSta1 = getID(sta1), hashSta2 = getID(sta2);
    //bptree find sta1 sta2
    station_val station1, station2;
    double price = val.buy(num,cur);
    if(station1.getval(bit_id) && station2.getval(bit_id) && price > 0){
//add ticket_value
    }
    sprintf(ret,"0");
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
    char *cur = cmd + commandLength;
    char ID[TRAIN_ID_SIZE];
    int len = getNextWord(cur,ID);
    int hashid = getID(ID);

    Train_val val(cur);
    val.setOrder(cnt_train++);
    //database.insert<hashid,val>
    sprintf(ret,"1");
  }
  void execute_saleTrain(){
    //TODO
    char *cur = cmd + commandLength;
    char ID[TRAIN_ID_SIZE];
    int len = getNextWord(cur,ID);
    int hashid = getID(ID);
    //database.find(hashid) failed:ret = "0"
    Train_val val;//此文件中所有这样的形式都是假装找到
    if (!val.if_sale && !val.if_delete) {
      val.if_sale = true;
      //insert<hashid,val>into bptree
      for(int i = 0;i < val.station_num;++i){
        int station_id = getID(val.stations[i].station_name);
        //find(stationid)
        station_val sta;
        sta.add(hashid);
      }
      sprintf(ret,"1");
      return;
    }
    else
      sprintf(ret,"0");

  }
  void execute_queryTrain(){
    //TODO
    char *cur = cmd + commandLength;
    char ID[TRAIN_ID_SIZE];
    int len = getNextWord(cur,ID);
    int hashid = getID(ID);
    //find(hashid) failed:ret = "0"
    Train_val val;
    if(val.if_sale && !val.if_delete){
      val.print(ret);
      return;
    }
    sprintf(ret,"0");
  }
  void execute_deleteTrain(){
    //TODO
    char *cur = cmd + commandLength;
    char ID[TRAIN_ID_SIZE];
    int len = getNextWord(cur,ID);
    int hashid = getID(ID);
    //find(hashid) failed:ret = "0"
    Train_val val;
    if(!val.if_sale && !val.if_delete){
      val.if_delete = true;
      sprintf(ret,"1");
      for(int i = 0;i < val.station_num;++i){
        int station_id = getID(val.stations[i].station_name);
        //find(stationid)
        station_val sta;
        sta.del(hashid);
      }
      return;
    }
    sprintf(ret,"0");
  }
  void execute_modifyTrain(){
    //TODO
    char *cur = cmd + commandLength;
    char ID[TRAIN_ID_SIZE];
    int len = getNextWord(cur,ID);
    int hashid = getID(ID);
    //find(hashid) failed:ret = "0"
    Train_val val;
    val.del();
    for(int i = 0;i < val.station_num;++i){
      int station_id = getID(val.stations[i].station_name);
      //find(stationid)
      station_val sta;
      sta.del(hashid);
    }
    Train_val value(cur);
    //insert to database
    //bplustree modify address

  }
  /*Administrate*/
  void clean(){
    //TODO
  };

};
}
