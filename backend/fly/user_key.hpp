//
// Created by 傅凌玥 on 2019/5/5.
//

#ifndef FLY_USER_HPP
#define FLY_USER_HPP

#include "constant.hpp"
#include "utility.hpp"
#include <cstring>

namespace sjtu{
    class User_val{
    public:
        char name[USER_NAME_SIZE];
        char password[PASSWORD_SIZE];
        char email[EMAIL_SIZE];
        char phone[PHONE_SIZE];
        unsigned int _d = 0;

        User_val() {}
        User_val(char * inp){
            int len = getNextWord(inp,name);
            inp += len;
            len = getNextWord(inp,password);
            inp += len;
            len = getNextWord(inp,email);
            inp += len;
            len = getNextWord(inp,phone);
            setPrivilege(1);
            setFirst(0);
        };
        User_val &operator=(const User_val &other) {
            _d = other._d;
            strcpy(name, other.name);
            strcpy(password, other.password);
            strcpy(email, other.email);
            strcpy(phone, other.phone);
            return *this;
        }
        void setInfo(char* inp){
            int len = getNextWord(inp,name);
            inp += len;
            len = getNextWord(inp,password);
            inp += len;
            len = getNextWord(inp,email);
            inp += len;
            len = getNextWord(inp,phone);
        }
        inline unsigned int getPrivilege() {
            return _d & 3;
        }
        inline void setPrivilege(unsigned int _v) {
            _d = (_d >> 2 << 2) + _v;
        }
        inline unsigned int getFirst() {
            return _d >> 2;
        }
        inline void setFirst(unsigned int _v) {
            _d = (_d & 3) + (_v << 2);
        }

    };
}

#endif
