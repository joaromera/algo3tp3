# TO DO

Agreguen lo que haga falta o pongan su nombre al lado de la tarea de la que se quieren hacer cargo. Las dudas que vayan sumando agreguenlas al final para enviar un mail cuando haya varias.

## Busqueda Local y GRasp

- [ ] Cómo compite la ganadora con la nueva

- [ ] Poda de 1024 a menos, para hacer competir menos combinaciones/equipos y poder hacer más iteraciones

## Greedy Player (greedy_player.hpp)

- [ ] Ver si es necesario acotar la cantidad de jugadas analizadas (tableros posibles), de acuerdo a lo consultado a Carolina no podemos ver todos los movimientos posibles para cada jugador en cada instancia aun si fuera un movimiento válido porque es muy costoso. Algunas restricciones posibles: si nuestro equipo tiene la pelota solo puntuamos tableros que con movimientos hacia adelante, si no tenemos la pelota solo vamos en direcciones que nos acerquen hacia ese lugar.
- 
## Logical Board (logical_board.hpp)

- [ ] Testear escenario en que hay en la misma posicion dos jugadores de distintos equipos y la pelota. Testar que el movimiento que eligen sea 0 y que el tablero le asigna la pelota a alguno de los dos.

## Consultas para hacer


## Correcciones mail de Carolina

- [ ]  **Cata** El trabajo debe ser autocontenido. En ese sentido, no debe hacerse 
referencia al enunciado. Por lo tanto es necesaria la descripción del 
juego y la definición de los términos utilizados (como "tablero").
- [ ] En general, a lo largo del trabajo, es importante mantener una 
terminología consistente. Por ejemplo: en la introducción se menciona a 
un "jugador goloso", y como el término "jugador" puede interpretarse 
como un integrante del equipo, quizás sea más apropiado hablar de 
"equipo goloso". (Deben corregir también todos los demás errores de este 
estilo que pueda haber.)
- [x] Los comentarios con respecto al código no corresponden al informe. Por 
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