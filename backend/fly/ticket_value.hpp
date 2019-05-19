//
// Created by 傅凌玥 on 2019/5/8.
//


#include "constant.hpp"
#include "utility.hpp"
#include <cstring>
namespace sjtu {
    class ticket_val {
    public:
        bool is_delete;
        int ID;
        int trainID;
        short loc1, loc2;
        char date[DATE_SIZE];
        short price_num, ticket_num;
        char catalog[CATALOG_SIZE];


        void del(){
            is_delete = false;
        }
        void setInput(char *ID, char* Date,char* Catalog, char*p) {

        };
        void print(char*ret) {

        }
    };
}

