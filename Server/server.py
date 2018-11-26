import select, socket, sys, queue, gameroom, player, enum

class DataType(enum.Enum):
    Empty = -1
    SetName = 0
    SetImage = 1
    OpponentSetName = 2
    OpponentSetImage = 3
    FindGame = 4
    YourRound = 6
    MakeMove = 7
    UpdateMap = 8
    End = 9

def bytesToInt(bytes):
    result = 0
    for b in bytes:
        result = result * 256 + int(b)
    return result

def intToBytes(value, length):
    result = []
    for i in range(0, length):
        result.append(value >> (i * 8) & 0xff)
    result.reverse()
    return result

class Server:
    def __init__(self):
       self.__socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
       self.__inputs = [self.__socket]
       self.__outputs = []
       self.__message_queues = {}
       self.__players = {}
       self.__findGamePleyers = []
       self.__gameRooms = []
       self.__size = 1024
       self.__dataType = DataType.Empty.value
       self.__remainingBytes = 0
       self.__receiveData = bytes()

       self.__socket.setblocking(0)
       self.__socket.bind(('', 8888))
       self.__socket.listen(6)


    def __sendData(self, socket, data):
        self.__message_queues[socket].put(data)
        if socket not in self.__outputs:
            self.__outputs.append(socket)

    def __roomeForSocket(self, socket):
        for room in self.__gameRooms:
            if room.playerA == socket or room.playerB == socket:
                return room
        return None

    def __updateMap(self, room):

         mapData = bytearray()
         for x in range(0, room.width):
            mapData += bytes(room.playerArray[x])

         data = bytearray()
         data.append(DataType.UpdateMap.value)
         data += bytes(intToBytes(len(mapData), 4))
         data += mapData

         self.__sendData(room.playerA, data)
         self.__sendData(room.playerB, data)

    def __endGame(self, room, bool):
         data = bytearray()
         data.append(DataType.End.value)
         data += bytes(intToBytes(1, 4))
         data += bytes(intToBytes(int(bool), 1))

         self.__sendData(room.playerA, data)
         self.__sendData(room.playerB, data)

    def __makeMove(self, socket, data):
        x = bytesToInt(data[:1])
        y = bytesToInt(data[1:2])
        room = self.__roomeForSocket(socket)

        if room.currentPlayer == socket:

            lose = room.makeMove(x, y)
            self.__updateMap(room)
            
            if lose == gameroom.MoveRetVal.Lose:
                self.__endGame(room, False)
            elif lose == gameroom.MoveRetVal.Win:
                self.__endGame(room, True)
            else:
                room.swapCurrent()
                self.__setTurn(room.currentPlayer)

    def __setTurn(self, socket):
        room = self.__roomeForSocket(socket)
        if room:
            if room.playerA == socket:  
                os = room.playerB
            else:                       
                os = room.playerA

            bool = (room.currentPlayer == socket)
            data = bytearray()
            data.append(DataType.YourRound.value)
            data += bytes(intToBytes(1, 4))

            self.__sendData(socket, data + bytes(intToBytes(bool, 1)))
            self.__sendData(os, data + bytes(intToBytes(not bool, 1)))

    def __setName(self, socket, name = None):
        if name:
            self.__players[socket].name = name

        room = self.__roomeForSocket(socket)
        if room:
            if room.playerA == socket:  
                os = room.playerB
            else:                       
                os = room.playerA
                
            data = bytearray()
            data.append(DataType.OpponentSetName.value)
            data += bytes(intToBytes(len(self.__players[socket].name), 4))
            data += self.__players[socket].name
            self.__sendData(os, data)

    def __setImage(self, socket, image = None):
        if image:
            self.__players[socket].image = image

        room = self.__roomeForSocket(socket)
        if room:
            if room.playerA == socket:  os = room.playerB
            else:                       os = room.playerA

            data = bytearray()
            data.append(DataType.OpponentSetImage.value)
            data += bytes(intToBytes(len(self.__players[socket].image), 4))
            data += self.__players[socket].image
            self.__sendData(os, data)
    
    def __findGame(self, socket):
        if not self.__findGamePleyers:
            self.__findGamePleyers.append(socket)
        else:
            opponentSocket = self.__findGamePleyers.pop()
            self.__gameRooms.append(gameroom.GameRoom(socket, opponentSocket))

            data = bytearray()
            data.append(DataType.FindGame.value)
            data += bytes(intToBytes(0, 4))

            self.__sendData(socket, data)
            self.__sendData(opponentSocket, data)

            self.__setName(socket)
            self.__setImage(socket)
            self.__setName(opponentSocket)
            self.__setImage(opponentSocket)

            self.__setTurn(socket)
            self.__setTurn(opponentSocket)

    def __readData(self, socket, data):

        while data:
            if self.__dataType == DataType.Empty.value:
                self.__dataType = int(data[0])
                self.__remainingBytes = bytesToInt(data[1:5])
                data = data[5:]
            else:
                minLength = min(self.__remainingBytes, len(data))
                self.__remainingBytes -= minLength

                self.__receiveData += data[:minLength]
                data = data[minLength:]

            if self.__remainingBytes == 0:
                    if self.__dataType == DataType.SetName.value:
                        self.__setName(socket, self.__receiveData)
                    elif self.__dataType == DataType.SetImage.value:
                        self.__setImage(socket, self.__receiveData)
                    elif self.__dataType == DataType.FindGame.value:
                        self.__findGame(socket)
                    elif self.__dataType == DataType.MakeMove.value:
                        self.__makeMove(socket, self.__receiveData)

                    self.__dataType = DataType.Empty.value
                    self.__receiveData = bytes()

    def __add(self, socket):
        connection, client_address = socket.accept()
        connection.setblocking(0)
        self.__inputs.append(connection)
        self.__message_queues[connection] = queue.Queue()
        self.__players[connection] = player.Player()

    def __remove(self, socket):
        if socket in self.__outputs:
            self.__outputs.remove(socket)
        if socket in self.__findGamePleyers:
            self.__findGamePleyers.remove(socket)

        self.__inputs.remove(socket)
        socket.close()
        del self.__message_queues[socket]
        del self.__players[socket]
  
    def __readable(self, socket):
        if socket is self.__socket:
            self.__add(socket)
        else:
            data = socket.recv(self.__size)
            if data:
                self.__readData(socket, data) 
            else:
                self.__remove(socket)

    def __writable(self, socket):
        try:
            next_msg = self.__message_queues[socket].get_nowait()
        except queue.Empty:
            self.__outputs.remove(socket)
        else:
            socket.send(next_msg)

    def __exceptional(self, socket):
        self.__remove(socket)

    def run(self):
        while self.__inputs:
            r, w, e = select.select(
                self.__inputs, self.__outputs, self.__inputs)

            for s in r:
                self.__readable(s)
            for s in w:
                self.__writable(s)
            for s in e:
                self.__exceptional(s)
    
