#ifndef TRAINTICKET_PRO
#define TRAINTICKET_PRO

#include "constant.h"
#include "station_val.hpp"
#include "ticket_value.hpp"
#include "train_val.hpp"
#include "user_key.hpp"
#include "utility.hpp"
#include <cstring>
#include <algorithm>
#include <iostream>
#include "../whj/bpt_new.hpp"
#include "../dhc/IOManager.hpp"

namespace sjtu {
    typedef int ID;
    typedef char *UserInfo;

    class Program {

    public:
        Program() {
            if (DataBase.createElement(0) == sizeof(int)) {
                userCurId = 2019, cnt_train = 0;
                int off = DataBase.createElement(4 * sizeof(int));
                trainTree.init(off, 1);
                off = DataBase.createElement(4 * sizeof(int));
                stationTree.init(off, 1);
                off = DataBase.createElement(sizeof(int));
                DataBase.setElement((char*)(&userCurId), off, sizeof(int));
                off = DataBase.createElement(sizeof(int));
                DataBase.setElement((char*)(&userCurId), off, sizeof(int));
            } else {
                int off = sizeof(int);
                trainTree.init(off, 0);
                off += 4 * sizeof(int);
                stationTree.init(off, 0);
                off += 4 * sizeof(int);
                DataBase.getElement((char*)(&userCurId), off, sizeof(int));
                off += sizeof(int);
                DataBase.getElement((char*)(&cnt_train), off, sizeof(int));
            }
        };  // TODO
        ~Program() {
            DataBase.setElement((char*)(&userCurId), sizeof(int) * 9, sizeof(int));
//            fprintf(stderr, ">>>>>> %lf %lf %lf     %lf\n", 1. * mdf / 1000000, 1. * qry / 1000000, 1. * exe / 1000000, 1. * clock() / 1000000);
        }; // TODO

        inline void exec(char *_cmd, char *_ret) {
            cmd = _cmd;
            ret = _ret;
            int len = strlen(cmd);
            char word[30];
            commandLength = getNextWord(cmd, word);
//            printf("cmd is *%s*\n", word);
            if (strcmp(word, "exit") == 0) {
                sprintf(ret, "BYE");
                return;
            } else if (strcmp(word, "clean") == 0) {
                clean();
                return;
            }
                /*User command:*/
            else if (strcmp(word, "register") == 0) {
                execute_register();
                return;
            } else if (strcmp(word, "login") == 0) {
                execute_login();
                return;
            } else if (strcmp(word, "query_profile") == 0) {
                int _t = clock();
                execute_queryProfile();
                qry += (clock() - _t);
                return;
            } else if (strcmp(word, "modify_profile") == 0) {
                execute_modifyProfile();
                return;
            } else if (strcmp(word, "modify_privilege") == 0) {
                int _t = clock();
                execute_modifyPrivilege();
                mdf += (clock() - _t);
                return;
            }
                /*Ticket command:*/
            else if (strcmp(word, "query_ticket") == 0) {
                execute_queryTicket();
                return;
            } else if (strcmp(word, "query_transfer") == 0) {
                execute_queryTransfer();
                return;
            } else if (strcmp(word, "buy_ticket") == 0) {
                execute_buyTicket();
                return;
            } else if (strcmp(word, "query_order") == 0) {
                execute_queryOrder();
                return;
            } else if (strcmp(word, "refund_ticket") == 0) {
                execute_refundTicket();
                return;
            }
                /*Train command:*/
            else if (strcmp(word, "add_train") == 0) {
                execute_addTrain();
                return;
            } else if (strcmp(word, "sale_train") == 0) {
                execute_saleTrain();
                return;
            } else if (strcmp(word, "query_train") == 0) {
                execute_queryTrain();
                return;
            } else if (strcmp(word, "delete_train") == 0) {
                execute_deleteTrain();
                return;
            } else if (strcmp(word, "modify_train") == 0) {
                execute_modifyTrain();
                return;
            }
            else {
                sprintf(ret, "invaild word");
//                exit(0);
            }
            return;
        }
        inline int  getTrainID(int index) {
            int id;
            DataBase.getElement((char*)&id, sizeof(int) + index * sizeof(int) , sizeof(int), TRID);
            return id;
        }



    private:
        char *cmd;
        char *ret;
        int commandLength;
//        int trainID[MAX_TRAIN_NUM];
        int cnt_train = 0;
        int userCurId;
//        BPlusTree userTree;
        BPlusTree trainTree;
        BPlusTree stationTree;

    public:
        int calculateOffset(int index) {
            if (index < userCurId) return sizeof(int) + (index - 2019) * USER_SIZE;
            else return -1;
        }
        /*User word:*/
        void execute_register() {
//            printf("--\n");
            char *cur = cmd + commandLength;
            User_val val(cur);
            int id = userCurId++;
            if (id == 2019) {
                val.privilege = 2;
            }
            int offset = DataBase.createElement(USER_SIZE, USER);
            DataBase.setElement((char*)&val, offset, USER_SIZE, USER);
//            userTree.insert(id, offset);
            sprintf(ret, "%d", id);
        }
        void execute_login() {
            char username[20], password[20];
            char *cur = cmd + commandLength;
            int userlen = getNextWord(cur, username);
            cur += userlen;
            int passlen = getNextWord(cur, password);
            sjtu::User_val val;
            int id = stringToInt(username);
            int offset = calculateOffset(id);
            if (offset == -1) {
                sprintf(ret, "0");
                return;
            }
            DataBase.getElement((char*)&val, offset, USER_SIZE, USER);

            if (strcmp(val.password, password) == 0) {
                sprintf(ret, "1");
                return;
            }
            sprintf(ret, "0");

        }
        void execute_queryProfile() {
            char *cur = cmd + commandLength;
            int id;
            skipWhiteSpace(cur);
            sscanf(cur, "%d", &id);
            static User_val val;
            int offset = calculateOffset(id);
            if (offset == -1) {
                sprintf(ret, "0");
                return;
            }
            DataBase.getElement((char*)&val, offset, USER_SIZE, USER);
            sprintf(ret, "%s %s %s %d", val.name, val.email, val.phone,
                    val.privilege);
        }
        void execute_modifyProfile() {
            char *cur = cmd + commandLength;
            char id[ID_SIZE];
            int idlen = getNextWord(cur, id);
            cur += idlen;
            int ID = stringToInt(id);
            User_val val(cur);
            int offset = calculateOffset(ID);
            if (offset == -1) {
                sprintf(ret, "0");
                return;
            }
            DataBase.setElement((char*)&val,offset,USER_SIZE - sizeof(int), USER);
            sprintf(ret, "1");
        }
        void execute_modifyPrivilege() {
            char *cur = cmd + commandLength;
            char id1[ID_SIZE], id2[ID_SIZE];
            int privil;
            int p_id1, p_id2;
            int idlen = getNextWord(cur, id1);
            cur += idlen;
            idlen = getNextWord(cur, id2);
            cur += idlen;
            int ID1 = stringToInt(id1), ID2 = stringToInt(id2);

            User_val val1;
            int offset1 = calculateOffset(ID1);
            if (offset1 == -1) {
                sprintf(ret, "0");
                return;
            }
            DataBase.getElement((char*)&val1,offset1,USER_SIZE,USER);
            p_id1 = val1.privilege;
            if (p_id1 == 1 || p_id1 == 0) {
                sprintf(ret, "0");
                return;
            }
            idlen = skipWhiteSpace(cur);
            cur += idlen;
            sscanf(cur, "%d", &privil);
            User_val val2;
            int offset2 = calculateOffset(ID2);
            if (offset2 == -1) {
                sprintf(ret, "0");
                return;
            }
            DataBase.getElement((char*)&val2,offset2,USER_SIZE,USER);
            p_id2 = val2.privilege;
            if (p_id2 == 2) {
                if (privil == 2) {
                    sprintf(ret, "1");
                    return;
                } else {
                    sprintf(ret, "0");
                    return;
                }
            }
            if (p_id2 == 1) {
                if (privil == 2) {
                    val2.privilege = 2;
                    DataBase.setElement((char*)&val2,offset2,USER_SIZE,USER);
                }
                sprintf(ret, "1");
                return;
            }
            sprintf(ret, "0");
        }

        /*Ticket word*/
        void execute_queryTicket()  {
//            char *cur = cmd + commandLength;
//            char loc1[LOCATION_SIZE], loc2[LOCATION_SIZE];
//            char date[DATE_SIZE];
//            char catalog[CATALOG_SIZE];
//            int len = getNextWord(cur, loc1);
//            cur += len;
//            len = getNextWord(cur, loc2);
//            cur += len;
//            len = getNextWord(cur,date);
//            cur += len;
//            len = getNextWord(cur,catalog);
//            cur += len;
//            len = (int)strlen(catalog);
//
//            if(dateToInt(date) < 0 || dateToInt(date) > 30){
//                sprintf(ret,"-1");
//                return;
//            }
//            int hash1 = getID(loc1), hash2 = getID(loc2);
//            int offset1 = stationTree.search(hash1);
//            int offset2 = stationTree.search(hash2);
//            station_val sta1,sta2;
//            DataBase.getElement((char*)&sta1,offset1,LOC_SIZE);
//            DataBase.getElement((char*)&sta2,offset2,LOC_SIZE);
//            int cnt = 0;
//
//            int offset;
//            int pos1,pos2;//loc1's num in the train
//            for (int i = 0;i < cnt_train;++i){
//                pos1 = sta1.getval(i);
//                pos2 = sta2.getval(i);
//                if(pos1 && pos2 && pos1 < pos2){
//                    offset = trainTree.search(trainID[i]);
//                    DataBase.getElement((char*)&train[cnt],offset,TRAIN_SIZE);
//                    //TODO(TRAIN_SIZE need verify: get an big big size of data
//                    int j = 0;
//                    for(;j < len; ++j){
//                        if(catalog[j] == train[cnt].catalog[0])
//                            break;
//                    }
//                    if(j == len)
//                        continue;
//                    cnt++;
//                }
//
//            }
//            sprintf(ret,"%d\n",cnt);
//            if(!cnt){
//                sprintf(ret,"0");
//                return;
//            }
//
//            sort(train,train + cnt, sortTrain);
//
//            for (int i = 0;i < cnt; ++i){
//                int start = sta1.getval(train->order) - 1,end = sta2.getval(train->order) - 1;
//                int ticketNum;
//
//                sprintf(ret,"%s %s %s %s %s %s %s",
//                        train[i].trainID,loc1,date,intToTime(train[i].getStation(start)->start),
//                        loc2,date,intToTime(train[i].getStation(end)->arrive));
//                for(int k = 0;k < train[i].price_num; ++k){
//                    ticketNum = 2000;
//                    for (int j = start; j <= end; ++j){
//                        if (ticketNum > train[i].getStation(j)->ticket[dateToInt(date)][k])
//                            ticketNum = train[i].getStation(j)->ticket[dateToInt(date)][k];
//                    }
//                    sprintf(ret," %s %d %f",train[i].pricename[k],ticketNum,
//                            train[i].getStation(end)->price - train[i].getStation(start)->price);
//                }
//                sprintf(ret,"\n");
//            }
        }
        void execute_queryTransfer() {
//            char * cur = cmd + commandLength;
//            char loc1[LOCATION_SIZE], loc2[LOCATION_SIZE];
//            char dat[DATE_SIZE];
//            int date;
//            char catalog[CATALOG_SIZE];
//            int minTime = 1e9;
//            char *info = "";
//
//            int len = getNextWord(cur,loc1);
//            cur += len;
//            len = getNextWord(cur,loc2);
//            cur += len;
//            len = getNextWord(cur,dat);
//            cur += len;
//            date = dateToInt(dat);
//            len = getNextWord(cur,catalog);
//
//            station_val sta1;
//            int offset1 = stationTree.search(getID(loc2));
//            DataBase.getElement((char*)&sta1,offset1,STATION_SIZE);
//            station_val sta2;
//            int offset2 = stationTree.search(getID(loc2));
//            DataBase.getElement((char*)&sta2,offset2,STATION_SIZE);
//            // traverse all train
//            for (int j = 0;j < cnt_train; ++j){
//                int ord_end = sta2.getval(j) - 1;
//                if (ord_end <= 0)
//                    continue;
//                else{
//                    int offset_train2 = trainTree.search(trainID[j]);
//                    Train_val train2;
//                    DataBase.getElement((char*)&train2,offset_train2,TRAIN_SIZE);
//                    int i = 0;
//                    for (;i < len; ++i){
//                        if(train2.catalog[0] == catalog[i])
//                            break;
//                    }
//                    if(i == len)
//                        continue;
//                    for (int k = 0;k < ord_end - 1;++k){
//                        int loc_transfer = getID(train2.getStation(k)->station_name);
//                        station_val sta_transfer;
//                        int offset_station_transfer = stationTree.search(loc_transfer);
//                        DataBase.getElement((char*)&sta_transfer,offset_station_transfer,STATION_SIZE);
//                        for(int l = 0;l < cnt_train;++l){
//                            int ord_start = sta1.getval(l) - 1, ord_transfer1 = sta_transfer.getval(l) - 1;
//                            int ord_transfer2 = k;
//                            if(ord_start <= 0 || ord_transfer1 <= 0 ||
//                               ord_start > ord_transfer1)
//                                continue;
//                            Train_val train1;
//                            int offset_train1 = trainTree.search(trainID[l]);
//                            DataBase.getElement((char*)&train1,offset_train1,TRAIN_SIZE);
//                            short transfer_arrive = train1.getStation(ord_transfer1)->arrive;
//                            short transfer_start = train2.getStation(ord_transfer2)->start;
//                            if (transfer_arrive > transfer_start)
//                                continue;
//                            int start = train1.getStation(ord_start)->start;
//                            int arrive = train2.getStation(ord_end)->arrive;
//                            int whole_time =  arrive - start;
//
//                            if(whole_time < minTime){
//                                info = nullptr;
//                                char time1[TIME_SIZE],time2[TIME_SIZE];
//                                intToTime(start,time1);
//                                intToTime(transfer_arrive,time2);
//                                sprintf(info,"%s %s %s %s %s %s %s ",train1.trainID,
//                                        train1.getStation(ord_start)->station_name,dat,
//                                        time1,train1.getStation(ord_transfer1)->station_name,
//                                        dat,time2);
//                                double price;
//                                for (int p = 0;p < train1.price_num;++p){
//                                    price = train1.getStation(ord_transfer1)->price[p] - train1.getStation(ord_start)->price[p]; //TODO
//                                    sprintf(info,"%s %d %f ",train1.pricename[p],train1.getSurplus(ord_start,ord_transfer1,date,i),
//                                            price);
//                                }
//                                sprintf(info,"\n");
//
//                                intToTime(transfer_start,time1);
//                                intToTime(arrive,time2);
//                                sprintf(info,"%s %s %s %s %s %s %s ",train2.trainID,
//                                        train2.getStation(ord_transfer2)->station_name,dat,
//                                        time1,train2.getStation(ord_end)->station_name,
//                                        dat,time2);
//                                for (int p = 0;p < train1.price_num;++p){
//                                    price = train2.getStation(ord_end)->price[p] - train1.getStation(ord_transfer2)->price[p];
//                                    sprintf(info,"%s %d %f ",train1.pricename[p],train1.getSurplus(ord_transfer2,ord_end,date,i),
//                                            price);
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//            if(info == ""){
//                sprintf(ret,"-1");
//                return;
//            }
//            sprintf(ret,info);
//            // TODO verify
        }
        void execute_buyTicket() {
//            char *cur = cmd + commandLength;
//            char user_id[ID_SIZE], train_id[TRAIN_ID_SIZE];
//            int num;
//            char Num[6];
//
//            int len = getNextWord(cur, user_id);
//            cur += len;
//            len = getNextWord(cur,Num);
//            num = stringToInt(Num);
//            cur += len;
//            len = getNextWord(cur, train_id);
//            cur += len;
//            int hashid = getID(train_id);
//            int offset = trainTree.search(hashid);
//            if(offset == -1){
//                sprintf(ret,"0");
//                return;
//            }
//            Train_val val;
//            DataBase.getElement((char*)&val,offset,TRAIN_SIZE);
//            //TODO(TRAIN_SIZE need verify: get an big big size of data
//            if (!val.if_sale || val.if_delete) {
//                sprintf(ret, "0");
//                return;
//            }
//            double price = val.buy(num, cur);
//            if (price > 0) {
//                sprintf(ret,"1");
//                return;
//            }
//            sprintf(ret, "0");
        }
        void execute_queryOrder() {
            // TODO
        }
        void execute_refundTicket() {
//            char *cur = cmd + commandLength;
//            char user_id[ID_SIZE], train_id[TRAIN_ID_SIZE];
//            int num;
//            char Num[6];
//            int len = getNextWord(cur, user_id);
//            cur += len;
//            len = getNextWord(cur,Num);
//            num = stringToInt(Num);
//            cur += len;
//            len = getNextWord(cur, train_id);
//            cur += len;

//            int hashid = getID(train_id);
//            int offset = trainTree.search(hashid);
//            if(offset == -1){
//                sprintf(ret,"0");
//                return;
//            }
//            Train_val val;
//            DataBase.getElement((char*)&val,offset,TRAIN_SIZE);
//            //TODO(TRAIN_SIZE need verify: get an big big size of data
//            if (!val.if_sale || val.if_delete) {
//                sprintf(ret, "0");
//                return;
//            }
//            if (val.refund(num, cur)) {//refund include search ticket record
//                sprintf(ret, "1");
//            }
//            sprintf(ret, "0");
        }
        /*Train word*/
        void execute_addTrain() {
            char *cur = cmd + commandLength;
            char ID[TRAIN_ID_SIZE + 1];
            int len = getNextWord(cur, ID);
            cur += len;
            int hashid = getID(ID);
            char station_num[2 + 1];
            char name[NAME_SIZE + 1];
            char catalog[1 + 1];
            short stationNum;

            len = getNextWord(cur, name);
            cur += len;
            len = getNextWord(cur, catalog);
            cur += len;
            len = getNextWord(cur, station_num);
            stationNum = (short)stringToInt(station_num);
            cur += len;

            int slen = strlen(cmd);
            cmd[slen] = ' ';
            slen++;
            for (short i = 0; i < stationNum; i++) {
                fgets(cmd + slen, 200, stdin);
                slen = strlen(cmd);
                cmd[slen - 1] = ' ';
                //TODO: This method maybe extremely slow!!!
            }
            cmd[slen - 1] = 0;

            int offset = DataBase.createElement(TRAIN_SIZE + stationNum * LOC_SIZE, TRAIN);
            Train_val *val = createTrain(ID,name,catalog,stationNum,cur,cnt_train++);

            int noff = DataBase.createElement(sizeof(int), TRID);
            DataBase.setElement((char*)&offset, noff, sizeof(int), TRID);

            DataBase.setElement((char*)val,offset,TRAIN_SIZE + stationNum * LOC_SIZE, TRAIN);
            trainTree.insert(hashid,offset);

            for (int i = 0; i < val->station_num; i++) {
                int id = getID(val->getStation(i)->station_name);
                if (stationTree.search(id) == -1) {
                    int off = DataBase.createElement(STATION_SIZE, STATION);
//                    printf("new for %s %d!\n", val->getStation(i)->station_name, off);
                    stationTree.insert(id, off);
                }
            }

            sprintf(ret, "1");
        }
        void execute_saleTrain() {
            char *cur = cmd + commandLength;
            char ID[TRAIN_ID_SIZE];
            int len = getNextWord(cur, ID);
            int hashid = getID(ID);
            int troffset = trainTree.search(hashid);
//            printf("~~~~~ %d %d\n", troffset, hashid);
            if(troffset == -1) {
                sprintf(ret,"0");
                return;
            }

            Train_val *val = createTrainWithOffset(troffset);

            if (!val->if_sale && !val->if_delete) {
                val->if_sale = true;
                station_val sta;
                for (int i = 0;i < val->station_num; ++i){
                    int offset = stationTree.search(getID(val->getStation(i)->station_name));
                    DataBase.getElement((char*)&sta,offset,STATION_SIZE,STATION);
                    sta.add(val->order, i + 1);
                    DataBase.setElement((char*)&sta,offset,STATION_SIZE,STATION);
                }
                DataBase.setElement((char*)val, troffset, TRAIN_SIZE + val->station_num * LOC_SIZE, TRAIN);
                sprintf(ret, "1");
                return;
            } else
                sprintf(ret, "0");
        }
        void execute_queryTrain() {
            char *cur = cmd + commandLength;
            char ID[TRAIN_ID_SIZE];
            int len = getNextWord(cur, ID);
            int hashid = getID(ID);
            int offset = trainTree.search(hashid);
            if(offset == -1){
                sprintf(ret,"0");
                return;
            }
            Train_val *val = createTrainWithOffset(offset);
            if (val->if_sale && !val->if_delete) {
                val->print(ret);
                return;
            }
            sprintf(ret, "0");
        }
        void execute_deleteTrain() {
            char *cur = cmd + commandLength;
            char ID[TRAIN_ID_SIZE];
            int len = getNextWord(cur, ID);
            int hashid = getID(ID);
            int troffset = trainTree.search(hashid);
            if(troffset == -1){
                sprintf(ret,"0");
                return;
            }
            Train_val *val = createTrainWithOffset(troffset);
            //TODO(TRAIN_SIZE need verify: get an big big size of data
            if (!val->if_sale && !val->if_delete) {
                val->if_delete = true;
                station_val sta;
                for (int i = 0; i < val->station_num; ++i) {
                    int offset = stationTree.search(getID(val->getStation(i)->station_name));
                    DataBase.getElement((char*)&sta,offset,LOC_SIZE);
                    sta.del(val->order);
                    DataBase.setElement((char*)&sta,offset,LOC_SIZE);
                }
                DataBase.setElement((char*)val, troffset, TRAIN_SIZE + val->station_num * LOC_SIZE, TRAIN);
                sprintf(ret, "1");
                return;
            }
            sprintf(ret, "0");
        }
        void execute_modifyTrain() {
            char *cur = cmd + commandLength;
            char ID[TRAIN_ID_SIZE];
            int len = getNextWord(cur, ID);
            cur += len;
            int hashid = getID(ID);
            int offset = trainTree.search(hashid);
            Train_val *val = (offset != -1 ? createTrainWithOffset(offset) : nullptr);
            char station_num[3];
            char name[NAME_SIZE + 1];
            char catalog[2];
            short stationNum;

            len = getNextWord(cur, name);
            cur += len;
            len = getNextWord(cur, catalog);
            cur += len;
            len = skipWhiteSpace(cur);
            cur += len;
            len = getNextWord(cur, station_num);
            stationNum = (short)stringToInt(station_num);
            cur += len;

            int slen = strlen(cmd);
            cmd[slen] = ' ';
            slen++;
            for (short i = 0; i < stationNum; i++) {
                fgets(cmd + slen, 200, stdin);
                slen = strlen(cmd);
                cmd[slen - 1] = ' ';
                //TODO: This method maybe extremely slow!!!
            }
            cmd[slen - 1] = 0;

            if(offset == -1){
                sprintf(ret,"0");
                return;
            }
            if (val->if_delete || val->if_sale){
                sprintf(ret,"0");
                return;
            }
            val->del();

            int order = val->order;
            offset = DataBase.createElement(TRAIN_SIZE + stationNum * LOC_SIZE, TRAIN);
            Train_val *value = createTrain(ID,name,catalog,stationNum,cur,order);
            DataBase.setElement((char*)value,offset,TRAIN_SIZE + stationNum * LOC_SIZE, TRAIN);
            trainTree.update(hashid,offset);
            sprintf(ret, "1");
        }
        /*Administrate*/
        void clean(){
            sprintf(ret, "1");
            //TODO
        }
        
    };

} // namespace sjtu

#endif
