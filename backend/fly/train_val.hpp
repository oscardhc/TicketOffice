//
// Created by 傅凌玥 on 2019/5/8.
//

#ifndef BACKEND_TRAIN_VAL_H
#define BACKEND_TRAIN_VAL_H
#include "utility.cpp"
#include "constant.h"
#include <string>

namespace sjtu {

class Train_val {

  struct station {
    char station_name[LOCATION_SIZE];
    short arrive;
    short start;
    short stopover;
    double *price;
    int ticket[31];
    station() = default;

    void setInp(char *inp) {
      char time[6];
      int len = getNextWord(inp,station_name);
      inp += len;
      len = getNextWord(inp,time);
      inp += len;
      arrive = timeToInt(time);
      len = getNextWord(inp,time);
      inp += len;
      start = timeToInt(time);
      len = getNextWord(inp,time);
      inp += len;
      stopover = timeToInt(time);
      price = new double[price_num];
      for(int i = 0;i < price_num;++i){
        len = skipWhiteSpace(inp);
        inp += (len + 1);
        sscanf(inp,"%f",&price[i]);
      }
      memset(ticket,2000,sizeof(ticket));
    }
    ~station(){
      delete[]price;
    }
  };
public:
  int station_num = 0, price_num = 0;
  short order;
  char trainID[TRAIN_ID_SIZE];
  char name[NAME_SIZE];
  char catalog[CATALOG_SIZE];
  station *stations;
  char *pricename[TICKET_KIND_SIZE];
  bool if_delete = true;
  bool if_sale = true;

  Train_val(char *inp) {
    int len = 0;
    len = getNextWord(inp,trainID);
    inp += len;
    len = getNextWord(inp, name);
    inp += len;
    len = getNextWord(inp, catalog);
    inp += len;
    len = skipWhiteSpace(inp);
    inp += len;
    sscanf(inp, "%d", &station_num);
    stations = new station[station_num];
    len = skipWhiteSpace(inp);
    inp += len;
    sscanf(inp, "%d", &price_num);
    for(int i = 0;i < price_num;++i){
      pricename[i] = new char[price_num];
      len = getNextWord(inp,pricename[i]);
      inp += len;
    }
    for(int i = 0;i < station_num;++i){
      stations[i].setInp(inp);
      len = skipWhiteSpace(inp);
      inp += len;
    }
    if_delete = false;

  }

  void setOrder(short ord){
    order = ord;
  }
  void print(char *ret) {
    sprintf(ret,"%s %s %d %d",name,catalog,station_num,price_num);
    for(int i = 0;i < price_num;++i)
      sprintf(ret," %s",pricename[i]);
    sprintf(ret,"\n");
    for(int i = 0;i < station_num;++i){
      char arri[6],st[6],stop[6];
      intToTime(stations[i].arrive,arri);
      intToTime(stations[i].start,st);
      intToTime(stations[i].stopover,stop);
      sprintf(ret,"%s %s %s %s",stations[i].station_name,
              arri,st,stop);
      for(int j = 0;j < price_num;++j){
        sprintf(ret," ¥%f",stations[i].price[j]);
        sprintf(ret,"\n");
      }
    }
  }
  int del() {
    if(if_sale || if_delete)
      return 0;
    if_delete = true;
    if_sale = false;
    return 1;
  }
  int refund(char *inp) {

  }
  int buy(char *inp) {

  }

  ~Train_val(){
    delete []stations;
  }

};
}
#endif //BACKEND_TRAIN_VAL_H


