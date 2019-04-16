#include <stdio.h>
#include <iostream>
#include "Program.hpp"

int main(){

    Program a;
    std::string cmd;

    while (1) {
        std::cin >> cmd;
        a.exec(cmd);
    }

    return 0;
}
