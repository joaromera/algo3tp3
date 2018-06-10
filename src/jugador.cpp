#include "jugador.h"

using namespace std;

Jugador::Jugador(int numero, float quite, Posicion pos_inicial, bool empieza_con_pelota)
{
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
Pelota Jugador::Avanzar(Posicion direccion, Pelota pelota)
{
	ult_pos = pos;
	pos = sumTuple(pos, direccion);
	if (tiene_pelota) {
		pelota.Avanzar(direccion);
	}
}

Pelota Jugador::Patear(Posicion direccion, int fuerza, Pelota pelota)
{	
	pelota.Avanzar(duplicarTuple(direccion));
	pelota.Guardar_trayectoria(Posicion direccion, int fuerza);
	tiene_pelota = false;
}

Pelota Jugador::Interceptar_pelota(Pelota pelota)
{	
	pelota.Interceptada(pos);
	tiene_pelota = true;
}

void Jugador::Perdio_pelota()
{	
	tiene_pelota = false;
}