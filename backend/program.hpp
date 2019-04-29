
#include <cstring>
#include <iostream>

class Program{
public:
    static inline void exec(char* cmd, char* ret){
        int len = strlen(cmd);
       	if (cmd[0] == 'l' && cmd[1] == 'o' && cmd[6] == 'a') ret[0] = '1';
       	else ret[0] = '0';
       	ret[1] = 0;
    }

};
