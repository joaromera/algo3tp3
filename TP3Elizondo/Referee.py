# -*- coding: utf-8 -*-
import sys

from LogicalBoard import *
from GraphicalBoard import *
from constants import *
from time import sleep


class Referee:

    def __init__(self, columns, rows, steps, team_A, team_B, quites_A, quites_B, show_ui=True):

        self.team_A = team_A
        self.team_A.players = zip(range(len(quites_A)), quites_A)
        self.team_B = team_B
        self.team_B.players = zip(range(len(quites_B)), quites_B)

        self.steps = steps

        self.logicalBoard = LogicalBoard(
            columns, 
            rows, 
            self.team_A.players, 
            self.team_B.players
        )

        if show_ui:
            self.graphicalBoard = GraphicalBoard(self.logicalBoard)
        else:
            self.graphicalBoard = None

    def runPlay(self, starting_team, seconds_between_steps=SECONDS_BETWEEN_STEPS, last_graphical_delay=LAST_GRAPHICAL_DELAY):
        self.team_A.starting_positions = self.team_A.startGame(self.logicalBoard.columns,
                                                               self.logicalBoard.rows,
                                                               self.steps,
                                                               SIDE[A],
                                                               self.team_A.players,
                                                               self.team_B.players)
        self.team_B.starting_positions = self.team_B.startGame(self.logicalBoard.columns,
                                                               self.logicalBoard.rows,
                                                               self.steps,
                                                               SIDE[B],
                                                               self.team_B.players,
                                                               self.team_A.players)

        self.logicalBoard.reset(
            self.team_A.starting_positions,
            self.team_B.starting_positions,
            starting_team
        )
        self.update_canvas()
        
        for step in range(self.steps):
            print "Step: ", step
            board_state_A = self.logicalBoard.getState()
            board_state_B = self.logicalBoard.getState()
            moves_A = self.team_A.move(board_state_A)
            moves_B = self.team_B.move(board_state_B)

            starter = self.logicalBoard.makeMove(moves_A, moves_B)

            self.update_canvas()

            if starter is not None:
                sleep(seconds_between_steps)
                self.logicalBoard.startingPositions(
                    self.team_A.starting_positions,
                    self.team_B.starting_positions,
                    starter
                )
                self.update_canvas()

            sleep(seconds_between_steps)

        winner = self.logicalBoard.winner()
        self.team_A.finished(winner)
        self.team_B.finished(winner)
        
        if self.graphicalBoard is not None:
          self.graphicalBoard.finished(winner)

        sleep(last_graphical_delay)
        return winner

    def update_canvas(self):
        if self.graphicalBoard is not None:
            self.graphicalBoard.draw()

    def exit(self):
        self.team_A.done()
        self.team_B.done()
        sys.exit()
