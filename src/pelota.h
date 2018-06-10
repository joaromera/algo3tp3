#ifndef PELOTA_H
#define PELOTA_H

#include "list"
#include "commonTypes.h"

class Pelota {
	Posicion pos;
	Posicion ult_pos;
  list < Posicion > futuras_pos;
	
  public:
    Pelota(Posicion pos_inicial);
    virtual ~Pelota();
    void Avanzar(Posicion direccion);
	  void Interceptada(Posicion nueva_posicion);
    void Seguir_trayectoria();
    void Guardar_trayectoria(Posicion direccion, int fuerza);
	  void Vaciar_trayectoria();
};

#endif