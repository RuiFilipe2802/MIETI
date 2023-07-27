import socket
import _thread
import sys
import time
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

clientWhoRequestedStream = {}

streamsAvailable = []
confirmationSent = 0

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
    It takes a packet received from the server and prints the tag, tag number and ip of each neighbour
    
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
        #print("TAG: " + str(neighbour_tag) + str(neighbour_tag_number) + " Ip: " + str(neighbours_ip))
        tag = neighbour_tag + neighbour_tag_number
        neighbours_dict[tag] = neighbours_ip
        a += 6
    for client in clientWhoRequestedStream.keys():
        print(client)
        if(client not in neighbours_dict.values()):
            del clientWhoRequestedStream[client]
            print(clientWhoRequestedStream)
            break

def askRelayForStream(packet_received):
    packet = bytearray(1)
    packet[0] = 11
    array = SOURCE_IP.split(".")
    for a in range(len(array)):
        packet.append(int(array[a]))
    packet.append(packet_received[5])   # id_stream
    packet.append(packet_received[6])   # PS
    for x in neighbours_dict:
        if x.find("R") != -1:
            sock.sendto(packet, (neighbours_dict[x], UDP_PORT))

def sendStreamToClient(packet_received):
    """
    It sends the stream to the client that requested it
    
    :param packet_received: the packet received from the relay
    """
    packet = bytearray(1)
    packet[0] = 15
    array = SOURCE_IP.split(".")
    for a in range(len(array)):
        packet.append(int(array[a]))
    packet.append(packet_received[5])       # stream id
    packet.append(packet_received[6])       # frame number
    packet.append(packet_received[7])       # byte
    for client in clientWhoRequestedStream.keys():
        if(packet_received[5] == clientWhoRequestedStream[client]):
            print('ENTREI NO IF ' + str(client))
            sock.sendto(packet, (client, UDP_PORT))

def sendConfirmationToRelay(packet_received, addr):
    global confirmationSent
    packet = bytearray(1)
    packet[0] = 17
    array = SOURCE_IP.split(".")
    for a in range(len(array)):
        packet.append(int(array[a]))
    packet.append(packet_received[5])
    confirmationSent = 1
    sock.sendto(packet, (addr[0], UDP_PORT))

def sendRejectionToRelay(packet_received, addr):
    packet = bytearray(1)
    packet[0] = 18
    array = SOURCE_IP.split(".")
    for a in range(len(array)):
        packet.append(int(array[a]))
    packet.append(packet_received[5])
    sock.sendto(packet, (addr[0], UDP_PORT))

if __name__ == "__main__":

    print("------ E ------")
    sendConnectionRequest()
    while True:
        packet_received, addr = sock.recvfrom(1024)  # buffer size is 1024 bytes
        if(packet_received[0] == 3):                            #   RECEIVE LIST OF NEIGHBOURS
            getNeighbours(packet_received)
            #print("NEIGHBOURS: " + str(neighbours_dict))
            _thread.start_new_thread(sendBeacon, ())
        if(packet_received[0] == 4):                            #   RECEIVE BEACON
            a = 1+1
            #print("Beacon received from: " + str(addr))
        if(packet_received[0] == 8):                            #   STREAM REQUEST FROM CLIENT 
            clientWhoRequestedStream[addr[0]] = packet_received[5]
            print('Dict: ' + str(clientWhoRequestedStream))
            #print(packet_received)
            if(packet_received[5] not in streamsAvailable):
                askRelayForStream(packet_received)
        if(packet_received[0] == 14):                           #   RECEIVE STREAM FROM RELAY
            if(packet_received[5] not in streamsAvailable):
                streamsAvailable.append(packet_received[5])
                print("Streams Available: "+ str(streamsAvailable))
            print("Stream recebida " + str(packet_received[5]))
            sendStreamToClient(packet_received)
        if(packet_received[0] == 16):                           #   REQUEST TO TRANSMIT FROM RELAY
            if(confirmationSent == 0):
                print('CONFIRMATION SENT' + str(addr[0]))
                sendConfirmationToRelay(packet_received, addr)
            else:
                sendRejectionToRelay(packet_received, addr)