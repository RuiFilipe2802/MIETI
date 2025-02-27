import socket
import pickle

class Network:
    def __init__(self):
        self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.host = "127.0.0.1"
        self.port = 5009
        self.addr = (self.host, self.port)

    def connect(self, name):
        self.client.connect(self.addr)
        self.client.send(str.encode(name))
        val = self.client.recv(8)
        print('VAL :' + str(val))
        return int(val.decode())

    def disconnect(self):
        self.client.close()

    def send(self, data, pick=False):
        try:
            if pick:
                self.client.sendall(pickle.dumps(data))
            else:
                self.client.sendall(str.encode(data))
                reply = self.client.recv(4096)
            try:
                reply = pickle.loads(reply)
            except Exception as e:
                print(e)
            return reply
        except socket.error as e:
            print(e)