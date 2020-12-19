# -*- coding: utf-8 -*-

from random import choice
import argparse

from LogicalBoard import *
from GraphicalBoard import *
from Referee import *
from PlayerCommunicator import *
from GeneticTeam import GeneticTeam

def main(args):

    print "----- Parámetros -----"
    print "columnas: \t", args.columns
    print "filas: \t\t", args.rows
    print "duración: \t\t", args.steps
    print "----------------------"

    assert len(args.quites_A) == 3
    assert len(args.quites_B) == 3

    if args.team_B is not None:
        executable = args.team_B.pop(0)
        team_B = PlayerCommunicator(executable, args.team_B, B, A)
    else:
        team_B = GeneticTeam(B, A, DEFAULT_GENOME)

    if args.team_A is not None:
        executable = args.team_A.pop(0)
        team_A = PlayerCommunicator(executable, args.team_A, A, B)
    else:
        team_A = GeneticTeam(A, B, DEFAULT_GENOME)
 
    ref = Referee(
        args.columns,
        args.rows,
        args.steps,
        team_A,
        team_B,
        args.quites_A,
        args.quites_B
    )

    if args.first == A:
        ref.runPlay(A)
    elif args.first == B:
        ref.runPlay(B)
    else:
        ref.runPlay(choice([A, B]))

def positive_integer(value):
    ivalue = int(value)
    if ivalue <= 0:
        raise argparse.ArgumentTypeError("%s Valor inválido, tiene que ser un numero natural." % value)
    return ivalue

def probability(value):
    dvalue = float(value)
    if dvalue < 0 or 1 < dvalue:
        raise argparse.ArgumentTypeError("%s Valor inválido, tiene que ser un número en [0,1]." % value)
    return dvalue

if __name__ == '__main__':

    description = 'Versión del laboratorio de AED III del juego cuatro en linea para el TP3.'
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('--team_A',
                        type=str,
                        nargs='*',
                        help='Path al ejecutable del equipo A. Si este parametro no está especificado, se utiliza al jugador de la cátedra para el equipo A')
    parser.add_argument('--quites_A',
                        type=probability,
                        nargs='*',
                        default=DEFAULT_QUITES,
                        help='p_quite de cada jugadore del equipo A, es una lista que debe tener las 3 probabilidades de quites (una por jugador). Si este parametro no está especificado, por defecto vale ' + str(DEFAULT_QUITES))
    parser.add_argument('--team_B',
                        type=str,
                        nargs='*',
                        help='Path al ejecutable del equipo B. Si este parametro no está especificado, se utiliza al jugador de la cátedra para el equipo A')
    parser.add_argument('--quites_B',
                        type=probability,
                        nargs='*',
                        default=DEFAULT_QUITES,
                        help='p_quite de cada jugadore del equipo B, es una lista que debe tener las 3 probabilidades de quites (una por jugador). Si este parametro no está especificado, por defecto vale ' + str(DEFAULT_QUITES))
    parser.add_argument('--columns',
                        type=positive_integer,
                        default=10,
                        help="Cantidad de columnas. Debe ser un número par mayor o igual a 2*--rows. Si no se especifica, por defecto vale 10")
    parser.add_argument('--rows',
                        type=positive_integer,
                        default=5,
                        help="Cantidad de filas. Debe ser un número impar mayor o igual a 5. Si no se especifica, por defecto vale 5")
    parser.add_argument('--steps',
                        type=positive_integer,
                        default=250,
                        help="La duración del partido en cantidad de movimientos. Si no se especifica, por defecto vale 250")
    parser.add_argument('--first',
                        type=str,
                        choices=[A, B],
                        help='Equipo que saca')
    args = parser.parse_args()

    main(args)
