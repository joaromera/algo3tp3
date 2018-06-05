# -*- coding: utf-8 -*-
import pygame
from pygame.locals import *
from LogicalBoard import *
from constants import *

clock = pygame.time.Clock()


class GraphicalBoard:

    def __init__(self, logicalBoard):

        pygame.init()
        pygame.display.set_caption('Algoritmos III - Mundial 2018')

        pygame.font.init()
        self.score_font = pygame.font.SysFont('Arial', 30)

        self.logicalBoard = logicalBoard

        self.bg_color = BLACK
        self.stage_width = WINDOWWIDTH + (self.logicalBoard.columns+1) * SPACESIZE
        self.stage_height = WINDOWHEIGHT + (self.logicalBoard.rows+1) * SPACESIZE
        self.canvas = pygame.display.set_mode((self.stage_width, self.stage_height))
        self.margin = { # Esto es para posicionar la cancha en el medio del canvas. Esta posición es el top-left de la cancha no el medio del canvas
            'x': int(WINDOWWIDTH / 2),
            'y': int(WINDOWHEIGHT / 2)
        }

        # Team A graphics
        self.A_token = [{
            BALL: pygame.image.load('img/players/team_A/' + str(i) + '_ball.png'),
            PLAYER: pygame.image.load('img/players/team_A/' + str(i) + '.png'),
            SMALL_PLAYER: pygame.image.load('img/players/team_A/' + str(i) + '_small.png'),
            SMALL_BALL: pygame.image.load('img/players/team_A/' + str(i) + '_small_ball.png')
        } for i in range(3)]
        self.A_token = [{
            BALL: pygame.transform.smoothscale(token[BALL], (SPACESIZE, SPACESIZE)),
            PLAYER: pygame.transform.smoothscale(token[PLAYER], (SPACESIZE, SPACESIZE)),
            SMALL_PLAYER: pygame.transform.smoothscale(token[SMALL_PLAYER], (SPACESIZE, SPACESIZE)),
            SMALL_BALL: pygame.transform.smoothscale(token[SMALL_BALL], (SPACESIZE, SPACESIZE))
        } for token in self.A_token]
        
        # Team B graphics
        self.B_token = [{
            BALL: pygame.image.load('img/players/team_B/' + str(i) + '_ball.png'),
            PLAYER: pygame.image.load('img/players/team_B/' + str(i) + '.png'),
            SMALL_PLAYER: pygame.image.load('img/players/team_B/' + str(i) + '_small.png'),
            SMALL_BALL: pygame.image.load('img/players/team_B/' + str(i) + '_small_ball.png')
        } for i in range(3)]
        self.B_token = [{
            BALL: pygame.transform.smoothscale(token[BALL], (SPACESIZE, SPACESIZE)),
            PLAYER: pygame.transform.smoothscale(token[PLAYER], (SPACESIZE, SPACESIZE)),
            SMALL_PLAYER: pygame.transform.smoothscale(token[SMALL_PLAYER], (SPACESIZE, SPACESIZE)),
            SMALL_BALL: pygame.transform.smoothscale(token[SMALL_BALL], (SPACESIZE, SPACESIZE))
        } for token in self.B_token]
        
        # Free ball graphics
        self.ball_token = pygame.image.load('img/ball.png')
        self.ball_token = pygame.transform.smoothscale(self.ball_token, (SPACESIZE, SPACESIZE))

        # Field graphics
        self.cell_img = {EVEN: pygame.image.load('img/cell_even.png'), ODD: pygame.image.load('img/cell_odd.png')} 
        self.cell_img[EVEN] = pygame.transform.smoothscale(self.cell_img[EVEN], (SPACESIZE, SPACESIZE))
        self.cell_img[ODD] = pygame.transform.smoothscale(self.cell_img[ODD], (SPACESIZE, SPACESIZE))


        # Goals
        self.goals_img = {A: pygame.image.load('img/goal_A.png'), B: pygame.image.load('img/goal_B.png')} 
        self.goals_img[A] = pygame.transform.smoothscale(self.goals_img[A], (SPACESIZE, 3*SPACESIZE))
        self.goals_img[B] = pygame.transform.smoothscale(self.goals_img[B], (SPACESIZE, 3*SPACESIZE))

        # Game result graphics
        # Team A WON
        self.A_won_img = pygame.image.load('img/results/A_won.png')
        self.A_won_rect = self.A_won_img.get_rect()
        self.A_won_rect.center = (int(self.stage_width / 2), int(self.stage_height / 2))

        # Team B WON
        self.B_won_img = pygame.image.load('img/results/B_won.png')
        self.B_won_rect = self.B_won_img.get_rect()
        self.B_won_rect.center = (int(self.stage_width / 2), int(self.stage_height / 2))

        # it's a tie
        self.tie_img = pygame.image.load('img/results/tie.png')
        w, h = self.tie_img.get_size()
        scale = float(self.logicalBoard.columns * SPACESIZE) / float(w)
        self.tie_img = pygame.transform.smoothscale(self.tie_img, (int(scale * w), int(scale * h)))
        self.tie_rect = self.tie_img.get_rect()
        self.tie_rect.center = (int(self.stage_width / 2), int(self.stage_height / 2))

    def draw(self):
        self.canvas.fill(self.bg_color)
        spaceRect = pygame.Rect(0, 0, SPACESIZE, SPACESIZE)

        # Draw the score board
        score_text = str(self.logicalBoard.score[A]) + " - " + str(self.logicalBoard.score[B])
        score_image = self.score_font.render(score_text, True, WHITE)
        score_size = self.score_font.size(score_text)
        spaceRect.topleft = (self.margin['x'] + (self.logicalBoard.columns / 2 * SPACESIZE) - score_size[0] / 2, 50 - score_size[1] / 2)
        self.canvas.blit(score_image, spaceRect)

        # Draw field over the tokens
        current_cell = EVEN
        for x in range(self.logicalBoard.columns):
            for y in range(self.logicalBoard.rows):
                spaceRect.topleft = (self.margin['x'] + (x * SPACESIZE), self.margin['y'] + (y * SPACESIZE))
                self.canvas.blit(self.cell_img[current_cell], spaceRect)
            current_cell = ODD if current_cell == EVEN else EVEN


        # Draw goals
        goalSpaceRect = pygame.Rect(0, 0, SPACESIZE, 3*SPACESIZE)
        goalSpaceRect.topleft = (self.margin['x'] + (self.logicalBoard.goal_A[0][1] * SPACESIZE), self.margin['y'] + (self.logicalBoard.goal_A[0][0] * SPACESIZE))
        self.canvas.blit(self.goals_img[A], goalSpaceRect)
        goalSpaceRect.topleft = (self.margin['x'] + (self.logicalBoard.goal_B[0][1] * SPACESIZE), self.margin['y'] + (self.logicalBoard.goal_B[0][0] * SPACESIZE))
        self.canvas.blit(self.goals_img[B], goalSpaceRect)


        positions_A = [(p.i, p.j) for p in self.logicalBoard.team_A]
        positions_B = [(p.i, p.j) for p in self.logicalBoard.team_B]

        # Draw team A
        for p in self.logicalBoard.team_A:
            is_alone = (p.i, p.j) not in positions_B
            spaceRect.topleft = (self.margin['x'] + (p.j * SPACESIZE), self.margin['y'] + (p.i * SPACESIZE))
            if p.ball is None:
                self.canvas.blit(self.A_token[p.id][PLAYER if is_alone else SMALL_PLAYER], spaceRect)
            else:
                self.canvas.blit(self.A_token[p.id][BALL if is_alone else SMALL_BALL], spaceRect)

        # Draw team B
        for p in self.logicalBoard.team_B:
            is_alone = (p.i, p.j) not in positions_A
            spaceRect.topleft = (self.margin['x'] + (p.j * SPACESIZE), self.margin['y'] + (p.i * SPACESIZE))
            if p.ball is None:
                self.canvas.blit(self.B_token[p.id][PLAYER if is_alone else SMALL_PLAYER], spaceRect)
            else:
                self.canvas.blit(self.B_token[p.id][BALL if is_alone else SMALL_BALL], spaceRect)

        # If ball is free, draw the ball
        if self.logicalBoard.free_ball is not None:
            spaceRect.topleft = (self.margin['x'] + (self.logicalBoard.ball.j * SPACESIZE), self.margin['y'] + (self.logicalBoard.ball.i * SPACESIZE))
            self.canvas.blit(self.ball_token, spaceRect)

        pygame.display.update()

    def finished(self, winner):
        self.draw()
        
        print winner
        if winner == A:
            status_img = self.A_won_img
            rect = self.A_won_rect
        elif winner == B:
            status_img = self.B_won_img
            rect = self.B_won_rect
        else:
            status_img = self.tie_img
            rect = self.tie_rect

        self.canvas.blit(status_img, rect)
        pygame.display.update()

    def exit(self):
        pygame.quit()
        return EXIT
