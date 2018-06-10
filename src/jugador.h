#ifndef JUGADOR_H
#define JUGADOR_H

#include "commonTypes.h"
#include "pelota.h"

class Jugador
{
	int id;
	float prob_quite;
	Posicion pos;
	Posicion ult_pos;
	bool tiene_pelota;
	
    public:
        Jugador(int id, float prob_quite, Posicion pos_inicial, bool tiene_pelota);
        virtual ~Jugador();
        Pelota Avanzar(Posicion direccion, Pelota pelota);
        Pelota Patear(Posicion direccion, int fuerza, Pelota pelota);
		Pelota Interceptar_pelota(Pelota pelota);
		void Perdio_pelota();
    protected:
    private:
};

#endif