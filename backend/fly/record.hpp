//
// Created by Haichen Dong on 2019-05-19.
//

#ifndef FLY_RECORD_HPP
#define FLY_RECORD_HPP

#include "train_val.hpp"


class Record {
    using uint = unsigned int;
public:
    uint trainid;
    uint cata;
    uint nxt;
    uint data;
    inline uint getQuantity() { return data & 2047u; }
    inline uint getType() { return data >> 11 & 7u; }
    inline uint getDate() { return data >> 14 & 31u; }
    inline uint getStart() { return data >> 19 & 63u; }
    inline uint getEnd() { return data >> 25 & 63u; }
    Record() {}
    Record(uint _trainid, uint _cata, uint _nxt, uint _type, uint _date, uint _start, uint _end, uint _qua) {
        trainid = _trainid;
        cata = _cata;
        nxt = _nxt;
        data = _end;
        data = (data << 6) + _start;
        data = (data << 5) + _date;
        data = (data << 3) + _type;
        data = (data << 11) + _qua;
       // printf("INIT %d %d %d %d %d -> %d   %d %d %d\n", _qua, _end, _start, _date, _type, data, trainid, cata, nxt);
    }
    inline void del() {
        data |= (1u << 31);
    }
};

#endif //FLY_RECORD_HPP
