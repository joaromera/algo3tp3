# -*- coding: utf-8 -*-
from constants import *
from LogicalBoard import LogicalBoard, moves
import math
from random import choice

def norm(x): 
    return math.sqrt(sum(i**2 for i in x))

# TODO: agregar pases como posibles movimientos
def generateMoves(team):
    generated_moves = [] 
    directions = range(len(moves))
    p1, p2, p3 = team
    for d1 in directions:
        for d2 in directions:
            for d3 in directions:
                generated_moves.append({
                    p1.id: {'move_type': MOVIMIENTO, 'value': d1},
                    p2.id: {'move_type': MOVIMIENTO, 'value': d2},
                    p3.id: {'move_type': MOVIMIENTO, 'value': d3}
                })
    return generated_moves

def static_move(team):
    return [{p.id: {'move_type': MOVIMIENTO, 'value': 0} for p in team}]

class GeneticTeam:

    def __init__(self, team, oponent_team, genome):
        self.team = team
        self.oponent_team = oponent_team
        self.genome = genome

        self.players = None
        self.columns = None
        self.rows = None
        self.steps = None
        self.oponent_players = None

    def startGame(self, columns, rows, steps, side, players, oponent_players):            
        self.columns = columns
        self.rows = rows
        self.steps = steps
        self.oponent_players = oponent_players
        self.side = side

        if self.team == A:
            self.logicalBoard = LogicalBoard(columns, rows, players, oponent_players)
        else:
            self.logicalBoard = LogicalBoard(columns, rows, oponent_players, players)

        return self.starting_positions();

    def starting_positions(self):
        column = 0 if self.side == LEFT else self.columns - 1
        return {i: (i, column) for i in range(3)}

    def move(self, board_state):
        self.logicalBoard.undoMove(board_state)

        team_moves = generateMoves(self.logicalBoard[self.team])
        
        # Dejo al equipo contrario sin moverse
        oponent_moves = static_move(self.logicalBoard[self.oponent_team])
        # Si quiero considerar todas las posibilidades, tengo que usar la linea comentada
        # oponent_moves = generateMoves(self.logicalBoard[self.oponent_team])
        
        best_move = []
        best_move_value = None


        # Miro la mejor jugada asumiendo que el oponente se mueve siempre lo peor posible respecto a mi jugada
        for team_move in team_moves:
            if not self.logicalBoard.isValidTeamMove(self.logicalBoard[self.team], team_move):
                continue
            
            move_value = None
            for oponent_move in oponent_moves:
                if not self.logicalBoard.isValidTeamMove(self.logicalBoard[self.oponent_team], oponent_move):
                    continue

                if self.team == A:
                    self.logicalBoard.makeMove(team_move, oponent_move)
                else:
                    self.logicalBoard.makeMove(oponent_move, team_move)

                new_value = self.evaluateBoard()
                if move_value is None or move_value > new_value:
                    move_value = new_value

                self.logicalBoard.undoMove()

            if best_move_value is None or best_move_value < move_value:
                best_move.append(team_move)
                best_move_value = move_value

        # El jugador es medio random, pero esto no es algo que los jugadores geneticos deban considerar
        # Es solo para que sirva mejor como jugador de debuggin
        return choice(best_move[-2:])

    def finished(self, status):
        pass

    def done(self):
        pass

    def evaluateBoard(self):
        res = 0

        # Distancia maxima entre dos objetos en el tablero, sirve para normalizar
        max_distance = float(norm([self.rows, self.columns]))

        # Distancia de la pelota al arco rival y propio
        ball = self.logicalBoard.free_ball;
        if ball is None:
            for p in self.logicalBoard.team_A + self.logicalBoard.team_B:
                if p.ball is not None:
                    ball = p.ball
                    break

        rival_goal_distance = min([norm((g[0] - ball.i, g[1] - ball.j)) for g in self.logicalBoard.getGoal(self.oponent_team)])
        rival_goal_distance = float(max_distance - rival_goal_distance)  / max_distance # Mas lejos mas bajo y normalizado entre 0 y 1
        
        goal_distance = min([norm((g[0] - ball.i, g[1] - ball.j)) for g in self.logicalBoard.getGoal(self.team)])
        goal_distance = float(max_distance - goal_distance) / max_distance # Mas lejos mas bajo y normalizado entre 0 y 1

        res += self.genome[0]*rival_goal_distance
        res += self.genome[1]*goal_distance

        # Poseción de la pelota mia y del rival
        in_posetion = 0.0
        rival_in_posetion = 0.0
        if self.logicalBoard.free_ball is None:
            for p in self.logicalBoard[self.team]:
                if p.ball is not None:
                    in_posetion = 1.0
                    break

            for p in self.logicalBoard[self.oponent_team]:
                if p.ball is not None:
                    rival_in_posetion = 1.0
                    break

        res += self.genome[2]*in_posetion
        res += self.genome[3]*rival_in_posetion

        # Distancia de todos los jugadores a la pelota
        ball_distance = sum([norm((p.i - ball.i, p.j - ball.j)) for p in self.logicalBoard[self.team]])
        ball_distance = float(3*max_distance - ball_distance) / float(3*max_distance)
        
        rival_ball_distance = sum([norm((p.i - ball.i, p.j - ball.j)) for p in self.logicalBoard[self.oponent_team]])
        rival_ball_distance = float(max_distance - rival_ball_distance) / max_distance

        res += self.genome[4]*ball_distance
        res += self.genome[5]*rival_ball_distance

        # Dispersion
        dispersion = sum([norm((p1.i - p2.i, p1.j - p2.j)) for p1 in self.logicalBoard[self.team] for p2 in self.logicalBoard[self.team]])
        dispersion = float(dispersion) / float(3*max_distance)

        res += self.genome[6]*dispersion

        # Distancia a los jugadores oponentes (salvo el que tiene la pelota)
        distances_to_oponent = sum([norm((p1.i - p2.i, p1.j - p2.j)) for p1 in self.logicalBoard[self.team] for p2 in self.logicalBoard[self.oponent_team]])
        distances_to_oponent = float(distances_to_oponent) / float(3*max_distance)

        res += self.genome[7]*dispersion

        return res
