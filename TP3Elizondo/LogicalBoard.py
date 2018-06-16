# -*- coding: utf-8 -*-
from constants import *
from random import uniform
from math import floor
from Helpers import *

moves = [(0, 0), (-1, -1), (-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1), (0, -1)]

# moves = [(player_id, move_type, value)] 
# value en [0, ..., 8] o (dir, steps) con dir en [0, ..., 7] y step en [0, ..., inf]

class Player:

    def __init__(self, player_id, p_quite=0.5):
        self.id = player_id
        self.p_quite = p_quite
        self.old_position = None
        self.i = None
        self.j = None
        self.ball = None

    def move(self, direction):
        self.old_position = (self.i, self.j)
        self.i += moves[direction][0]
        self.j += moves[direction][1]
        self.moveBall()

    def backwardMove(self, direction):
        self.old_position = (self.i, self.j)
        self.i -= moves[direction][0]
        self.j -= moves[direction][1]
        self.moveBall()

    def undoMove(self):
        if self.old_position is None:
            return # no hay previa jugada a donde ir
        
        self.i = self.old_position[0]
        self.j = self.old_position[1]
        self.moveBall()
        # solo guarda una jugada, 
        # si quieren deshacer mas, se rompe a proposito
        self.old_position = None

    def moveBall(self):
        if not self.ball is None:
            self.ball.i = self.i
            self.ball.j = self.j

    def takeBall(self, ball):
        self.ball = ball
        self.ball.setMovement(None)
        self.moveBall()

    def __str__(self):
        position = (self.id, self.i, self.j, IN_POSETION)
        if self.ball is None:
            position = (self.id, self.i, self.j, FREE_PLAYER)

        return str(position)


class Ball:

    def __init__(self):
        self.movement = None
        self.i = None
        self.j = None

    def setMovement(self, movement):
        # movement = (dir, steps)
        self.movement = movement

    def move(self):
        if self.movement is None:
            return

        if self.movement[1] > 0:
            move = moves[self.movement[0]] 
            self.i += 2*move[0]
            self.j += 2*move[1]
            self.movement = (self.movement[0], self.movement[1] - 1)

    def finalPosition(self):
        move = moves[self.movement[0]]
        steps = 2*self.movement[1]
        return (self.i + steps*move[0], self.j + steps*move[1])

    def trajectory(self):
        move = moves[self.movement[0]]
        steps = 2*self.movement[1]
        return [(self.i + i*move[0], self.j + i*move[1]) for i in range(steps + 1)]

    def undoMove(self):
        if self.movement is None:
            return

        move = moves[self.movement[0]] 
        self.i -= 2*move[0]
        self.j -= 2*move[1]
        self.movement = (self.movement[0], self.movement[1] + 1)

    def step_back_one(self):
        if self.movement is None:
            return

        move = moves[self.movement[0]] 
        self.i -= move[0]
        self.j -= move[1]

    def __str__(self):
        position = (self.i, self.j)
        if self.movement is not None:
            position = (self.i, self.j, self.movement[0], self.movement[1])

        return str(position)


class LogicalBoard:

    def __init__(self, columns, rows, team_A, team_B):

        assert is_odd(rows) and rows >= 3
        assert is_even(columns) and columns >= 2*rows

        self.score = {A: 0, B: 0} # arrancan 0 - 0
        self.team_A = [Player(p_id, p_quite) for p_id, p_quite in team_A] # jugadores de equipo A
        self.team_B = [Player(p_id, p_quite) for p_id, p_quite in team_B] # jugadores de equipo B
        self.columns = columns
        self.rows = rows

        goal_rows = [int(floor(self.rows / 2)) - 1 + i for i in range(3)]
        self.goal_A = [(i, -1) for i in goal_rows]
        self.goal_B = [(i, self.columns) for i in goal_rows]

        self.free_ball = None
        self.last_state = None

    def isValidTeamMove(self, team, moves):
        valid = True

        # Exactamente un movimiento por jugador del equipo
        player_ids = [p.id for p in team]
        valid = valid and len(moves) == len([p_id for p_id in moves.keys() if p_id in player_ids])
        valid = valid and len(moves) == len(set(moves.keys()))
        valid = valid and len(moves) == len(team)

        # muevo los jugadores
        for p in team:
            player_move = moves[p.id]
            if player_move['move_type'] == MOVIMIENTO: 
                p.move(player_move['value'])
            elif p.ball is None:
                valid = False # Quiere pasar la pelota pero no la tiene
            else:
                # Mirar que el pase es válido: O sea que termina adentro de la cancha, en algún 
                # arco o cruza un arco (ya que va de a dos pasos por vez).
                # Además, no puede ser más largo que M / 2
                valid = valid and player_move['value'][1] <= self.rows / 2

                ball = Ball()
                ball.i = p.i
                ball.j = p.j
                print "is valid move ball"
                print ball
                ball.setMovement(player_move['value'])
                print ball
                ball_trajectory = ball.trajectory()
                print ball_trajectory                
                print "is valid move ball"
                trajectory_in_board = all([self.positionInBoard(t[0], t[1]) for t in ball_trajectory])
                trajectory_in_goal = any([t in self.goal_A + self.goal_B for t in ball_trajectory])
                valid = valid and (trajectory_in_board or trajectory_in_goal)

        # Dos jugadores del mismo equipo estan en la misma posicion
        valid = valid and len(set([(p.i, p.j) for p in team])) == len(team)

        # Todos los jugadores deben estar dentro de la cancha
        for p in team:
            # Tambien puede estar en un arco si es un jugador con pelota
            if p.ball is not None and (p.i, p.j) in self.goal_A + self.goal_B:
                continue
            valid = valid and self.positionInBoard(p.i, p.j)

        # Deshago los movimientos
        for p in team:
            if moves[p.id]['move_type'] == MOVIMIENTO: 
                p.undoMove()

        return valid

    def makeTeamMove(self, team, moves):
        for p in team:
            player_move = moves[p.id]
            if player_move['move_type'] == MOVIMIENTO:
                p.move(player_move['value'])

            # Si el jugador pasó la pelota se setea la dirección y fuerza y se pierde 
            # la poseción, luego el tablero detecta la pelota libre y la mueve en cada paso
            if player_move['move_type'] == PASE:
                self.free_ball = p.ball
                self.free_ball.setMovement(player_move['value'])
                p.ball = None # Ya no posee la pelota

    def normalize(self, prob_1, prob_2):
        total = float(prob_1 + prob_2)
        return float(prob_1) / total, float(prob_2) / total

    def figthBall(self, p_ball, p_empty):
        prob_ball = 1 - p_ball.p_quite # probabilidad de defensa
        prob_empty = p_empty.p_quite # probabilidad de quite

        _, prob_empty = self.normalize(prob_ball, prob_empty)
    
        if uniform(0,1) <= prob_empty:
            p_empty.takeBall(p_ball.ball)
            p_ball.ball = None

    def fairFaightBall(self, p1, p2):
        
        _, prob_p2 = self.normalize(p1.p_quite, p2.p_quite) # ambos usan la probabilidad de quite
        x = uniform(0,1)

        if x < prob_p2:
            p2.takeBall(self.free_ball)
        else:
            p1.takeBall(self.free_ball)

        self.free_ball = None

    # Este metodo asume fuertemente que la pelota todavia no fue actualizada a su nueva posicion
    # y que la pelota esta libre. 
    def intercepted(self, curr_state_player, team):
        result = True

        # Buscar el estado anterior del jugador
        prev_state_player = None
        for p in self.last_state['players'][team]:
            if p.id == curr_state_player.id:
                prev_state_player = p

        # Si se movio no la intercepto
        result = result and prev_state_player.i == curr_state_player.i and prev_state_player.j == curr_state_player.j

        # Si esta en el camino la intercepta
        prev_state_player.backwardMove(self.free_ball.movement[0])
        result = result and prev_state_player.i == self.free_ball.i and prev_state_player.j == self.free_ball.j
        prev_state_player.undoMove()

        return result

    def makeMove(self, moves_A, moves_B):
        assert self.isValidTeamMove(self.team_A, moves_A)
        assert self.isValidTeamMove(self.team_B, moves_B)

        self.last_state = self.getState()

        self.makeTeamMove(self.team_A, moves_A)
        self.makeTeamMove(self.team_B, moves_B)

        # El balon se mueve en la dirección indicada por el ultimo pase
        if self.free_ball is not None:
            # Mira si alguien interceptó la pelota
            intercepters = [p for p in self.team_A if self.intercepted(p, A)] + [p for p in self.team_B if self.intercepted(p, B)]
            
            assert len(intercepters) < 3
            if len(intercepters) == 1:
                intercepters[0].takeBall(self.free_ball)
                self.free_ball = None
            elif len(intercepters) == 2:
                p1, p2 = intercepters
                self.fairFaightBall(p1, p2)
            else:
                self.free_ball.move()
                ball_in_board = self.positionInBoard(self.free_ball.i, self.free_ball.j)
                
                if ball_in_board:
                    # Si hay jugadores en ese casillero, entonces hay que ver si es uno
                    # solo entonces agarra la pelota y si son dos se la disputan
                    players_to_fight = [p for p in self.team_A + self.team_B if p.i == self.free_ball.i and p.j == self.free_ball.j]
                    if len(players_to_fight) == 1:
                        players_to_fight[0].takeBall(self.free_ball)
                        self.free_ball = None
                    elif len(players_to_fight) == 2:
                        p1, p2 = players_to_fight
                        self.fairFaightBall(p1, p2)
                elif is_neighbor((self.free_ball.i, self.free_ball.j), self.goal_A + self.goal_B):
                    # Si la pelota no está en la cancha y es vecina del arco, entonces cruzo el arco 
                    # y quedó atrapada en las redes, por lo que hay que volver un paso atrás.
                    self.free_ball.step_back_one()

        else:
            # Si dos jugadores estan en el mismo casillero y uno tiene la pelota
            # Los mismos se disputan quien termina con la posecion.
            
            # Team A tiene la pelota
            alreadyFight = False
            for p1 in self.team_A:
                if alreadyFight:
                    break

                if p1.ball is not None:
                    for p2 in self.team_B:
                        if p1.i == p2.i and p1.j == p2.j:
                            self.figthBall(p1, p2)
                            alreadyFight = True
                            break

            if not alreadyFight:
                # Team B tiene la pelota
                for p1 in self.team_B:
                    if alreadyFight:
                        break

                    if p1.ball is not None:
                        for p2 in self.team_A:
                            if p1.i == p2.i and p1.j == p2.j:
                                self.figthBall(p1, p2)
                                alreadyFight = True
                                break

        # Si alguien metió gol, tiene que actalizar el tablero y poner los equipos
        # en las posiciones iniciales con el equipo al que le metieron gol sacando
        return self.updateScore()

    def undoMove(self, last_state=None):
        if last_state is None:
            last_state = self.last_state

        if last_state is None:
            return

        self.team_A = last_state['players'][A]
        self.team_B = last_state['players'][B]

        self.free_ball = None
        if last_state['ball'] is not None:
            self.free_ball = Ball()
            self.free_ball.i = last_state['ball'][0] 
            self.free_ball.j = last_state['ball'][1] 
            self.free_ball.movement = last_state['ball'][2:4]

    def winner(self):
        if self.score[A] > self.score[B]:
            return A
        elif self.score[A] < self.score[B]:
            return B
        else:
            return None

    def updateScore(self):
        ball = self.free_ball

        if ball is None:
            for p in self.team_A + self.team_B:
                if not p.ball is None:
                    ball = p.ball

        if (ball.i, ball.j) in self.goal_A:
            self.score[B] += 1
            return A
        elif (ball.i, ball.j) in self.goal_B:
            self.score[A] += 1
            return B

        return None

    def reset(self, position_A, position_B, starting):
        self.startingPositions(position_A, position_B, starting)
        self.score[A] = self.score[B] = 0

    def positionInBoard(self, i, j):
        return 0 <= i and i < self.rows and 0 <= j and j < self.columns 

    def startingPositions(self, position_A, position_B, starting):

        # Mira que las posiciones iniciales sean en el lado correcto de la cancha
        assert all([j < int(floor(self.columns / 2)) for _, j in position_A.values()])
        assert all([j >= int(floor(self.columns / 2)) for _, j in position_B.values()])

        # Saco la pelota del juego
        for p in self.team_A + self.team_B:
            p.ball = None

        self.free_ball = None

        # Coloco los jugadores en las posiciones correctas
        for p, (i, j) in position_A.iteritems():
            self.team_A[p].i = i
            self.team_A[p].j = j

        for p, (i, j) in position_B.iteritems():
            self.team_B[p].i = i
            self.team_B[p].j = j

        # le doy la pelota al jugador que saca y lo pongo en el centro
        if starting == A:
            self.team_A[0].i = int(self.rows / 2)
            self.team_A[0].j = (self.columns / 2) - 1
            self.team_A[0].takeBall(Ball())
        else:
            self.team_B[0].i = int(self.rows / 2)
            self.team_B[0].j = (self.columns / 2)
            self.team_B[0].takeBall(Ball())

    def getState(self):
        ball_position = None
        if self.free_ball is not None:
            ball_position = (self.free_ball.i, self.free_ball.j, self.free_ball.movement[0], self.free_ball.movement[1])
        
        players_a = {p.id: Player(p.id, p.p_quite) for p in self.team_A}
        players_b = {p.id: Player(p.id, p.p_quite) for p in self.team_B}

        for p in self.team_A:
            players_a[p.id].i = p.i
            players_a[p.id].j = p.j
            if p.ball is not None:
                players_a[p.id].takeBall(Ball())

        for p in self.team_B:
            players_b[p.id].i = p.i
            players_b[p.id].j = p.j
            if p.ball is not None:
                players_b[p.id].takeBall(Ball())

        return {
            'players': {
                A: players_a.values(),
                B: players_b.values()
            },
            'ball': ball_position
        }

    def getGoal(self, team):
        if team == A:
            return self.goal_A
        else:
            return self.goal_B

    def __getitem__(self, team_name):
        if team_name == A:
            return self.team_A
        elif team_name == B:
            return self.team_B

    def __str__(self):
        str_positions = ' '.join([str(p) for p in self.team_A + self.team_B])
        if self.free_ball is not None:
            str_positions += ' ' + str(self.free_ball)

        return str_positions
