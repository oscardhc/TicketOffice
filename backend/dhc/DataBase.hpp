//
//  DataBase.hpp
//  Backend
//
//  Created by Haichen Dong on 5/9/19.
//  Copyright © 2018 Haichen Dong. All rights reserved.
//

#ifndef DataBase_hpp
#define DataBase_hpp

#include <stdio.h>
#include <fstream>

class DataBase {
    size_t BaseSize;

public:
    int tot;
    int del[BitSize];

private:
    std::fstream file;

public:
    DataBase() {
    }
    void init(std::string filename) {
    }

    ~DataBase() {
    }

    void getElement(char *t, size_t offset, size_t elementSize) {
    }

    size_t createElement() {
    }

    void setElement(char *t, size_t offset, size_t elementSize) {
    }

};

#endif /* DataBase_hpp */
