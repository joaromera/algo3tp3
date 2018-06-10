#include <fstream>
#include <cstdlib>
#include <string>

// #include "commonTypes.h"
#include "jugador.h"

using namespace std;

typedef std::vector < Jugador > Equipo;
typedef std::vector < int > Arco;
typedef std::tuple < int, int > Marcador;

std::vector<Posicion > movimientos {
  make_tuple(0, 0),
  make_tuple(-1, -1),
  make_tuple(-1, 0),
  make_tuple(-1, 1),
  make_tuple(0, 1),
  make_tuple(1, 1),
  make_tuple(1, 0),
  make_tuple(1, -1),
  make_tuple(0, -1)
};

enum Resultado {
    Gana,
    Empate,
    Pierde
};

bool Equipo_tiene_pelota(Equipo eq);

bool Pelota_esta_libre(Equipo eq_a, Equipo eq_b);

int Robar_Pelota_a(Jugador & jugador_a, Jugador & jugador_b, Pelota & pelota);

int Pelea_por_pelota(Jugador & jugador_a, Jugador & jugador_b, Pelota & pelota);

bool Gana_pelota_jug_a(float & prob_jug_a, float & prob_jug_b);

int Gano_pelota(Jugador & jugador_a, Jugador & jugador_b, Pelota & pelota);

int main(int argc, char const *argv[]) {

  cout << "hello darwin" << endl;

  return 0;
}

bool Pelota_esta_libre(Equipo eq_a, Equipo eq_b) {
	return !(Equipo_tiene_pelota(eq_a) || Equipo_tiene_pelota(eq_b));
}

bool Equipo_tiene_pelota(
	Equipo eq) {
	
	return eq[0].Tiene_pelota()|| eq[1].Tiene_pelota()|| eq[2].Tiene_pelota();
}

int Robar_Pelota_a( Jugador & jugador_a, Jugador & jugador_b, Pelota & pelota) {
	float prob_defensa = (float)(1 - jugador_a.Probabilidad_quite());
	if (Gana_pelota_jug_a(jugador_b.Probabilidad_quite(), prob_defensa)) {
		return Gano_pelota(jugador_b, jugador_a, pelota);
	} else {
		return jugador_a.Id();
	}
}

int Pelea_por_pelota (Jugador & jugador_a, Jugador & jugador_b, Pelota & pelota) {
	float total = jugador_a.Probabilidad_quite() + jugador_b.Probabilidad_quite();
	float prob_gana_jug_a = jugador_a.Probabilidad_quite() / total;
	float resultado = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	
	if (Gana_pelota_jug_a(jugador_a.Probabilidad_quite(), jugador_b.Probabilidad_quite())) {
		return Gano_pelota(jugador_a, jugador_b, pelota);
	} else {
		return Gano_pelota(jugador_b, jugador_a, pelota);
	}
}

bool Gana_pelota_jug_a(float & prob_jug_a, float & prob_jug_b) {
	float total = prob_jug_a + prob_jug_b;
	float prob_gana_jug_a = prob_jug_a / total;
	float resultado = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	
	return resultado <= prob_gana_jug_a;
}

int Gano_pelota(Jugador & jugador_a, Jugador & jugador_b, Pelota & pelota) {

	jugador_a.Interceptar_pelota(pelota);
	jugador_b.Perdio_pelota();
	return jugador_a.Id();
}
