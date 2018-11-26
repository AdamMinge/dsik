import enum, random

class Values(enum.Enum):
    EmptyCell = 0
    One = 1
    Two = 2
    Three = 3
    Four = 4
    Five = 5
    Six = 6
    Seven = 7
    Eight = 8
    Mine = 9
    ExplodedMine = 10
    Cell = 11

class MoveRetVal(enum.Enum):
    Lose = -1
    Continues = 0
    Win = 1
 
class GameRoom:
    def __init__(self, playerA, playerB):
        self.playerA = playerA
        self.playerB = playerB
        self.width = 50
        self.height = 50
        self.boombs = int((self.width * self.height) * 0.2)
        self.array = [[Values.EmptyCell.value] * self.height for i in range(self.width)]
        self.playerArray = [[Values.Cell.value] * self.height for i in range(self.width)]
        self.currentPlayer = playerA
        self.otherCell = ((self.width * self.height) - self.boombs)

        for i in range(0, self.boombs):
            while True:
                randX = random.randint(0, self.width - 1)
                randY = random.randint(0, self.height - 1)
                if self.array[randX][randY] != Values.Mine.value:
                    self.array[randX][randY] = Values.Mine.value
                    break
        
        for x in range(0, self.width):
            for y in range(0, self.height):
                if self.array[x][y] != Values.Mine.value:
                    self.array[x][y] = self.__countBoombs(x, y)

    def swapCurrent(self):
        if self.currentPlayer == self.playerA:
            self.currentPlayer = self.playerB
        else:
            self.currentPlayer = self.playerA

    def __inRange(self, x, y):
        if x >= 0 and x < self.width and y >= 0 and y < self.height:
            return True
        else:
            return False

    def __isBoomb(self, x, y):
        if self.__inRange(x,y):
            if self.array[x][y] == Values.Mine.value:
                return True
        return False

    def __isEmpty(self, x, y):
        if self.__inRange(x,y):
            if self.array[x][y] == Values.EmptyCell.value:
                return True
        return False

    def __countBoombs(self, x, y):
        c = 0

        if self.__isBoomb(x + 1, y + 1): c += 1
        if self.__isBoomb(x + 1, y): c += 1
        if self.__isBoomb(x + 1, y - 1): c += 1

        if self.__isBoomb(x, y + 1): c += 1
        if self.__isBoomb(x, y - 1): c += 1

        if self.__isBoomb(x - 1, y + 1): c += 1
        if self.__isBoomb(x - 1, y): c += 1
        if self.__isBoomb(x - 1, y - 1): c += 1

        return c

    def __updatePlayerArray(self, x, y):

        points = [[x,y]]

        while len(points) > 0:
            p = points.pop()

            if self.array[p[0]][p[1]] == 0 and self.playerArray[p[0]][p[1]] != Values.EmptyCell.value:
                self.playerArray[p[0]][p[1]] = Values.EmptyCell.value

                if self.__inRange(p[0] + 1, p[1] + 1): points.append([p[0] + 1, p[1] + 1])
                if self.__inRange(p[0] + 1, p[1]): points.append([p[0] + 1, p[1]])
                if self.__inRange(p[0] + 1, p[1] - 1): points.append([p[0] + 1, p[1] - 1])

                if self.__inRange(p[0], p[1] + 1): points.append([p[0], p[1] + 1])
                if self.__inRange(p[0], p[1] - 1): points.append([p[0], p[1] - 1])

                if self.__inRange(p[0] - 1, p[1] + 1): points.append([p[0] - 1, p[1] + 1])
                if self.__inRange(p[0] - 1, p[1]): points.append([p[0] - 1, p[1]])
                if self.__inRange(p[0] - 1, p[1] - 1): points.append([p[0] - 1, p[1] - 1])
            else:
                self.playerArray[p[0]][p[1]] = self.array[p[0]][p[1]]

    def makeMove(self, x, y):
        if self.__isBoomb(x, y):
            self.playerArray = self.array[:]
            self.playerArray[x][y] = Values.ExplodedMine.value
            return MoveRetVal.Lose
        else:
            self.__updatePlayerArray(x, y)
            self.otherCell -= 1

            if self.otherCell == 0:
                return MoveRetVal.Win
            else:
                return MoveRetVal.Continues


