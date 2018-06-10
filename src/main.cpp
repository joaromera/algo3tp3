#include <fstream>
#include <stdlib.h>
#include <string>

#include "commonTypes.h"
#include "jugador.h"

using namespace std;

typedef std::vector < Jugador > Equipo;
typedef std::vector < int > Arco;
typedef std::tuple < int, int > Marcador;

vector < int > moviemientos = [(0, 0), (-1, -1), (-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1), (0, -1)];

enum Resultado {
    Gana,
    Empate,
    Pierde
};

bool Equipo_tiene_pelota(
	Equipo eq
);
bool Equipo_tiene_pelota(
	Equipo eq
);
int Robar_Pelota_a(
    Jugador & jugador_a,
    Jugador & jugador_b
);
int Pelea_por_pelota(
    Jugador & jugador_a,
    Jugador & jugador_b
);
bool Gana_pelota_jug_a(
    float & prob_jug_a,
    float & prob_jug_b
);
int Gano_pelota(
    Jugador & jugador_a,
    Jugador & jugador_b
);

int main(int argc, char const *argv[]) {

  cout << "hello darwin" << endl;

  return 0;
}

bool Pelota_esta_libre(
	Equipo eq_a,
	Equipo eq_b) {
	
	return !(Equipo_tiene_pelota(eq_a) || Equipo_tiene_pelota(eq_b));
}

bool Equipo_tiene_pelota(
	Equipo eq) {
	
	return eq[0].tiene_pelota || eq[1].tiene_pelota || eq[2].tiene_pelota;
}

int Robar_Pelota_a(
    Jugador & jugador_a,
    Jugador & jugador_b) {
	
	float prob_defensa = (float)(1 - jugador_a.prob_quite);
	
	if (Gana_pelota_jug_a(jugador_b.prob_quite, prob_defensa)) {
		return Gano_pelota(jugador_b, jugador_a);
	} else {
		return jugador_a.id;
	}
}

int Pelea_por_pelota (
    Jugador & jugador_a,
    Jugador & jugador_b) {
		
	float total = jugador_a.prob_quite + jugador_b.prob_quite;
	float prob_gana_jug_a = jugador_a.prob_quite / total;
	
	float resultado = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	
	if (Gana_pelota_jug_a(jugador_a.prob_quite, jugador_b.prob_quite)) {
		return Gano_pelota(jugador_a, jugador_b);
	} else {
		return Gano_pelota(jugador_b, jugador_a);
	}
}

bool Gana_pelota_jug_a(
    float & prob_jug_a,
    float & prob_jug_b) {
		
	float total = prob_jug_a + prob_jug_b;
	float prob_gana_jug_a = prob_jug_a / total;
	
	float resultado = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	
	return resultado <= prob_gana_jug_a;
}

int Gano_pelota(
    Jugador & jugador_a,
    Jugador & jugador_b) {
		
	jugador_a.Interceptar_pelota();
	jugador_b.Perdio_pelota();
	return jugador_a.id;
}
