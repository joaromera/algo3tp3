# Trabajo Práctico #3 - Algoritmos y Estructuras de Datos III

## Darwin DT en el mundial 2018

El proyecto cuenta con un Makefile principal que genera todos los ejecutables necesarios para tener el jugador goloso, jugador estático, test cases, torneos:

- greedy_player 
- static_player 
- tournament 
- logical_board (test)

Además se cuenta con los siguientes programas complementarios a las tareas del torneo:

- *constants.hpp* incluye definiciones de tipos comunes a todos los algoritmos.

- *auxiliars.hpp* incluye funciones de distancia comunes a todos los jugadores.

- *referee.cpp* clase que hace jugar dos equipos e interactúa con la cancha.

- *tournament_player.hpp* jugador de tipo goloso para utilizar en torneos.

---

## Cómo compilar

Se requiere *g++* como compilador. Desde la carpeta donde están los archivos ejecutar para compilar los archivos:

> make

Y para borrarlos:

> make clean

---

## Uso

Detallamos el uso de los programas.

Ofrecemos las siguientes opciones para ejecutar y testear:

> ./logical_board

Programa hecho con la finalidad de testear funcionalidad implementada. Se utiliza la cancha y se usan asserts para asegurar su correctitud.

> ./tournament

Ejecuta un set de algoritmos preconfigurados, tanto ejecuciones de grasp como de genéticos, se guardan sus resultados en archivos de texto y luego se hace un torneo con los ganadores de cada uno. La salida consiste en un archivo de texto con la tabla del último torneo y por salida estándard muestra el ganador principal.

> ./greedy_player

Dicho ejecutable recibe los pesos provenientes de las pruebas y los torneos y es consumido por Elizondo.py al momento de jugar un partido con interfaz.

---
