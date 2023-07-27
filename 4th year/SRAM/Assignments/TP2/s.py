import socket
import _thread
import sys
from time import sleep
import random

SC_IP = "10.0.0.3"
SC_UDP_PORT = 9999

SOURCE_IP = sys.argv[1]
print(SOURCE_IP)

UDP_PORT = 9999
AVAILABLE_STREAMS = [1,2]

FREQUENCY = 0.5

sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) # UDP
sock.bind((SOURCE_IP, UDP_PORT))

neighbours_dict = {}
neighbours_dict["SC"] = SC_IP


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

def sendListOfStreamsToSC():
    """
    It sends a packet to the SC with the IP address of the source and the list of available streams
    """
    packet = bytearray(1)
    packet[0] = 2
    array = SOURCE_IP.split(".")
    for a in range(len(array)):
        packet.append(int(array[a]))
    packet.append(len(AVAILABLE_STREAMS))
    for a in range(len(AVAILABLE_STREAMS)):
        packet.append(AVAILABLE_STREAMS[a])
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
   
def sendStream(packet_received, addr, frequency, idStream):
    """
    It reads a byte from the file, creates a packet with the byte, and sends the packet to the address
    and port specified
    
    :param stream_id: The stream ID of the stream you want to send
    :param addr: the IP address of the receiver
    :param frequency: the time between each packet sent
    """
    stream_id = packet_received[5]
    requestID = packet_received[6]
    stream = "streams/stream" + str(stream_id) + ".txt"
    file = open(stream, "rb")
    frame_number = 0
    while True:
        byte = file.read(1)
        packet = bytearray(1)
        packet[0] = 13
        array = SOURCE_IP.split(".")
        for a in range(len(array)):
            packet.append(int(array[a]))
        packet.append(stream_id)
        packet.append(frame_number)
        frame_number += 1
        if(frame_number >= 256):
            frame_number = 0
        packet.append(ord(byte))
        packet.append(idStream) # id da stream gerada
        #print(stream_id)
        sock.sendto(packet, (addr[0], UDP_PORT))
        sleep(frequency)

if __name__ == "__main__":

    print("------ S ------")
    sendConnectionRequest()
    sendListOfStreamsToSC()
    while True:
        packet_received, addr = sock.recvfrom(
            1024)  # buffer size is 1024 bytes
        if(packet_received[0] == 3):    # GET NEIGHBOURS
            getNeighbours(packet_received)
            #print("NEIGHBOURS: " + str(neighbours_dict))
            _thread.start_new_thread(sendBeacon, ())
        if(packet_received[0] == 4):    # BEACONS
            a = 1
            #print("Beacon received from: " + str(addr))
        if(packet_received[0] == 12):   #  TRANMISSSION REQUEST FROM RELAY
            idStream = random.randint(1, 255)
            _thread.start_new_thread(sendStream,(packet_received, addr, FREQUENCY, idStream))