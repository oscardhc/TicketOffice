//
// Created by 傅凌玥 on 2019/5/8.
//


#include "constant.h"
#include "utility.cpp"
#include <cstring>
namespace sjtu {
  class ticket_val {
public:
  bool is_delete;
  int ID;
  char loc1[LOCATION_SIZE], loc2[LOCATION_SIZE];
  char date[DATE_SIZE];
  char time1[TIME_SIZE], time2[TIME_SIZE];
  char catalog[CATALOG_SIZE];
  void del(char *inp){

  }
  void add(char *inp) {};
};
}

