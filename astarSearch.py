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

class ManhattanDistance:
  def __init__(self):
    self._goal = Node([1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0],[],self)

  def compute(self,node):
    score = 0
    for value in range(1,16):
      i = self._goal.getCoordByValue(value)
      j = node.getCoordByValue(value)
      score += abs(i%4 - j%4) + abs(i//4 - j//4)

    return score


class AStar:

  def __init__(self,heuristic):
    self._nodePool = NodePool()
    self._nodeBuilder = NodeBuilder()
    self._heuristic = heuristic

  def solve(self,position):
    self._bootstrap(position)
    while not self._nodePool.isEmpty():
      currentNode = self._nodePool.pop()
      if currentNode.getHScore() == 0:
        return currentNode.getMoves()
      children = self._nodeBuilder.getChildNodes(currentNode)

      for child in children:
        self._nodePool.add(child)

    return None

  def _bootstrap(self,position):
    node = Node(position,[],self._heuristic)
    self._nodePool.add(node)

heuristic = ManhattanDistance()
astar = AStar(heuristic)

start = [ 1,  6,  7,  5, 9,  3, 10,  2, 13,  8,  4, 12, 14, 11, 15,  0]

result = astar.solve(start)

if result is not None:
  print("solved!")
  print(result)
else:
  print("failed...")
