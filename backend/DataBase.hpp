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

#define BitSize (MAX_SIZE/32+1)

template <class T, size_t MAX_SIZE>
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
        tot = 0;
        BaseSize = (1 + BitSize) * sizeof(int);
        memset(del, 0, sizeof(del));
        file = std::fstream(filename, std::ios::app);
        file.close();
        file.clear();
        file = std::fstream(filename, std::ios::in|std::ios::out|std::ios::binary);
        file.seekp(0, std::ios::end);
        if (file.tellg() == 0) {
            file.seekp(0, std::ios::beg);
            file.write((char*)&tot, sizeof(int));
            file.write((char*)del, BitSize * sizeof(int));
        } else {
            file.seekp(0, std::ios::beg);
            file.read((char*)&tot, sizeof(int));
            file.read((char*)del, BitSize * sizeof(int));
        }
    }

    ~DataBase() {
        file.seekp(0, std::ios::beg);
        file.write((char*)&tot, sizeof(int));
        file.write((char*)del, BitSize * sizeof(int));
        file.close();
    }

    void getElement(T *t, size_t offset, size_t elementSize) {
        file.seekp(BaseSize + offset, std::ios::beg);
        file.read((char*)t, elementSize);
    }

    // return 0: ok, -1: error
    int delElement(int index) {
        int i = index / 32;
        int j = index % 32;
        if (del[i] & (1 << j)) {
            del[i] ^= (1 << j);
            return 0;
        } else {
            return -1;
        }
    }

    // return the offset of the appended element
    size_t appElement(T *t, size_t elementSize) {
        file.seekp(0, std::ios::beg);
        size_t pos = file.tellp();
        file.write((char*)t, elementSize);
        return pos;
    }

    void setElement(T *t, size_t offset, size_t elementSize) {
        file.seekp(BaseSize + offset, std::ios::beg);
        file.write((char*)t, elementSize);
    }

};

#endif /* DataBase_hpp */
