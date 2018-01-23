import argparse
import sys
import os

parser = argparse.ArgumentParser(description='Vérifie la solution d\'un labyrinthe.')

parser.add_argument('file', help='Fichier dans lequel le labyrinthe est décrit.')
parser.add_argument('-q', '--quiet', help='N\'affiche rien.', action='store_true')

args = parser.parse_args()

lab = []

with open(args.file, 'r') as f:
  n = int(f.readline())
  for _ in range(n):
    lab.append(list(f.readline()[:-1]))
  timer = int(f.readline())

curline = 0
curcol = 0
key = False

keysym = 'a'
doorsym = 'A'
outsym = 'S'
wallsym = 'X'
tpsyms = ['*', '%', '$', '#', '&', '+', '-', '@', '^', '!']

tps = [(i,j) for i, line in enumerate(lab) for j, c in enumerate(line) if c in tpsyms]


herosym = 'H'

actionup = 'HAUT'
actiondown = 'BAS'
actionleft = 'GAUCHE'
actionright = 'DROITE'
actiontp = 'TP'

import copy
import time

sleeptime = max(1/(timer/5), 0.005)

HOV = 40

def print_lab(action):
  print(chr(27) + "[2J") # Clear screen
  print("Timer : ", timer)
  print("Action : ", action)
  print()
  
  if n <= HOV:
    lab2 = copy.deepcopy(lab)
    lab2[curline][curcol] = herosym
  else:
    minline = max(curline - HOV // 2, 0)
    maxline = min(minline + HOV, n)
    if maxline - minline < HOV:
      minline = maxline - HOV
    mincol = max(curcol - HOV // 2, 0)
    maxcol = min(mincol + HOV, n)
    if maxcol - mincol < HOV:
      mincol = maxcol - HOV
    lab2 = [line[mincol:maxcol] for line in lab[minline:maxline]]
    lab2[curline - minline][curcol - mincol] = herosym

  
  print('        Y = {:3d}'.format(curline))
  print('X = {:3d} {}'.format(curcol, ''.join(lab2[0])))
  for line in lab2[1:]:
    print('        ' + ''.join(line))
  time.sleep(sleeptime)

iteration = 0

def die(message):
    if not args.quiet:
      print('Iteration', iteration, ':', message)
    sys.exit(1)
   


def move(di, dj):
  global curline, curcol, timer
  i, j = curline + di, curcol + dj
  if not 0 <= i < n or not 0 <= j < n:
    die('Vous êtes tombé du labyrinthe, vous êtes mort.')
  if lab[i][j] == wallsym:
    die('Vous avez tenté de traverser un mur, il était plus dur que votre tête. Vous êtes mort.')
  if lab[i][j] == doorsym and not key:
    die('Vous ne pouvez pas passer une porte sans la clef. Vous êtes mort (de faim probablement).')

  curline, curcol = i, j
  timer -= 1

def teleport():
  global curline, curcol
  sym = lab[curline][curcol]
  if not sym in tpsyms:
    die('Vous avez tenté de vous téléporter alors que vous n\'êtes pas sur un téléporteur. Vous êtes donc devenu fou et vous êtes mort.')
  for i,j in tps:
    if (i != curline or j != curcol) and lab[i][j] == sym:
      curline, curcol = i, j
      break


for action in sys.stdin.readlines():
  iteration += 1
  key = key or lab[curline][curcol] == keysym
  
  if action[-1] == '\n':
    action = action[:-1]
  if len(action) == 0:
    continue

  if not args.quiet:
    print_lab(action)

  if action == actionup:
    move(-1, 0)
  elif action == actiondown:
    move(1, 0)
  elif action == actionleft:
    move(0, -1)
  elif action == actionright:
    move(0, 1)
  elif action == actiontp:
    teleport()
  else:
    die('Votre discours est incohérent, on ne comprend que HAUT, BAS, GAUCHE, DROITE et TP. Du coup personne ne veux vous aider. Vous êtes mort.')

  if lab[curline][curcol] == 'S':
    if not args.quiet:
      print('Bravo, vous êtes sorti du terrible labyrinthe à temps. Quels autres défis palpitants vous attendent dehors?')
    sys.exit(0)

  if timer == 0:
    die('La labyrinthe s\'écroule sur vous. Vous êtes mort.')
    
die('Votre chemin ne mène pas à la sortie. Vous errez sans fin et sans but. Vous êtes mort.')

