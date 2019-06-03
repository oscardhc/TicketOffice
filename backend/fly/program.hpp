#ifndef TRAINTICKET_PRO
#define TRAINTICKET_PRO

#include "constant.hpp"
#include "station_val.hpp"
#include "ticket_value.hpp"
#include "train_val.hpp"
#include "user_key.hpp"
#include "utility.hpp"
#include <cstring>
//#include <iostream>
#include "../whj/bpt_new.hpp"
#include "../dhc/IOManager.hpp"
#include "record.hpp"
#include "../dhc/map.hpp"
#include "vector.hpp"
#include <thread>
#include <mutex>

namespace sjtu {
    typedef int ID;
    typedef char *UserInfo;

    std::mutex vecLock;

    void execPart(sjtu::vector<Train_val*> *trains, station_val *sta1, station_val *sta2, char *loc1, char* loc2, int len, char* catalog, int from, int to);

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
        };
        ~Program() {
            DataBase.setElement((char*)(&userCurId), sizeof(int) * 9, sizeof(int));
            DataBase.setElement((char*)(&cnt_train), sizeof(int) * 10, sizeof(int));
        };

        inline void exec(char *_cmd, char *_ret) {
            cmd = _cmd;
            ret = _ret;
            int len = strlen(cmd);
            char word[30];
            commandLength = getNextWord(cmd, word);
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
                execute_queryProfile();
                return;
            } else if (strcmp(word, "modify_profile") == 0) {
                execute_modifyProfile();
                return;
            } else if (strcmp(word, "modify_privilege") == 0) {
                execute_modifyPrivilege();
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

        static inline int getTrainID(int index) {
            int id;
            DataBase.getElement((char*)&id, sizeof(int) + index * sizeof(int) , sizeof(int), TRID);
//            printf("get %d %d\n", index, id);
            return id;
        }

        inline void setPri(int index, unsigned int l) {
            int a = index >> 4;
            int b = index & 15;
            while (quickPri->size() <= a) quickPri->push_back(0u);
            quickPri->at(a) &= (~0u - (3u << (b << 1)));
            quickPri->at(a) += (l << (b << 1));
            User_val tmp;
            DataBase.getElement((char*)&tmp, calculateOffset(index, userCurId), USER_SIZE, USER);
            tmp.setPrivilege(l);
            DataBase.setElement((char*)&tmp, calculateOffset(index, userCurId), USER_SIZE, USER);
        }

        inline int getPri(int index) {
            int a = index >> 4;
            int b = index & 15;
            if (!priInited) {
                int _a = userCurId >> 4;
                fprintf(stderr, "~ %d\n", _a);
                priInited = true;
                quickPri = new sjtu::vector<unsigned int>(0, _a + 10);
                for (int i = 0; i <= _a; i++) {
                    quickPri->push_back(0u);
                }
                fprintf(stderr, "> %d\n", quickPri->size());
            }
            while (quickPri->size() <= a) quickPri->push_back(0u);
            if ((quickPri->at(a) >> (b << 1) & 3u) == 0) {
                User_val tmp;
                DataBase.getElement((char*)&tmp, calculateOffset(index, userCurId), USER_SIZE, USER);
                quickPri->at(a) &= (~0u - (3u << (b << 1)));
                quickPri->at(a) += (tmp.getPrivilege() << (b << 1));
            }
            return quickPri->at(a) >> (b << 1) & 3u;
        }



    private:
        char *cmd;
        char *ret;
        int commandLength;
//        int trainID[MAX_TRAIN_NUM];
        int cnt_train = 0;
        int userCurId;
        bool priInited = false;
//        BPlusTree userTree;
        BPlusTree trainTree;
        BPlusTree stationTree;
        sjtu::vector<unsigned int> *quickPri;


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

            if (ID1 > userCurId || ID2 > userCurId) {
                sprintf(ret, "0");
                return;
            }


            int pr1 = getPri(ID1);
            if (pr1 ^ 2) {
                sprintf(ret, "0");
                return;
            }

//            sscanf(cur, "%d", &privil);
            privil = *(cur+1) - '0';

            int pr2 = getPri(ID2);
            if (pr2 == 2) {
                if (privil == 2) {
                    ret[0] = '1';
                    ret[1] = 0;
                    return;
                } else {
                    ret[0] = '0';
                    ret[1] = 0;
                    return;
                }
            } else {
                if (privil == 2) {
                    setPri(ID2, 2);
                }
                ret[0] = '1';
                ret[1] = 0;
                return;
            }


//            User_val val1;
//            int offset1 = calculateOffset(ID1, userCurId);
//            if (offset1 == -1) {
//                sprintf(ret, "0");
//                return;
//            }
//            DataBase.getElement((char*)&val1,offset1,USER_SIZE,USER);
//            p_id1 = val1.getPrivilege();
//            if (p_id1 == 1 || p_id1 == 0) {
//                sprintf(ret, "0");
//                return;
//            }
//            idlen = skipWhiteSpace(cur);
//            cur += idlen;
//            sscanf(cur, "%d", &privil);
//            User_val val2;
//            int offset2 = calculateOffset(ID2, userCurId);
//            if (offset2 == -1) {
//                sprintf(ret, "0");
//                return;
//            }
//            DataBase.getElement((char*)&val2,offset2,USER_SIZE,USER);
//            p_id2 = val2.getPrivilege();
//            if (p_id2 == 2) {
//                if (privil == 2) {
//                    sprintf(ret, "1");
//                    return;
//                } else {
//                    sprintf(ret, "0");
//                    return;
//                }
//            }
//            if (p_id2 == 1) {
//                if (privil == 2) {
//                    val2.setPrivilege(2);
//                    DataBase.setElement((char*)&val2,offset2,USER_SIZE,USER);
//                }
//                sprintf(ret, "1");
//                return;
//            }
//            sprintf(ret, "0");
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

            int hash1 = getID(loc1), hash2 = getID(loc2);//hashID of loc
            int offset1 = stationTree.search(hash1);
            int offset2 = stationTree.search(hash2);
            station_val sta1,sta2;
            DataBase.getElement((char*)&sta1,offset1,STATION_SIZE,STATION);
            DataBase.getElement((char*)&sta2,offset2,STATION_SIZE,STATION);

            int offset;
            int pos1,pos2;//loc1's num in the train

            sjtu::vector<Train_val*> trains;

            const int THREAD_COUNT = 1;
//            std::thread t[THREAD_COUNT];

//            for (int i = 0; i < THREAD_COUNT; i++) {
//                t[i] = std::thread(execPart, &trains, &sta1, &sta2, loc1, loc2, len, catalog, 186 * i / THREAD_COUNT, 186 * (i+1) / THREAD_COUNT);
//                std::thread(execPart, &trains, &sta1, &sta2, loc1, loc2, len, catalog, 93, 186);
//            }
//            for (int i = 0; i < THREAD_COUNT; i++) {
//                t[i].join();
//            }


            execPart(&trains, &sta1, &sta2, loc1, loc2, len, catalog, 0, 186);
//            execPart(trains, sta1, sta2, loc1, loc2, len, catalog, 0, 100);


            if(!trains.size()){
                sprintf(ret,"0");
                return;
            }

            sprintf(ret,"%d\n",trains.size());
            Quicksort(trains, 0, trains.size() - 1);


            for (int i = 0;i < trains.size(); ++i){
                int start = 0,end = 0;
                for( int j = 0;j < trains[i]->station_num;++j) {
                    if(strcmp(loc1, trains[i]->_stations[j].station_name) == 0)
                        start = j;
                    if(strcmp(loc2, trains[i]->_stations[j].station_name) == 0)
                        end = j;
                }

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
                    sprintf(ret + strlen(ret)," %s %d %.2f",trains[i]->pricename[k],ticketNum,price);
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

            for(int j = 0;j <= (cnt_train >> 5);++j) {//traverse all trains
                int bit_str = sta2.passby_train[j];
                if (bit_str == 0)
                    continue;
                else {
                    for (int bit = 0; bit < 32; ++bit) {
                        if (!bit_str)
                            break;
                        if (!(bit_str & 1)) {
                            bit_str >>= 1;
                            continue;
                        }

                        //find train2 pass by loc2
                        int offset_train2 = getTrainID((j << 5) + bit);
                        Train_val *train2 = createTrainWithOffset(offset_train2);

//                        cerr << "train2:" << train2->trainID << train2->name << endl;//TODO
                        int i = 0;
                        //check catalog exists
                        for (; i < len; ++i) {
                            if (train2->catalog[0] == catalog[i])
                                break;
                        }
                        if (i == len)
                            continue;
//                        cerr << "continue1" << endl;
                        int k;
                        //find loc2's index of trian2's station
                        for (k = 0; k < train2->station_num; ++k) {
                            if (strcmp(train2->_stations[k].station_name, loc2) == 0)
                                break;
                        }

//                        printf("~ %d\n", k);
                        int ord_end = k;
//                        cerr << "ord_end:" << ord_end << endl;
                        if(ord_end == 0 || ord_end == train2->station_num)
                            continue;

                        //traverse 0~k station of train2
                        for (k = 0; k < ord_end; k++) {
                            char *transfer_loc = train2->_stations[k].station_name;
                            int loc_transfer = getID(transfer_loc);
                            station_val sta_transfer;
                            int offset_station_transfer = stationTree.search(loc_transfer);
                            DataBase.getElement((char*)&sta_transfer, offset_station_transfer, STATION_SIZE, STATION);

                            for (int l = 0; l < cnt_train; ++l) {
                                if(!sta1.getval(l))
                                    continue;
                                if(!sta_transfer.getval(l))
                                    continue;
                                int ord_transfer2 = k;
//                                cerr << "ord_transfer2:" << ord_transfer2 << endl;
                                int offset_train1 = getTrainID(l);
                                if(offset_train1 == offset_train2)
                                    continue;

                                Train_val *train1 = createTrainWithOffset(offset_train1);
                                //check catalog exists
                                for (; i < len; ++i) {
                                    if (train1->catalog[0] == catalog[i])
                                        break;
                                }
                                if (i == len)
                                    continue;

//                                cerr << "train1:" << train1->trainID << endl;
                                int ord_start = -1;
                                int ord_transfer1 = -1;
                                for (int u = 0; u < train1->station_num; ++u) {
                                    if (strcmp(train1->_stations[u].station_name, loc1) == 0) {
                                        ord_start = u;
//                                        cerr <<"ord_start:" << ord_start << endl;
                                    }
                                    if (strcmp(train1->_stations[u].station_name, transfer_loc) == 0) {
                                        if (ord_start == -1)
                                            break;
                                        ord_transfer1 = u;
//                                        cerr << "ord_transfer1:" << ord_transfer1 << endl;
                                        break;
                                    }
                                }

                                if (ord_start == -1 || ord_transfer1 == -1 || ord_start == ord_transfer1)
                                    continue;
//                                cerr << "break4" << endl;
                                short transfer_arrive = train1->getStation(ord_transfer1)->arrive;
                                short transfer_start = train2->getStation(ord_transfer2)->start;
                                if (transfer_arrive > transfer_start){
//                                    char time[TIME_SIZE];
//                                   intToTime(transfer_arrive,time);
//                                    cerr << "transfer_arrive" << time <<  endl;
//                                    intToTime(transfer_start,time);
//                                   cerr << "transfer_start" << time <<  endl;
                                    continue;
                                }


                                int start = train1->getStation(ord_start)->start;
                                int arrive = train2->getStation(ord_end)->arrive;
                                int whole_time = arrive - start;
//                                cerr << "whole_time" << whole_time << endl;
                                if (whole_time < minTime) {
//                                    fprintf(stderr, "FIND %d %d %d %d\n", ord_start, ord_transfer1, ord_transfer2, ord_end);
                                    minTime = whole_time;
                                    char time1[TIME_SIZE], time2[TIME_SIZE];
                                    intToTime(start, time1);
                                    intToTime(transfer_arrive, time2);
                                    sprintf(info, "%s %s %s %s %s %s %s ", train1->trainID,
                                            train1->getStation(ord_start)->station_name, dat,
                                            time1, train1->getStation(ord_transfer1)->station_name,
                                            dat, time2);
                                    float price;
                                    for (int p = 0; p < train1->price_num; ++p) {
                                        price = 0;
                                        for (int kk = ord_start + 1; kk <= ord_transfer1; kk++) {
                                            price += train1->getStation(kk)->price[p];
                                        }
                                        sprintf(info + strlen(info),
                                                "%s %d %.2f ",
                                                train1->pricename[p],
                                                train1->getSurplus(ord_start, ord_transfer1, date, i),
                                                price);
                                    }
                                    sprintf(info + strlen(info), "\n");

                                    intToTime(transfer_start, time1);
                                    intToTime(arrive, time2);
                                    sprintf(info + strlen(info), "%s %s %s %s %s %s %s ", train2->trainID,
                                            train2->getStation(ord_transfer2)->station_name, dat,
                                            time1, train2->getStation(ord_end)->station_name,
                                            dat, time2);
                                    for (int p = 0; p < train2->price_num; ++p) {
                                        price = 0;
                                        for (int kk = ord_transfer2 + 1; kk <= ord_end; kk++) {
                                            price += train2->getStation(kk)->price[p];
                                        }
                                        sprintf(info + strlen(info),
                                                "%s %d %.2f ",
                                                train2->pricename[p],
                                                train2->getSurplus(ord_transfer2, ord_end, date, i),
                                                price);
                                    }
//                                    sprintf(info + strlen(info),"\n");
                                }
                            }
                        }
                        bit_str >>= 1;
                    }
                }
            }
            if(strlen(info) == 0){
                sprintf(ret,"-1");
                return;
            }
            sprintf(ret,"%s", info);
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
                // fprintf(stderr, "... %d\n", off);
                DataBase.getElement((char*)&rec, off, RECORD_SIZE, RECORD);
                int flag = 0;
                for (int i = 0; i < calalen; i++) {
                    if (rec.cata == cata[i]) {
                        flag = 1;
                        break;
                    }
                }
                fprintf(stderr, "%d %d\n", dt, rec.getDate());
                if (flag == 1 && dt == rec.getDate() && rec.getQuantity() > 0) {
                    tot++;
                    // fprintf(stderr, ">>> %d %d\n", off, rec.nxt);
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
                        sprintf(tmp + strlen(tmp), "%s %d %.2f ", val->pricename[j], j == rec.getType() ? rec.getQuantity() : 0, price);
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

            Train_val *val = createTrainWithOffset(offset);

            if (val->refund(num, cur, stringToInt(user_id), hashid)) {//refund include search ticket record
                DataBase.setElement((char*)val, offset, TRAIN_SIZE + val->station_num * LOC_SIZE, TRAIN);
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
                    sta.add(val->order);
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
        }

    };

    void execPart(sjtu::vector<Train_val*> *trains, station_val *sta1, station_val *sta2, char *loc1, char* loc2, int len, char* catalog, int from, int to) {
        for (int i = from;i <to; ++i){
            unsigned int bit = sta1->passby_train[i] & sta2->passby_train[i];
            int cnt = 0;
            while(bit > 0){
                if(bit & 1){
                    int trainOrd = i * 32 + cnt;
                    int offset = Program::getTrainID(trainOrd);
                    Train_val *tra = createTrainWithOffset(offset);
                    int j = 0;// check catalog
                    for(;j < len; ++j){
                        if(catalog[j] == tra->catalog[0])
                            break;
                    }
                    if(j == len){
                        bit >>= 1;
                        cnt ++;
                        continue;
                    }
                    int index1 = -1,index2 = -1;
                    for(j = 0;j < tra->station_num;++j) {
                        if(strcmp(loc1, tra->getStation(j)->station_name) == 0)
                            index1 = j;
                        if(strcmp(loc2, tra->getStation(j)->station_name) == 0){
                            if(index1 == -1)
                                break;
                            index2 = j;
                        }
                    }
                    if(index1 == -1 || index2 == -1){
                        deleteTrain(tra);
                        bit >>= 1;
                        cnt ++;
                        continue;
                    }
//                    vecLock.lock();
                    trains->push_back(tra);
//                    vecLock.unlock();
                }
                bit >>= 1;
                cnt ++;
            }
        }
    }
} // namespace sjtu
#endif
