import copy
from pprint import pprint

class Node:
  def __init__(self,position,moves,heuristic):
    self._position = position
    self._moves = moves
    self._heuristic = heuristic
    self._hScore = None

  def getPosition(self):
    return copy.deepcopy(self._position)

  def getGScore(self):
    return len(self._moves)

  def getHScore(self):
    if self._hScore is None:
      self._hScore = self._heuristic.compute(self)
    return self._hScore

  def getFScore(self):
    return self.getGScore() + self.getHScore()

  def getMoves(self):
    return copy.copy(self._moves)

  def getHeuristic(self):
    return self._heuristic

  def getCoordByValue(self,value):
    i = 0
    for x in self._position:
      if x == value:
        return i
      else:
        i += 1

class NodeBuilder:
  def getChildNodes(self,node):
    children = []
    iSpace = node.getCoordByValue(0)
    dx = [-1,1,0,0]
    dy = [0,0,-1,1]

    for i in range(4):
      x = iSpace % 4
      y = iSpace // 4

      if (0<=x+dx[i]<4 and 0<=y+dy[i]<4):
        position = node.getPosition()
        position[y*4+x] = position[(y+dy[i])*4 + x+dx[i]]
        position[(y+dy[i])*4 + x+dx[i]] = 0

        moves = node.getMoves()
        moves.append(self._getMoveNameFromDelta(i))
        child = Node(position,moves,node.getHeuristic())
        children.append(child)

    return children

  def _getMoveNameFromDelta(self,iDelta):
    if iDelta == 0:
      return 'left'
    elif iDelta == 1:
      return 'right'
    elif iDelta == 2:
      return 'up'
    elif iDelta == 3:
      return 'down'

class NodePool:
  def __init__(self):
    self._pool = []
    self._history = {}

  def add(self,node):
    if str(node.getPosition()) in self._history:
      return
    self._history[str(node.getPosition())] = True
    self._insort(node)

  def pop(self):
    return self._pool.pop(0)

  def isEmpty(self):
    return len(self._pool) == 0

  def _insort(self,node):
    lo = 0
    hi = len(self._pool)

    while lo < hi:
      mid = (lo+hi)//2
      if node.getFScore() < self._pool[mid].getFScore():
        hi = mid
      else:
        lo = mid + 1

    self._pool.insert(lo,node)

class ManhattanDistance1:
  def __init__(self):
    self._goal = Node([1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0],[],self)

  def compute(self,node):
    score = 0
    for value in range(1,16):
      i = self._goal.getCoordByValue(value)
      j = node.getCoordByValue(value)
      score += abs(i%4 - j%4) + abs(i//4 - j//4)

    return score

class ManhattanDistance2:
  def __init__(self,position):
    self._goal = Node(position,[],self)

  def compute(self,node):
    score = 0
    for value in range(1,16):
      i = self._goal.getCoordByValue(value)
      j = node.getCoordByValue(value)
      score += abs(i%4 - j%4) + abs(i//4 - j//4)

    return score

class AStar:

  def __init__(self,heuristic1,heuristic2):
    self._nodePool1 = NodePool()
    self._nodePool2 = NodePool()
    self._nodeBuilder = NodeBuilder()
    self._heuristic1 = heuristic1
    self._heuristic2 = heuristic2

  def solve(self,position):
    start = Node(position,[],self._heuristic1)
    self._nodePool1.add(start)

    goal = Node([1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0],[],self._heuristic2)
    self._nodePool2.add(goal)

    while not self._nodePool1.isEmpty() or not self._nodePool2.isEmpty():
      currentStartNode = self._nodePool1.pop()

      if str(currentStartNode.getPosition()) in self._nodePool2._history:
        print("mid position is")
        print(currentStartNode.getPosition())

        return 'success'

      children = self._nodeBuilder.getChildNodes(currentStartNode)
      for child in children:
        self._nodePool1.add(child)

      currentGoalNode = self._nodePool2.pop()

      if str(currentGoalNode.getPosition()) in self._nodePool1._history:
        print("mid position is")
        print(currentGoalNode.getPosition())
        return 'success'

      children = self._nodeBuilder.getChildNodes(currentGoalNode)
      for child in children:
        self._nodePool2.add(child)

    return None

start = [ 1,  6,  7,  5, 9,  3, 10,  2, 13,  8,  4, 12, 14, 11, 0,  15]

heuristic1= ManhattanDistance1()
heuristic2= ManhattanDistance2(start)
astar = AStar(heuristic1,heuristic2)

result = astar.solve(start)

if result is not None:
  print("solved!")
else:
  print("failed...")
