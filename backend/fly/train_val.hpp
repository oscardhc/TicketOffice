//
// Created by 傅凌玥 on 2019/5/8->
//

#include "utility.hpp"
#include "constant.h"
#include <string>
#include <new>
#include "../whj/bpt_new.hpp"

namespace sjtu {

    struct Train_val {
    public:
        struct station {
            char station_name[LOCATION_SIZE];
            short arrive;
            short start;
            // stopover time can be calculated by arrive and start time
            // and never be used any more
            double price[5];
            short ticket[31][5];//TODO
            // save the ticket on date i from this station to previous station
            station() = default;

            char* setInp(char *inp, int price_num) {
                char time[TIME_SIZE];
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
                int stopover = timeToInt(time);
                fprintf(stderr, "> %d %d %d\n", arrive, start, stopover);
                if (stopover == -1 && arrive != -1) {
                    start = -1;
                }


                for(int i = 0;i < price_num;++i){
                    len = skipWhiteSpace(inp);
                    inp += len;
                    inp += 3;

                    char buff[20];
                    len = getNextWord(inp, buff);
                    inp += len;
                    sscanf(buff, "%lf", &price[i]);

                }
                memset(ticket,MAXTICKET,sizeof(ticket));
                return inp;
            }
            ~station(){}
        };
        int station_num = 0;
        int price_num = 0;
        short order;
        char trainID[TRAIN_ID_SIZE + 1];
        char name[NAME_SIZE + 1];
        char catalog[2];
        char pricename[5][TICKET_KIND_SIZE];
        bool if_delete = true;
        bool if_sale = true;
        station _stations[];
        station *getStation(int index){
            return &_stations[index];
        }
        void init(char* trainid,char* Name,char* Catalog,int stationNum,char *inp,short ord) {
            strcpy(trainID, trainid);
            strcpy(name,Name);
            strcpy(catalog,Catalog);
            station_num = stationNum;
            order = ord;
            int len = 0;
            char priceNum[2];
            len = getNextWord(inp,priceNum);
            inp += len;
            price_num = stringToInt(priceNum);
//            printf("~~~~~~~ %d\n", price_num);

            for(int i = 0;i < price_num;++i){
                len = getNextWord(inp,pricename[i]);
                inp += len;
            }
            for(int i = 0;i < station_num;++i){
                inp = getStation(i)->setInp(inp,price_num);
                len = skipWhiteSpace(inp);
                inp += len;
            }
            if_delete = false;
        }

        void print(char *ret) {
            sprintf(ret,"%s %s %s %d %d",trainID,name,catalog,station_num,price_num);
            for(int i = 0;i < price_num;++i)
                sprintf(ret + strlen(ret)," %s",pricename[i]);
            sprintf(ret + strlen(ret),"\n");
            for(int i = 0;i < station_num;++i){
                char arri[6],st[6],stop[6];
                fprintf(stderr, "%d %d\n", getStation(i)->arrive, getStation(i)->start );
                intToTime(getStation(i)->arrive,arri);
                intToTime(getStation(i)->start == -1 ? getStation(i)->arrive : getStation(i)->start ,st);
                intToTime(getStation(i)->arrive == -1 || getStation(i)->start == -1 ? -1 : getStation(i)->start - getStation(i)->arrive,stop);
                sprintf(ret + strlen(ret),"%s %s %s %s",getStation(i)->station_name,
                        arri,st,stop);
                for(int j = 0;j < price_num;++j){
                    sprintf(ret + strlen(ret)," ￥%f",getStation(i)->price[j]);
                }
                if (i != station_num - 1) sprintf(ret + strlen(ret),"\n");
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
            while(strcmp(getStation(cnt1)->station_name,sta1) != 0){
                if (++cnt1 == station_num)
                    return 0;
            }
            int cnt2 = cnt1;
            while(strcmp(getStation(cnt2)->station_name,sta2) != 0){
                if (++cnt2 == station_num)
                    return 0;
            }

            for(int j = cnt1; j < cnt2; ++j)
                getStation(j)->ticket[date][i] += num;
            //TODO del ticket record
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
            while(strcmp(getStation(cnt1)->station_name,sta1) != 0){
                if (++cnt1 == station_num)
                    return 0;
            }
            int cnt2 = cnt1;
            while(strcmp(getStation(cnt2)->station_name,sta2) != 0){
                if (getStation(cnt2)->ticket[date][i] < num || ++cnt2 == station_num)
                    return 0;
            }
            for(int j = cnt1; j < cnt2; ++j)
                getStation(j)->ticket[date][i] -= num;
            //TODO add ticket record
            return getStation(cnt2)->price[i] - getStation(cnt1)->price[i];

        }
        short getSurplus(int start,int end,int cata,int dat){
            short ret = 2000;
            for (int i = start + 1;i <= end; ++i){
                short surplus = getStation(i)->ticket[dat][cata];
                if ( surplus < ret)
                    ret = surplus;
            }
            return ret;
        }

//        ~Train_val(){
//            //TODO
//        }

    };
    Train_val *createTrainWithOffset(int offset) {
        int stationNum = 0;
        DataBase.getElement((char*)&stationNum, offset, sizeof(int), TRAIN);
        void *ptr = operator new(TRAIN_SIZE + stationNum * LOC_SIZE);
        DataBase.getElement((char*)ptr, offset, TRAIN_SIZE + stationNum * LOC_SIZE, TRAIN);
        return (Train_val*) ptr;
    }
    Train_val *createTrain(char* trainid,char* Name,char* Catalog,int stationNum,char *inp,short ord) {
        void *ptr = operator new(TRAIN_SIZE + stationNum * LOC_SIZE);
        ((Train_val*) ptr)->init(trainid, Name, Catalog, stationNum, inp, ord);
        return (Train_val*) ptr;
    }
    void deleteTrain(Train_val *tr) {
        operator delete((void*)tr);
    }
}



