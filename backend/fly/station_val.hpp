
//
// Created by 傅凌玥 on 2019/5/8.
//


#include "utility.hpp"
#include "constant.h"
#include <cstring>
namespace sjtu{
    class station_val{
    public:
        //char name[NAME_SIZE];
        unsigned long passby_train[1485] = {0};//eight bit save a passby_train station number

        station_val() = default;
        void add(int train_num, int num){
            int index = train_num >> 2;
            int bit = train_num & 3;
            passby_train[index] |= ((255) << (bit * 8));
            passby_train[index] &= (num << (bit * 8));
        }

        void del(int train_num){
            int index = train_num >> 2;
            int bit = train_num & 3;
            for (int i = 0;i<6;++i){
                passby_train[index] &= ~(1 << (bit * 8 + i));
            }
            //TODO need verify
        }

        short getval(int train_num){
            int index = train_num >> 2;
            int bit = train_num & 3;
            return (passby_train[index] >> (bit * 8)) & 255;//TODO need verify
        }



    };
}