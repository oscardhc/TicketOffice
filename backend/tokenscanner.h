//
// Created by 傅凌玥 on 2019/4/23.
//

#ifndef TRAINTICKET_TOKENSCANNER_H
#define TRAINTICKET_TOKENSCANNER_H
#include <iostream>
#include <string>

/*
 * Type:Token Type
 * ---------------
 */
enum ParaType{NAME,PASSWORD,EMAIL,PHONE,ID,PRIVILEGE,
    LOC,DATE,CATALOG,TICKET_KIND,NUM,TIME,PRICE};
enum StateType{REGISTER,LOGIN,QUERY_PROFILE,MODIFY_PROFILE,MODIFY_PRIVILEGE,
    QUERY_TICKET,QUERY_TRANSFER,BUY_TICKET,QUERYORDER,REFUND_TICKET,
    ADD_TRAIN,SALE_TRAIN,QUERY_TRAIN,DELETE_TRAIN,MODIFY_TRAIN ,
    QUIT,CLEAR};

class TokenScanner{
public:
  TokenScanner(std::string inp);
  virtual ~TokenScanner();

  void setInput(std::string inp);
  bool hasMoreTokens();
  std::string nextToken();
  StateType getType();
private:
    std::string str;
    int ptr = 0;
void ignoreWhiteSpace();
};
#endif //TRAINTICKET_TOKENSCANNER_H
