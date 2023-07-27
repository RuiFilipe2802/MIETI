import socket
import _thread
from struct import pack
import sys
import time
import random
from time import sleep

SC_IP = "10.0.0.3"
SC_UDP_PORT = 9999

SOURCE_IP = sys.argv[1]
print(SOURCE_IP)

UDP_PORT = 9999

sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) # UDP
sock.bind((SOURCE_IP, UDP_PORT))

times_beacon = {}
neighbours_dict = {}
neighbours_dict["SC"] = SC_IP

edgeWhoRequestedStream = {}
requestID = 0

streamsAvailable = []

def sendBeacon():
    """
    It sends a beacon packet to all the neighbours
    """
    packet = bytearray(1)
    packet[0] = 4
    array = SOURCE_IP.split(".")
    for a in range(len(array)):
        packet.append(int(array[a]))
    while True:
        for b in neighbours_dict:
            sock.sendto(packet, (neighbours_dict[b], SC_UDP_PORT))
        sleep(1)

def sendConnectionRequest():
    """
    It sends a connection request to the server
    """
    packet = bytearray(1)
    packet[0] = 1
    array = SOURCE_IP.split(".")
    for a in range(len(array)):
        packet.append(int(array[a]))
    sock.sendto(packet, (SC_IP, SC_UDP_PORT))


def getNeighbours(packet_received):
    """
    It takes the packet received from the server and creates a dictionary of the neighbours
    
    :param packet_received: The packet received from the server
    """
    neighbours_dict.clear()
    neighbours_dict["SC"] = SC_IP
    neighbours = packet_received[2:]
    a = 0
    while(a < len(neighbours)):
        neighbour_tag = chr(neighbours[a])
        neighbour_tag_number = chr(neighbours[a + 1])
        neighbours_ip = socket.inet_ntoa(neighbours[a + 2: a + 6])
        tag = neighbour_tag + neighbour_tag_number
        #print("TAG: " + str(neighbour_tag) + str(neighbour_tag_number) + " Ip: " + str(neighbours_ip))
        neighbours_dict[tag] = neighbours_ip
        a += 6

def askServerForStream(id_stream):
    """
    It sends a request to all the servers in the network to ask for a stream
    
    :param id_stream: the stream ID of the stream you want to ask for
    """
    global requestID
    packet = bytearray(1)
    packet[0] = 12
    array = SOURCE_IP.split(".")
    for a in range(len(array)):
        packet.append(int(array[a]))
    requestID = random.randint(1, 255)
    packet.append(id_stream)
    packet.append(requestID)
    for x in neighbours_dict:
        if x.find("S") != -1:
            sock.sendto(packet, (neighbours_dict[x], UDP_PORT))

def askRelayNeighbourForStream(id_stream, ps):
    """
    It sends a packet to all the relay neighbours asking for the stream with the given id_stream and ps
    
    :param id_stream: the id of the stream
    :param ps: ps
    """
    packet = bytearray(1)
    packet[0] = 11
    array = SOURCE_IP.split(".")
    for a in range(len(array)):
        packet.append(int(array[a]))
    packet.append(id_stream)
    packet.append(ps)
    for x in neighbours_dict:
        if x.find("R") != -1:
            sock.sendto(packet, (neighbours_dict[x], UDP_PORT))

def sendStreamToEdge(packet_received):
    """
    It sends the stream to the edge that requested it
    
    :param packet_received: the packet received from the server
    """
    packet = bytearray(1)
    packet[0] = 14
    array = SOURCE_IP.split(".")
    for a in range(len(array)):
        packet.append(int(array[a]))
    packet.append(packet_received[5])       # stream id
    packet.append(packet_received[6])       # frame number
    packet.append(packet_received[7])       # byte
    for edge in edgeWhoRequestedStream.keys():
        array = edgeWhoRequestedStream[edge]
        if(packet_received[5] == array[0] and array[2] == packet_received[8]):
            ip = socket.inet_ntoa(array[3:7])
            packet.append(array[1])
            sock.sendto(packet, (ip, UDP_PORT))
            packet.pop()

if __name__ == "__main__":
    print("------ R ------")
    sendConnectionRequest()
    while True:
        packet_received, addr = sock.recvfrom(1024)  # buffer size is 1024 bytes
        if(packet_received[0] == 3):                            #   RECEIVE LIST OF NEIGHBOURS
            getNeighbours(packet_received)
            #print("NEIGHBOURS: " + str(neighbours_dict))
            _thread.start_new_thread(sendBeacon, ())
        if(packet_received[0] == 4):                            #   RECEIVE BEACON
            a = 1
            #print("Beacon received from: " + str(addr))
        if(packet_received[0] == 11):                           #   TRANSMISSION REQUEST FROM EDGE
            if(packet_received[6] not in edgeWhoRequestedStream.keys()):
                #print(packet_received)
                array = bytearray(1)
                array[0] = packet_received[5]
                array.append(packet_received[6])
                array.append(0)
                for a in range(1,5):
                    array.append(packet_received[a])
                edgeWhoRequestedStream[packet_received[6]] = array
                if(packet_received[5] not in streamsAvailable):
                    serverExists = 0
                    for x in neighbours_dict:
                        if x.find("S1") != -1:
                            askServerForStream(packet_received[5])
                            serverExists  = 1
                    if(serverExists == 0):
                        askRelayNeighbourForStream(packet_received[5], packet_received[6])
        if(packet_received[0] == 13 or packet_received[0] == 14):   #   RECEIVE STREAM FROM SERVER OR NEIGHBOUR RELAY
            if(packet_received[5] not in streamsAvailable):
                streamsAvailable.append(packet_received[5])
            entrar_no_if = 0
            for x in edgeWhoRequestedStream.values():
                array = x
                if(x[0] == packet_received[5] and x[2] == 0 and entrar_no_if == 0):
                    x[2] = packet_received[8]
                    entrar_no_if = 1
            sendStreamToEdge(packet_received)
