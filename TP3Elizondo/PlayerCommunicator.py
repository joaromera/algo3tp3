# -*- coding: utf-8 -*-
from subprocess import Popen, PIPE
from constants import *
import os

def parse_positions(positions):
    parsed_positions = {}
    positions = positions.split(' ')

    i = 0
    while i < len(positions):
        parsed_positions[int(positions[i])] = (int(positions[i+1]), int(positions[i+2]))
        i += 3

    return parsed_positions

def parseMove(move):
    parsed_moves = {}
    moves = move.split(' ')
    i = 0
    while i < len(moves):
        player_id = int(moves[i])
        move_type = moves[i+1]
        i += 2
        if move_type == MOVIMIENTO:
            val = int(moves[i])
            i += 1
        else:
            val = (int(moves[i]), int(moves[i+1]))
            i += 2
        parsed_moves[player_id] = {'move_type': move_type, 'value': val}
    return parsed_moves

class PlayerCommunicator:

    def __init__(self, player_executable, params, team, oponent_team):
        self.team = team
        self.oponent_team = oponent_team
        self.player_process = Popen([player_executable] + params, shell=False, stdout=PIPE, stdin=PIPE)
        self.players = None

        if not os.path.isdir('log'):
            os.mkdir('log')
        self.log_file = open('log/player_communicator_' + self.team + '.log', 'a')

    def startGame(self, columns, rows, steps, players, oponent_players):
        
        assert len(players) == len(oponent_players)

        self.log('\n/*************** Nueva partida *****************/' +
                 '\ncolumnas: ' + str(columns) +
                 '\nfilas: ' + str(rows) +
                 '\nduración en movimientos: ' + str(steps) +
                 '\nequipo: ' + str(self.team) +
                 '\nequipo del oponente: ' + str(self.oponent_team) +
                 '\n/**********************************************/')
        

        self.sendArray([columns, rows, steps])
        self.send(SIDE[self.team])
        
        for player in players:
            self.sendArray(player)
        
        for player in oponent_players:
            self.sendArray(player)

        return self.receive(parse_positions)

    def sendBoardState(self, board_state):
        for player in board_state['players'][self.team]:
            position = [player.id, player.i, player.j]
            if player.ball is not None:
                position.append(IN_POSETION)
            else:
                position.append(FREE_PLAYER)
            self.sendArray(position)

        for player in board_state['players'][self.oponent_team]:
            position = [player.id, player.i, player.j]
            if player.ball is not None:
                position.append(IN_POSETION)
            else:
                position.append(FREE_PLAYER)
            self.sendArray(position)

        if board_state['ball'] is not None:
            self.sendArray(board_state['ball'])

    def sendArray(self, array):
        self.send(' '.join(map(str, array)))

    def send(self, msg):
        self.log('envio ' + str(type(msg)) + ': ' + str(msg))
        self.player_process.stdin.write(str(msg) + '\n')
        self.player_process.stdin.flush()

    def receive(self, cast):
        msg = self.player_process.stdout.readline().strip()
        self.log('recibo ' + str(type(msg)) + ': ' + str(msg))
        return cast(msg)

    def move(self, board_state):
        self.sendBoardState(board_state)
        return self.receive(parseMove)

    def finished(self, status):
        self.send(status)

    def done(self):
        self.send(EXIT)
        self.receive(str)
        return True

    def log(self, msg):
        self.log_file.write(str(msg) + "\n")
        self.log_file.flush()
