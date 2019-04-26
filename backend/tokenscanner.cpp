//
// Created by 傅凌玥 on 2019/4/23.
//
#include "tokenscanner.h"
#include <string>
#include <iostream>
#include "error.h"
using namespace std;
TokenScanner::TokenScanner(string inp) {
  str = move(inp);
  ptr = 0;
}

void TokenScanner::setInput(std::string inp){
  str = move(inp);
};
bool TokenScanner::hasMoreTokens(){
  ignoreWhiteSpace();
  return ptr < (int)str.length();
};
std::string TokenScanner::nextToken(){
  string ret;
  ignoreWhiteSpace();
  for(;hasMoreTokens() || str[ptr] == '\n' || str[ptr] == '=';++ptr)
    ret += str[ptr];
  return ret;
};
StateType TokenScanner::getType(){
  if(nextToken() == "register") return REGISTER;
  if(nextToken() == "login") return LOGIN;
  if(nextToken() == "query_profile") return QUERY_PROFILE;
  if(nextToken() == "modify_profile") return MODIFY_PROFILE;
  if(nextToken() == "modify_privilege") return MODIFY_PRIVILEGE;
  if(nextToken() == "query_ticket") return QUERY_TICKET;

};

/* private part*/
void TokenScanner::ignoreWhiteSpace(){
  for(;hasMoreTokens() || str[ptr] == '\n' || str[ptr] == '=';++ptr);
}
