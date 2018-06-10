#include "pelota.h"
#include "commonTypes.h"

using namespace std;

Pelota::Pelota(Posicion pos_inicial)
{
	pos = pos_inicial;
	ult_pos = pos_inicial;
}

Pelota::~Pelota()
{
    //dtor
}

void Pelota::Avanzar(Posicion direccion)
{
	ult_pos = pos;
	pos = sumTuple(pos, direccion);
}

void Pelota::Interceptada(Posicion nueva_posicion)
{
	ult_pos = pos;
	pos = nueva_posicion;
	Vaciar_trayectoria();
}

void Pelota::Seguir_trayectoria()
{
	ult_pos = pos;
	pos = futuras_pos.front();
	futuras_pos.pop_front();
}

void Pelota::Guardar_trayectoria(Posicion direccion, int fuerza)
{	
	Posicion fut_pos = pos;
	for(int i = 1; i < fuerza; ++i) {
		Posicion siguiente_pos = sumTuple(fut_pos, duplicarTuple(direccion));
		futuras_pos.push_back(siguiente_pos);
		fut_pos = siguiente_pos;
	}
}

void Pelota::Vaciar_trayectoria()
{	
	futuras_pos.clear();
}