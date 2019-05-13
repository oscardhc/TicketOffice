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
            if (DataBase.createElement(0) == 0) {
                int off = DataBase.createElement(4 * sizeof(int));
                userTree = BPlusTree(off, 1);
                off = DataBase.createElement(4 * sizeof(int));
                trainTree = BPlusTree(off, 1);
                off = DataBase.createElement(4 * sizeof(int));
                stationTree = BPlusTree(off, 1);
            } else {
                int off = 0;
                userTree = BPlusTree(off, 0);
                off = 4 * sizeof(int);
                trainTree = BPlusTree(off, 0);
                off = 8 * sizeof(int);
                stationTree = BPlusTree(off, 0);
            }
        };  // TODO
        ~Program() {
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
                /*invalid word*/
            else {
                sprintf(ret, "invaild word");
            }
            return;
        }

    private:
        char *cmd;
        char *ret;
        int commandLength;
        int trainID[MAX_TRAIN_NUM];
        short cnt_train = 0;
        BPlusTree userTree;
        BPlusTree trainTree;
        BPlusTree stationTree;

    public:
        /*User word:*/
        void execute_register() {
            char *cur = cmd + commandLength;
            User_val val(cur);
            int id = (int)userTree.getsize() + 2019;
            if (id == 2019) {
                val.privilege = 2;
            }
            int offset = DataBase.createElement(USER_SIZE);
            DataBase.setElement((char*)&val, offset, USER_SIZE);
            userTree.insert(id, offset);
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
            int offset = userTree.search(id);
            if (offset == -1) {
                sprintf(ret, "0");
                return;
            }
            DataBase.getElement((char*)&val, offset, USER_SIZE);

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
            User_val val;
            int offset = userTree.search(id);
            if (offset == -1) {
                sprintf(ret, "0");
                return;
            }
            DataBase.getElement((char*)&val, offset, USER_SIZE);
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
            int offset = userTree.search(ID);
            if (offset == -1) {
                sprintf(ret, "0");
                return;
            }
            DataBase.setElement((char*)&val,offset,USER_SIZE - sizeof(int));
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
            int offset1 = userTree.search(ID1);
            if (offset1 == -1) {
                sprintf(ret, "0");
                return;
            }
            DataBase.getElement((char*)&val1,offset1,USER_SIZE);
            p_id1 = val1.privilege;
            if (p_id1 == 1 || p_id1 == 0) {
                sprintf(ret, "0");
                return;
            }
            idlen = skipWhiteSpace(cur);
            cur += idlen;
            sscanf(cur, "%d", &privil);
            User_val val2;
            int offset2 = userTree.search(ID2);
            if (offset2 == -1) {
                sprintf(ret, "0");
                return;
            }
            DataBase.getElement((char*)&val2,offset2,USER_SIZE);
            p_id2 = val2.privilege;
//            printf("> %d %d %d ", p_id1, p_id2, privil);
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
                    DataBase.setElement((char*)&val2,offset2,USER_SIZE);
                }
                sprintf(ret, "1");
                return;
            }
            sprintf(ret, "0");
        }
        /*Ticket word*/
        void execute_queryTicket() {
            // TODO
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
            len = strlen(catalog);

            int hash1 = getID(loc1), hash2 = getID(loc2);
            int offset1 = stationTree.search(hash1);
            int offset2 = stationTree.search(hash2);
            station_val sta1,sta2;
            DataBase.getElement((char*)&sta1,offset1,STATION_SIZE);
            DataBase.getElement((char*)&sta2,offset2,STATION_SIZE);
            int biString1,biString2,biString;
            int cnt = 0;
            for (int i = 0;i < 186;++i){
                biString1 = sta1.passby_train[i];
                biString2 = sta2.passby_train[i];
                biString = biString1 & biString2;
                while(biString){
                    biString = biString & (biString - 1);
                    cnt++;
                }
            }
            sprintf(ret,"%d",cnt);
            Train_val train[cnt];
            int offset;
            int tmp = cnt;
            for(int i = 0;i < MAX_TRAIN_NUM; ++i){
                if(sta1.getval(i) & sta2.getval(i)){
                    offset = trainTree.search(trainID[i]);
                    DataBase.getElement((char*)&train[tmp--],offset,TRAIN_SIZE);
                }
            }
//    sort(train,train + cnt, sortTrain);



        }
        void execute_queryTransfer() {
            // TODO
        }
        void execute_buyTicket() {
            char *cur = cmd + commandLength;
            char user_id[ID_SIZE], train_id[TRAIN_ID_SIZE];
            int num;

            int len = getNextWord(cur, user_id);
            cur += len;
            skipWhiteSpace(cur);
            scanf(cur, "%d", &num);
            len = 0;
            int tmp = num;
            while (tmp) {
                tmp /= 10;
                len++;
            }
            cur += len;
            len = getNextWord(cur, train_id);
            cur += len;
            int hashid = getID(train_id);
            int offset = trainTree.search(hashid);
            Train_val val;
            DataBase.getElement((char*)&val,offset,TRAIN_SIZE);//TODO
            if (!val.if_sale || val.if_delete) {
                sprintf(ret, "0");
                return;
            }
            double price = val.buy(num, cur);
            if (price > 0) {
                // add ticket_value
                sprintf(ret,"1");
                return;
            }
            sprintf(ret, "0");
        }
        void execute_queryOrder() {
            // TODO
        }
        void execute_refundTicket() {
            char *cur = cmd + commandLength;
            char user_id[ID_SIZE], train_id[TRAIN_ID_SIZE];
            int num;

            int len = getNextWord(cur, user_id);
            cur += len;
            skipWhiteSpace(cur);
            scanf(cur, "%d", &num);
            len = 0;
            int tmp = num;
            while (tmp) {
                tmp /= 10;
                len++;
            }
            cur += len;
            len = getNextWord(cur, train_id);
            cur += len;

            int hashid = getID(train_id);
            int offset = trainTree.search(hashid);
            Train_val val;
            DataBase.getElement((char*)&val,offset,TRAIN_SIZE);
            if (!val.if_sale || val.if_delete) {
                sprintf(ret, "0");
                return;
            }
            if (val.refund(num, cur)) {
                // TODO del ticket_value
                sprintf(ret, "1");
            }
            sprintf(ret, "0");
        }
        /*Train word*/
        void execute_addTrain() {
            char *cur = cmd + commandLength;
            char ID[TRAIN_ID_SIZE];
            int len = getNextWord(cur, ID);
            int hashid = getID(ID);

            Train_val val(cur);
            val.setOrder(cnt_train++);
            int offset = DataBase.createElement(TRAIN_SIZE);
            DataBase.setElement((char*)&val,offset,TRAIN_SIZE);
            trainTree.insert(hashid,offset);
            sprintf(ret, "1");
        }
        void execute_saleTrain() {
            char *cur = cmd + commandLength;
            char ID[TRAIN_ID_SIZE];
            int len = getNextWord(cur, ID);
            int hashid = getID(ID);
            int offset = trainTree.search(hashid);
            Train_val val;
            DataBase.getElement((char*)&val, offset,TRAIN_SIZE);
            if (!val.if_sale && !val.if_delete) {
                val.if_sale = true;
                for (int i = 0; i < val.station_num; ++i) {
                    int station_id = getID(val.getStation(i)->station_name);
                    offset = stationTree.search(hashid);
                    station_val sta;
                    DataBase.getElement((char*)&sta,offset,STATION_SIZE);
                    sta.add(hashid);
                    DataBase.setElement((char*)&sta,offset,STATION_SIZE);
                }
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
            Train_val val;
            DataBase.getElement((char*)&val,offset,TRAIN_SIZE);
            if (val.if_sale && !val.if_delete) {
                val.print(ret);
                return;
            }
            sprintf(ret, "0");
        }
        void execute_deleteTrain() {

            char *cur = cmd + commandLength;
            char ID[TRAIN_ID_SIZE];
            int len = getNextWord(cur, ID);
            int hashid = getID(ID);
            int offset = trainTree.search(hashid);
            Train_val val;
            DataBase.getElement((char*)&val,offset,TRAIN_SIZE);

            if (!val.if_sale && !val.if_delete) {
                val.if_delete = true;
                DataBase.setElement((char*)&val,offset,TRAIN_SIZE);
                for (int i = 0; i < val.station_num; ++i) {
                    int station_id = getID(val.getStation(i)->station_name);
                    offset = stationTree.search(station_id);
                    station_val sta;
                    DataBase.getElement((char*)&sta,offset,STATION_SIZE);
                    sta.del(hashid);
                    DataBase.setElement((char*)&sta,offset,STATION_SIZE);
                }
                sprintf(ret, "1");
                return;
            }
            sprintf(ret, "0");
        }
        void execute_modifyTrain() {
            char *cur = cmd + commandLength;
            char ID[TRAIN_ID_SIZE];
            int len = getNextWord(cur, ID);
            int hashid = getID(ID);
            int offset = trainTree.search(hashid);
            Train_val val;
            DataBase.getElement((char*)&val,offset,TRAIN_SIZE);
            for (int i = 0; i < val.station_num; ++i) {
                int station_id = getID(val.getStation(i)->station_name);
                int offset2 = stationTree.search(hashid);
                station_val sta;
                DataBase.getElement((char*)&sta,offset,STATION_SIZE);
                sta.del(hashid);
                DataBase.setElement((char*)&sta,offset,STATION_SIZE);
            }
            Train_val value(cur);
            DataBase.setElement((char*)&value,offset,TRAIN_SIZE);
        }
        /*Administrate*/
        void clean(){
            sprintf(ret, "1");
            return;
        }
        bool sortTrain(Train_val train1, Train_val train2) {
            if(strcmp(train1.trainID, train2.trainID) > 0)
                return false;
            return true;
        }
    };

} // namespace sjtu

#endif
