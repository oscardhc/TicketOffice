//
// Created by 傅凌玥 on 2019/5/8.
//

#ifndef BACKEND_TRAIN_VAL_H
#define BACKEND_TRAIN_VAL_H
#include "utility.h"
#include "constant.h"

namespace sjtu{
struct station{
   char station_name[NAME_SIZE];
   short arrive;
   short start;
   shour stopover;
   double price[];

};
class train_val{
public:
  int date,citynum = 0,pricenum = 0;
  char ID[TRAIN_ID_SIZE];
  char name[NAME_SIZE];
  char catalog[CATALOG_SIZE];
  station* stations;
  bool if_delete;
  bool if_sale = true;

  train_val(char *inp){
    int len = 0;
    len = getNextWord(inp,ID);
    inp += len;
    len = getNextWord(inp,name);
    inp += len;
    len = getNextWord(inp,catalog);
    inp += len;
    sscanf(inp,"%d",&citynum);
//TODO

  }
  int sale(){
      if_sale = true;
      //insert into bptree

  }
  void print(char *ret){

  }
  int del(){
      if_delete = true;
      if_sale = false;
  }
  int modify(char *inp){

  }
  int refund(char* inp){

  }
  int buy(char *inp){

  }


};
}
#endif //BACKEND_TRAIN_VAL_H
