#include "jugador.h"

using namespace std;

Jugador::Jugador(int numero, float quite, Posicion pos_inicial, bool empieza_con_pelota) {
  id = numero;
  prob_quite = quite;
  pos = pos_inicial;
  ult_pos = pos_inicial;
  tiene_pelota = empieza_con_pelota;
}

Jugador::~Jugador()
{
  //dtor
}

void Jugador::Avanzar(Posicion direccion, Pelota & pelota) {
  ult_pos = pos;
  pos = sumarPosiciones(pos, direccion);
  if (tiene_pelota) {
    pelota.Avanzar(direccion);
  }
}

void Jugador::Patear(Posicion direccion, int fuerza, Pelota & pelota) {
  pelota.Avanzar(duplicarDireccion(direccion));
  pelota.Guardar_trayectoria(direccion, fuerza);
  tiene_pelota = false;
}

void Jugador::Interceptar_pelota(Pelota & pelota) {
  pelota.Interceptada(pos);
  tiene_pelota = true;
}

void Jugador::Perdio_pelota() { 
  tiene_pelota = false;
}

bool Jugador::Tiene_pelota() {
  return tiene_pelota;
}

float & Jugador::Probabilidad_quite() {
  return prob_quite;
}

int Jugador::Id() {
  return id;
}
