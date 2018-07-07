# TO DO

Agreguen lo que haga falta o pongan su nombre al lado de la tarea de la que se quieren hacer cargo. Las dudas que vayan sumando agreguenlas al final para enviar un mail cuando haya varias.

## Referí que haga jugar y devuelva ganador (referee.hpp) (Adrián)

- [ ]  Do it. Code it. Test it.

## Greedy Player (greedy_player.hpp) (Cata)

- [ ]  Normalizar puntajes. Por ejemplo, es posible que como están las cosas haya situaciones donde no tener la pelota sume más que tenerla simplemente porque hay más criterios. Hacerlo de forma tal que el puntaje mínimo sea 0. O sea que los criterios nunca resten a result.

- [ ] Averiguar máximos y mínimos para los puntajes. Si es todo en base a distancias los puntajes máximos para cada criterio es la distancia máxima del tablero, habría que calcularlo y usar eso.

## Logical Board (logical_board.hpp)

- [X] Uso de Asserts
- [ ] Testearlo y sus subclases, que realice bien los movimientos, las intercepciones de pelotas, el reinicio de partido al haber un gol, etc.

## Grid Search (grid_search.cpp)

- [ ] Hacer generador de combinaciones que se dividan el espacio de soluciones. Por ejemplo, si tuviesemos solo dos pesos y el rango fuese [0,1]. Hacer una función que genere un punto de partida para el grid search con los siguientes vectores: {0,0}, {0.5, 0}, {1, 0}, {0, 0.5}, {0, 1}, {0.5, 1}, {0.5, 0.5}, {1, 0.5}, {1, 1} ... en este ejemplo puse de a 0.5, pero tal vez hay que hacer 0.33 0.66 0.99.

## Consultas para hacer

- [X] Qué combinación inicial usar para grid search? Para búsqueda local? Para grasp?
- [X] Hay que probar distintas posiciones iniciales para los equipos? O podemos definir una.
- [X] Idem para las probabilidades de quite