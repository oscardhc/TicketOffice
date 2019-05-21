#ifndef TRAINTICKET_PRO
#define TRAINTICKET_PRO

#include "constant.hpp"
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
#include <vector>
#include "record.hpp"

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
                DataBase.setElement((char*)(&cnt_train), off, sizeof(int));
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
            DataBase.setElement((char*)(&cnt_train), sizeof(int) * 10, sizeof(int));
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
//            printf("get %d %d\n", index, id);
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

        /*User word:*/
        void execute_register() {
//            printf("--\n");
            char *cur = cmd + commandLength;
            User_val val(cur);
            int id = userCurId++;
            if (id == 2019) {
                val.setPrivilege(2);
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
            int offset = calculateOffset(id, userCurId);
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
            int offset = calculateOffset(id, userCurId);
            if (offset == -1) {
                sprintf(ret, "0");
                return;
            }
            DataBase.getElement((char*)&val, offset, USER_SIZE, USER);
            sprintf(ret, "%s %s %s %d", val.name, val.email, val.phone,
                    val.getPrivilege());
        }
        void execute_modifyProfile() {
            char *cur = cmd + commandLength;
            char id[ID_SIZE];
            int idlen = getNextWord(cur, id);
            cur += idlen;
            int ID = stringToInt(id);
            User_val val(cur);
            int offset = calculateOffset(ID, userCurId);
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
            int offset1 = calculateOffset(ID1, userCurId);
            if (offset1 == -1) {
                sprintf(ret, "0");
                return;
            }
            DataBase.getElement((char*)&val1,offset1,USER_SIZE,USER);
            p_id1 = val1.getPrivilege();
            if (p_id1 == 1 || p_id1 == 0) {
                sprintf(ret, "0");
                return;
            }
            idlen = skipWhiteSpace(cur);
            cur += idlen;
            sscanf(cur, "%d", &privil);
            User_val val2;
            int offset2 = calculateOffset(ID2, userCurId);
            if (offset2 == -1) {
                sprintf(ret, "0");
                return;
            }
            DataBase.getElement((char*)&val2,offset2,USER_SIZE,USER);
            p_id2 = val2.getPrivilege();
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
                    val2.setPrivilege(2);
                    DataBase.setElement((char*)&val2,offset2,USER_SIZE,USER);
                }
                sprintf(ret, "1");
                return;
            }
            sprintf(ret, "0");
        }

        /*Ticket word*/
        void execute_queryTicket()  {
            char *cur = cmd + commandLength;
            char loc1[LOCATION_SIZE], loc2[LOCATION_SIZE];
            char date[DATE_SIZE];
            char catalog[CATALOG_SIZE];
            int len = getNextWord(cur, loc1);
            cur += len;
            len = getNextWord(cur, loc2);
            cur += len;
            len = getNextWord(cur,date);
            cur += len;
            len = getNextWord(cur,catalog);
            cur += len;
            len = (int)strlen(catalog);

            if(dateToInt(date) < 0 || dateToInt(date) > 30){
                sprintf(ret,"-1");
                return;
            }

            int hash1 = getID(loc1), hash2 = getID(loc2);
            int offset1 = stationTree.search(hash1);
            int offset2 = stationTree.search(hash2);
//            fprintf(stderr, "off %d %d       %d %d\n", hash1, offset1, hash2, offset2);
            station_val sta1,sta2;
            DataBase.getElement((char*)&sta1,offset1,STATION_SIZE,STATION);
            DataBase.getElement((char*)&sta2,offset2,STATION_SIZE,STATION);

            int offset;
            int pos1,pos2;//loc1's num in the train

            std::vector<Train_val*> trains;

            for (int i = 0;i < cnt_train; ++i){
                pos1 = sta1.getval(i);
                pos2 = sta2.getval(i);
                //遍历可能会比较慢 可以改进
                if(pos1 && pos2 && pos1 < pos2){
//                    offset = trainTree.search(trainID[i]);
//                    DataBase.getElement((char*)&offset, getTrainID(i), sizeof(int), TRID);
                    offset = getTrainID(i);
//                    fprintf(stderr, "~~~ %d %d\n", i, offset);
                    Train_val *tr = createTrainWithOffset(offset);

                    int j = 0;
                    for(;j < len; ++j){
                        if(catalog[j] == tr->catalog[0])
                            break;
                    }
                    if(j == len)
                        continue;
                    trains.push_back(tr);
                }

            }
            sprintf(ret,"%d\n",trains.size());

            if(!trains.size()){
                sprintf(ret,"0");
                return;
            }

            std::sort(trains.begin(), trains.end(), trainCompare);


            for (int i = 0;i < trains.size(); ++i){
                int start = sta1.getval(trains[i]->order) - 1,end = sta2.getval(trains[i]->order) - 1;
                int trstart = trains[i]->getStation(0)->start;
                int ticketNum;
                float price;

                char time1[8], time2[8];
                intToTime(trains[i]->getStation(start)->start % 1440, time1);
                intToTime(trains[i]->getStation(end)->arrive % 1440, time2);
                char tmpDate[11];

                sprintf(ret + strlen(ret),"%s %s %s %s %s %s %s",
                        trains[i]->trainID,
                        loc1,
                        (trains[i]->getStation(start)->start)>=1440?(intToDate(dateToInt(date)+1,tmpDate),tmpDate):date,
                        time1,
                        loc2,
                        (trains[i]->getStation(end)->arrive)>=1440?(intToDate(dateToInt(date)+1,tmpDate),tmpDate):date,
                        time2);
                for(int k = 0;k < trains[i]->price_num; ++k){
                    price = 0;
                    ticketNum = 2000;

                    for (int j = start + 1; j <= end; ++j){
                        price += trains[i]->getStation(j)->price[k];
                        if (ticketNum > trains[i]->getStation(j)->ticket[dateToInt(date)][k])
                            ticketNum = trains[i]->getStation(j)->ticket[dateToInt(date)][k];
                    }
                    sprintf(ret + strlen(ret)," %s %d %f",trains[i]->pricename[k],ticketNum,price);
                }
                if (i != trains.size() - 1) {
                    sprintf(ret + strlen(ret), "\n");
                }
            }
            for (int i = 0; i < trains.size(); ++i) {
                deleteTrain(trains[i]);
            }
        }
        void execute_queryTransfer() {
            char * cur = cmd + commandLength;
            char loc1[LOCATION_SIZE], loc2[LOCATION_SIZE];
            char dat[DATE_SIZE];
            int date;
            char catalog[CATALOG_SIZE];
            int minTime = 1e9;
            char info[1000];
            info[0] = 0;

            int len = getNextWord(cur,loc1);
            cur += len;
            len = getNextWord(cur,loc2);
            cur += len;
            len = getNextWord(cur,dat);
            cur += len;
            date = dateToInt(dat);
            len = getNextWord(cur,catalog);

            station_val sta1;
            int offset1 = stationTree.search(getID(loc1));
            if (offset1 < 0) {
                sprintf(ret, "-1");
                return;
            }
            DataBase.getElement((char*)&sta1,offset1,STATION_SIZE,STATION);
            station_val sta2;
            int offset2 = stationTree.search(getID(loc2));
            if (offset2 < 0) {
                sprintf(ret, "-1");
                return;
            }
            DataBase.getElement((char*)&sta2,offset2,STATION_SIZE,STATION);
            // traverse all train
            for (int j = 0;j < cnt_train; ++j){
                int ord_end = sta2.getval(j) - 1;
                if (ord_end < 0)
                    continue;
                else{
                    int offset_train2 = getTrainID(j);
//                    Train_val train2;
//                    DataBase.getElement((char*)&train2,offset_train2,TRAIN_SIZE);
                    Train_val *train2 = createTrainWithOffset(offset_train2);
                    int i = 0;
                    for (;i < len; ++i){
                        if(train2->catalog[0] == catalog[i])
                            break;
                    }
                    if(i == len)
                        continue;
                    for (int k = 0;k < ord_end;++k){
                        int loc_transfer = getID(train2->getStation(k)->station_name);
                        station_val sta_transfer;
                        int offset_station_transfer = stationTree.search(loc_transfer);
                        DataBase.getElement((char*)&sta_transfer,offset_station_transfer,STATION_SIZE,STATION);
                        for(int l = 0;l < cnt_train;++l){
                            int ord_start = sta1.getval(l) - 1, ord_transfer1 = sta_transfer.getval(l) - 1;
                            int ord_transfer2 = k;
                            if(ord_start < 0 || ord_transfer1 < 0 ||
                               ord_start > ord_transfer1)
                                continue;
//                            Train_val train1;
                            int offset_train1 = getTrainID(l);
                            Train_val *train1 = createTrainWithOffset(offset_train1);
//                            DataBase.getElement((char*)&train1,offset_train1,TRAIN_SIZE);
                            short transfer_arrive = train1->getStation(ord_transfer1)->arrive;
                            short transfer_start = train2->getStation(ord_transfer2)->start;
                            if (transfer_arrive > transfer_start)
                                continue;
                            int start = train1->getStation(ord_start)->start;
                            int arrive = train2->getStation(ord_end)->arrive;
                            int whole_time =  arrive - start;

                            if(whole_time < minTime){
//                                info = nullptr;
                                char time1[TIME_SIZE],time2[TIME_SIZE];
                                intToTime(start,time1);
                                intToTime(transfer_arrive,time2);
                                sprintf(info + strlen(info),"%s %s %s %s %s %s %s ",train1->trainID,
                                        train1->getStation(ord_start)->station_name,dat,
                                        time1,train1->getStation(ord_transfer1)->station_name,
                                        dat,time2);
                                float price;
                                for (int p = 0;p < train1->price_num;++p){
                                    price = 0;
                                    for (int kk = ord_start + 1; kk <= ord_transfer1; kk++) {
                                        price += train1->getStation(kk)->price[p];
                                    }
                                    sprintf(info + strlen(info),"%s %d %f ",train1->pricename[p],train1->getSurplus(ord_start,ord_transfer1,date,i),
                                            price);
                                }
                                sprintf(info + strlen(info),"\n");

                                intToTime(transfer_start,time1);
                                intToTime(arrive,time2);
                                sprintf(info + strlen(info),"%s %s %s %s %s %s %s ",train2->trainID,
                                        train2->getStation(ord_transfer2)->station_name,dat,
                                        time1,train2->getStation(ord_end)->station_name,
                                        dat,time2);
                                for (int p = 0;p < train1->price_num;++p){
//                                    price = train2->getStation(ord_end)->price[p] - train1->getStation(ord_transfer2)->price[p];
                                    price = 0;
                                    for (int kk = ord_transfer2 + 1; kk <= ord_end; kk++) {
                                        price += train2->getStation(kk)->price[p];
                                    }
                                    sprintf(info + strlen(info),"%s %d %f ",train1->pricename[p],train1->getSurplus(ord_transfer2,ord_end,date,i),
                                            price);
                                }
                            }
                        }
                    }
                }
            }
            if(strlen(info) == 0){
                sprintf(ret,"-1");
                return;
            }
            sprintf(ret,"%s", info);
            // TODO verify
        }
        void execute_buyTicket() {
            char *cur = cmd + commandLength;
            char user_id[ID_SIZE], train_id[TRAIN_ID_SIZE];
            int num;
            char Num[6];

            int len = getNextWord(cur, user_id);
            cur += len;
            len = getNextWord(cur,Num);
            num = stringToInt(Num);
            cur += len;
            len = getNextWord(cur, train_id);
            cur += len;
            int hashid = getID(train_id);
            int offset = trainTree.search(hashid);
            if(offset == -1){
                sprintf(ret,"0");
                return;
            }

            Train_val *val = createTrainWithOffset(offset);
            if (!val->if_sale || val->if_delete) {
                sprintf(ret, "0");
                return;
            }
            float price = val->buy(num, cur, stringToInt(user_id), hashid);
            if (price < 0) {
                deleteTrain(val);
                sprintf(ret, "0");
                return;
            }
            DataBase.setElement((char*)val, offset, TRAIN_SIZE + val->station_num * LOC_SIZE, TRAIN);
            deleteTrain(val);
            sprintf(ret, "1");
        }
        void execute_queryOrder() {
            // TODO
            char *cur = cmd + commandLength;
            char user_id[ID_SIZE], date[DATE_SIZE], cata[10];
            int len = getNextWord(cur, user_id);
            cur += len;
            len = getNextWord(cur,date);
            cur += len;
            len = getNextWord(cur,cata);
            cur += len;
            int calalen = strlen(cata);

            int userid = stringToInt(user_id), da = dateToInt(date);
            Record rec;
            int offset = calculateOffset(userid, 999999);
            User_val user;
            int dt = dateToInt(date);

            char tmp[10000];
            int tot = 0;
            tmp[0] = 0;

            DataBase.getElement((char*)&user, offset, USER_SIZE, USER);
            for (int off = user.getFirst(); off; ){
//                printf("!!! %d\n", off);
                DataBase.getElement((char*)&rec, off, RECORD_SIZE, RECORD);
                int flag = 0;
                for (int i = 0; i < calalen; i++) {
                    if (rec.cata == cata[i]) {
                        flag = 1;
                        break;
                    }
                }
//                printf("~~~~~~~ %d %d\n", dt, rec.getDate());
                if (flag == 1 && dt == rec.getDate()) {
                    tot++;
                    int troff = trainTree.search(rec.trainid);
                    Train_val *val = createTrainWithOffset(troff);
                    char ds[11],dt[11];
                    intToTime(val->getStation(rec.getStart())->start, ds);
                    intToTime(val->getStation(rec.getEnd())->arrive, dt);
                    sprintf(tmp + strlen(tmp), "%s %s %s %s %s %s %s ",
                            val->trainID,
                            val->getStation(rec.getStart())->station_name, date, ds,
                            val->getStation(rec.getEnd())->station_name, date, dt
                            );
                    for (int j = 0; j < val->price_num; j++) {
                        float price = 0.0;
                        for (int k = rec.getStart() + 1; k <= rec.getEnd(); k++) {
                            price += val->getStation(k)->price[j];
                        }
                        sprintf(tmp + strlen(tmp), "%s %d %f ", val->pricename[j], j == rec.getType() ? rec.getQuantity() : 0, price);
                    }
                    sprintf(tmp + strlen(tmp), "\n");

                }
                off = rec.nxt;
            }
            tmp[strlen(tmp) - 1] = 0;
            sprintf(ret, "%d\n", tot);
            sprintf(ret + strlen(ret), "%s", tmp);
        }
        void execute_refundTicket() {
            char *cur = cmd + commandLength;
            char user_id[ID_SIZE], train_id[TRAIN_ID_SIZE];
            int num;
            char Num[6];
            int len = getNextWord(cur, user_id);
            cur += len;
            len = getNextWord(cur,Num);
            num = stringToInt(Num);
            cur += len;
            len = getNextWord(cur, train_id);
            cur += len;

            int hashid = getID(train_id);
            int offset = trainTree.search(hashid);
            if(offset == -1){
                sprintf(ret,"0");
                return;
            }
//            Train_val val;
//            DataBase.getElement((char*)&val,offset,TRAIN_SIZE);

            Train_val *val = createTrainWithOffset(offset);

            if (val->refund(num, cur, stringToInt(user_id), hashid)) {//refund include search ticket record
                sprintf(ret, "1");
                return;
            }
            sprintf(ret, "0");
            return;
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
                //TODO: This method can be extremely slow!!!
            }
            cmd[slen - 1] = 0;

            int offset = DataBase.createElement(TRAIN_SIZE + stationNum * LOC_SIZE, TRAIN);
            Train_val *val = createTrain(ID,name,catalog,stationNum,cur,cnt_train++);

            int noff = DataBase.createElement(sizeof(int), TRID);
            DataBase.setElement((char*)&offset, noff, sizeof(int), TRID);
//            fprintf(stderr, "%d %d %d %d\n", cnt_train, getTrainID(cnt_train - 1), noff, offset);
            getTrainID(cnt_train - 1);

            DataBase.setElement((char*)val,offset,TRAIN_SIZE + stationNum * LOC_SIZE, TRAIN);
//            fprintf(stderr, "%d %d         %d %d %d\n", hashid, offset, NODE_SIZE, sizeof(LeafNode), sizeof(InterNode));
            trainTree.insert(hashid,offset);

            station_val tmp;

            for (int i = 0; i < val->station_num; i++) {
                int id = getID(val->getStation(i)->station_name);
                if (stationTree.search(id) == -1) {
                    int off = DataBase.createElement(STATION_SIZE, STATION);
//                    fprintf(stderr, "new for %s %d %d\n", val->getStation(i)->station_name, id, off);
                    DataBase.setElement((char*)&tmp, off, STATION_SIZE, STATION);
                    stationTree.insert(id, off);
                }
            }
            deleteTrain(val);
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
//            printf("%d %d %d %d     ", troffset, val->if_sale, val->if_delete, val->station_num);

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
                deleteTrain(val);
                sprintf(ret, "1");
                return;
            } else {
                deleteTrain(val);
                sprintf(ret, "0");
            }
        }
        void execute_queryTrain() {
            char *cur = cmd + commandLength;
            char ID[TRAIN_ID_SIZE];
            int len = getNextWord(cur, ID);
            int hashid = getID(ID);
            int offset = trainTree.search(hashid);
            if(offset == -1) {
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
