# TO DO

Agreguen lo que haga falta o pongan su nombre al lado de la tarea de la que se quieren hacer cargo. Las dudas que vayan sumando agreguenlas al final para enviar un mail cuando haya varias.

## Referí que haga jugar y devuelva ganador (referee.hpp) (Adrián)

- [ ]  Do it. Code it. Test it.

## Greedy Player (greedy_player.hpp)

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

## Correcciones mail de Carolina

- **Cata** [ ] El trabajo debe ser autocontenido. En ese sentido, no debe hacerse 
referencia al enunciado. Por lo tanto es necesaria la descripción del 
juego y la definición de los términos utilizados (como "tablero").
- [ ] En general, a lo largo del trabajo, es importante mantener una 
terminología consistente. Por ejemplo: en la introducción se menciona a 
un "jugador goloso", y como el término "jugador" puede interpretarse 
como un integrante del equipo, quizás sea más apropiado hablar de 
"equipo goloso". (Deben corregir también todos los demás errores de este 
estilo que pueda haber.)
- [ ] Los comentarios con respecto al código no corresponden al informe. Por 
ejemplo, la sección 1.2. debería omitirse.
- [ ] En las descripciones de los algoritmos y en los pseudocódigos, es 
recomendable evitar la escritura al estilo de un lenguaje de 
programación. Por ejemplo: "for player in updatedBoard.team do" podría 
reemplazarse por "para todo jugador del equipo hacer"; expresiones como 
"weight[i]" podrían escribirse como $w_x$, indicando qué significa este 
término; los parámetros que recibe la función del pseudocódigo de la 
página 3 se puede escribir de forma similar a "Entrada: un tablero 
currentBoard y los pesos de los parámetros ($w_x$, donde x ...)".
También evitar el llamado a funciones (o la referencia a variables) que 
no están explicadas. Por ejemplo, "if weHaveTheBall() then" podría ser 
"si el equipo tiene la pelota entonces".
- [ ] Las secciones 2.3. y 3(completa) no se entienden con claridad.
- [ ] En la sección 3, primer párrafo: ¿qué es una solución óptima en este 
caso? ¿por qué esta técnica aseguraría obtenerla?
- [ ] En la sección 4.1, segundo párrafo, dice que la población inicial se 
elige aleatoriamente. Esto no es siempre así. Algunas aplicaciones 
buscan una población inicial que cumpla ciertas características.
- [ ] A lo largo de toda la sección 4, faltan citas a artículos u otras 
fuentes.
- [ ] En 4.2, poner algún ejemplo más concreto sería enriquecedor.
- [ ] Para las referencias, sugiero seguir un formato parecido al siguiente: 
A. A. Author, "Document title", \textit{Webpage name}, Date of internet 
publication. [Format]. Available: internet address. [Accessed: Date of 
access]. Por ejemplo: Global Health Observatory. "Violence against 
women", \textit{World Health Organization}. [Online]. Disponible en: 
http://www.who.int/gho/women_and_health/violence. [Último acceso: 6 de 
julio de 2018].
- [ ] Evitar el uso de fuentes como Wikipedia
- [ ] A lo largo de todo el trabajo se utiliza mucho lenguaje informal, hay 
errores de tipeo y la redacción no es buena. Por ejemplo: la frase "es 
muy caótico donde todo sube y baja", el segundo ítem de la página 7, 
etc.