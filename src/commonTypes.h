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

using namespace std;

Posicion sumarPosiciones(const Posicion & a, const Posicion & b);
Posicion duplicarDireccion(const Posicion & a);

Posicion duplicarDireccion(const Posicion & a) {
  return make_tuple(get < 0 > (a) * 2, get < 1 > (a) * 2);
}

Posicion sumarPosiciones(const Posicion &a, const Posicion &b) {
  return make_tuple(get < 0 > (a) + get < 0 > (b), get < 1 > (a) + get < 1 > (b));
}

#endif