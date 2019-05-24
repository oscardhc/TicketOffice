
//
// Created by 傅凌玥 on 2019/5/8.
//

#ifndef STATION_VAL
#define STATION_VAL

#include "utility.hpp"
#include "constant.hpp"
#include <cstring>
namespace sjtu{
    class station_val{
    public:
        //char name[NAME_SIZE];
        unsigned int passby_train[186] = {0};//one bit save whether a train pass-by

        station_val() = default;
        void add(int train_num){
            int index = train_num >> 5;
            int bit = train_num & 31u;
            passby_train[index] |= (1u << bit);
            //TODO need verify
        }

        void del(int train_num){
            int index = train_num >> 5;
            int bit = train_num & 31u;
            passby_train[index] &= (~(1u << bit));
            //TODO need verify
        }

        bool getval(int train_num){
            int index = train_num >> 5;
            int bit = train_num & 31u;
            return (passby_train[index] >> bit) & 1;
            //TODO need verify
        }



    };
}

#endif