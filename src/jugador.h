#ifndef JUGADOR_H
#define JUGADOR_H

#include "pelota.h"

class Jugador {
  
  public:  
    Jugador(int id, float prob_quite, Posicion pos_inicial, bool tiene_pelota);
    virtual ~Jugador();

    int Id();
    float & Probabilidad_quite();
    bool Tiene_pelota();
    void Avanzar(Posicion direccion, Pelota & pelota);
    void Patear(Posicion direccion, int fuerza, Pelota & pelota);
    void Interceptar_pelota(Pelota & pelota);
    void Perdio_pelota();

  private:
  	int id;
  	float prob_quite;
    bool tiene_pelota;
    Posicion pos;
    Posicion ult_pos;
  
};

#endif