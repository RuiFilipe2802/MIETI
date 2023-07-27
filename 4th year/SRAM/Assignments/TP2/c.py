import socket
import _thread
import sys
import random
from time import sleep

SC_IP = "10.0.0.3"
SC_UDP_PORT = 9999

SOURCE_IP = sys.argv[1]
print(SOURCE_IP)

UDP_PORT = 9999

PS = 0

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP
sock.bind((SOURCE_IP, UDP_PORT))

neighbours_dict = {}
neighbours_dict["SC"] = SC_IP

streams_available =[]

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
    It takes the packet received and extracts the neighbours' information from it
    
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

def askForStreamsAvailable():
    """
    It sends a packet to the SC asking for the streams available
    """
    packet = bytearray(1)
    packet[0] = 6
    array = SOURCE_IP.split(".")
    for a in range(len(array)):
        packet.append(int(array[a]))
    sock.sendto(packet, (SC_IP, SC_UDP_PORT))

def askForStream(stream_id):
    """
    It sends a packet to all the neighbours asking for a stream
    
    :param stream_id: The stream ID of the stream you want to ask for
    """
    packet = bytearray(1)
    packet[0] = 8
    array = SOURCE_IP.split(".")
    for a in range(len(array)):
        packet.append(int(array[a]))
    packet.append(stream_id)
    PS = random.randint(1, 255)
    packet.append(PS)
    for b in neighbours_dict:
        sock.sendto(packet, (neighbours_dict[b], UDP_PORT))

def inputStreamFromUser():
    """
    It asks the user to choose a stream
    """
    stream_selected = input("Choose the stream you want to watch:")
    askForStream(int(stream_selected))
    print("Selected stream: " + str(stream_selected))

if __name__ == "__main__":
    print("------ C ------")
    sendConnectionRequest()
    askForStreamsAvailable()
    frames = []
    while True:
        packet_received, addr = sock.recvfrom(1024)  # buffer size is 1024 bytes
        if(packet_received[0] == 3):                            #   RECEIVE LIST OF NEIGHBOURS
            getNeighbours(packet_received)
            _thread.start_new_thread(sendBeacon, ())
        #if(packet_received[0] == 4):
            #print("Beacon received from: " + str(addr))
        if(packet_received[0] == 7):                            #   RECEIVE LIST OF STREAMS FROM SC
            for i in range(5,len(packet_received)):
                streams_available.append(int(packet_received[i]))
            print("Streams available: " + str(streams_available))
            _thread.start_new_thread(inputStreamFromUser, ())
        if(packet_received[0] == 15):                           #   RECEIVE STREAM FROM EDGE
            if(packet_received[6] >= 255):
                frames.clear()
            if(packet_received[6] not in frames):
                frames.append(packet_received[6])
                print(chr(packet_received[7]), end='', flush=True)

