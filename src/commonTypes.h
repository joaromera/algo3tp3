#ifndef COMMONTYPES_H
#define COMMONTYPES_H

#include <vector>
#include <tuple>
#include <queue>
#include <iostream>
#include <algorithm>
#include <set>
#include <chrono>

typedef std::tuple < int, int > Posicion;

class sumTuple {
    public:
        Posicion operator()(const Posicion & a,
            const Posicion & b) const {
            return make_tuple(get < 0 > (a) + get < 0 > (b), get < 1 > (a) + get < 1 > (b));
        }
};

class duplicarTuple {
    public:
        Posicion operator()(const Posicion & a) const {
            return make_tuple(get < 0 > (a) * 2, get < 1 > (a) * 2);
        }
};

#endif