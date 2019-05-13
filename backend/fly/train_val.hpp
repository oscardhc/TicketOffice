//
// Created by 傅凌玥 on 2019/5/8.
//

#include "utility.hpp"
#include "constant.h"
#include <string>

namespace sjtu {

class Train_val {

public:
  struct station;
  int station_num = 0, price_num = 0;
  short order;
  char trainID[TRAIN_ID_SIZE];
  char name[NAME_SIZE];
  char catalog[CATALOG_SIZE];
  char *_stations;
  char pricename[5][TICKET_KIND_SIZE];
  bool if_delete = true;
  bool if_sale = true;
  station *stations;
  station *getStation(int index){

  }
  struct station {
    char station_name[LOCATION_SIZE];
    short arrive;
    short start;
    short stopover;
    double price[5];
    int ticket[31][5];//save the ticket on date i from this station to next station
    station() = default;

    void setInp(char *inp, int price_num) {
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
      for(int i = 0;i < price_num;++i){
        len = skipWhiteSpace(inp);
        inp += (len + 1);
        sscanf(inp,"%f",&price[i]);
        if(i > 0)
          price[i] += price[i - 1];
      }
      memset(ticket,MAXTICKET,sizeof(ticket));
    }
    ~station(){

    }
  };
  Train_val (){}
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
      len = getNextWord(inp,pricename[i]);
      inp += len;
    }
    for(int i = 0;i < station_num;++i){
      stations[i].setInp(inp,price_num);
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
  int refund(int num,char *inp) {
    int date;
    char dateStr[DATE_SIZE];
    char sta1[LOCATION_SIZE];
    char sta2[LOCATION_SIZE];
    char ticket_kind[TICKET_KIND_SIZE];
    char *cur = inp;

    int len = getNextWord(cur,sta1);
    cur += len;
    len = getNextWord(cur,sta2);
    cur += len;
    len = getNextWord(cur,dateStr);
    date = dateToInt(dateStr);
    cur += len;
    len = getNextWord(cur,ticket_kind);
    cur += len;

    int i;//check if ticket_kind exists
    for (i = 0;i < price_num;++i)
      if (strcmp(ticket_kind,pricename[i]) == 0) break;
    if (i == price_num) return 0;

    int cnt1 = 0;//check loc1 and loc2 is exist
    while(strcmp(stations[cnt1].station_name,sta1) != 0){
      if (++cnt1 == station_num)
        return 0;
    }
    int cnt2 = cnt1;
    while(strcmp(stations[cnt2].station_name,sta2) != 0){
      if (stations[cnt2].ticket[date][i] < num || ++cnt2 == station_num)
        return 0;
    }

    for(int j = cnt1; j < cnt2; ++j)
      stations[j].ticket[date][i] += num;
    return 1;
  }
  double buy(int num, char *inp) {
    int date;
    char dateStr[DATE_SIZE];
    char sta1[LOCATION_SIZE];
    char sta2[LOCATION_SIZE];
    char ticket_kind[TICKET_KIND_SIZE];
    char *cur = inp;

    int len = getNextWord(cur,sta1);
    cur += len;
    len = getNextWord(cur,sta2);
    cur += len;
    len = getNextWord(cur,dateStr);
    date = dateToInt(dateStr);
    cur += len;
    len = getNextWord(cur,ticket_kind);
    cur += len;

    int i;//check if ticket_kind exists
    for (i = 0;i < price_num;++i)
      if (strcmp(ticket_kind,pricename[i]) == 0) break;
    if (i == price_num) return 0;

    int cnt1 = 0;//check loc1 and loc2 is exist
    while(strcmp(stations[cnt1].station_name,sta1) != 0){
      if (++cnt1 == station_num)
        return 0;
    }
    int cnt2 = cnt1;
    while(strcmp(stations[cnt2].station_name,sta2) != 0){
      if (stations[cnt2].ticket[date][i] < num || ++cnt2 == station_num)
        return 0;
    }
    for(int j = cnt1; j < cnt2; ++j)
      stations[j].ticket[date][i] -= num;
    return stations[cnt2].price[i] - stations[cnt1].price[i];


  }


  ~Train_val(){
    delete []stations;
  }

};
}



