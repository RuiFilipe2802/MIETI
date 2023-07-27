import socket
import _thread
import time
from time import sleep
import sys

host = '10.0.0.3'  # Standard loopback interface address (localhost)       // 10.0.0.3
port = 9999 

configFile = "configFile.conf"

available_streams = []

sock = socket.socket(socket.AF_INET, # Internet
                            socket.SOCK_DGRAM) # UDP
sock.bind((host, port))

topology = {}
times_beacon = {}

def loadTopology():
    with open(configFile) as f:
        lines = f.readlines()
        for line in lines:
            arguments = line.split(" ")
            if(arguments[0] == "T"):
                topology[arguments[3]] = "OFF"

def sendNeighbours(ip):
    name = ""
    neighbours_dict = {}
    with open(configFile) as f:
        lines = f.readlines()
        for line in lines:
            if line.find(ip) != -1:
                arguments = line.split(" ")
                name = arguments[2]
            if line.find(name) != -1 and len(name) > 0:
                args = line.split(" ")
                if(args[0] == 'V' and (args[1] == name or args[2] == name)):
                    if(args[1] == name):
                        neighbours_dict[args[2].strip('\n')] = None
                    if(args[2] == name):
                        neighbours_dict[args[1].strip('\n')] = None

        for neighbour in neighbours_dict:
            for line in lines:
                if(line.find(neighbour) != -1 and line.find("T") != -1):
                    arguments = line.split(" ")
                    if(topology[arguments[3]] == "ON"):
                        neighbours_dict[neighbour] = arguments[3]
        
    filtered = {k: v for k, v in neighbours_dict.items() if v is not None}
    neighbours_dict.clear()
    neighbours_dict.update(filtered)
    #print(neighbours_dict)
    send = bytearray(1)
    send[0] = 3
    send.append(len(neighbours_dict))
    for neighbour in neighbours_dict:
        send.append(ord(neighbour[0]))
        send.append(ord(neighbour[1]))
        ip_neighbour = neighbours_dict[neighbour]
        array = ip_neighbour.split(".")
        for a in range(len(array)):
            send.append(int(array[a]))

    sock.sendto(send, (ip, port))

def sendListosStreams(addr):
    """
    It sends a packet to the client with the IP address of the server and the available streams
    
    :param addr: The address of the client that requested the list of streams
    """
    packet = bytearray(1)
    packet[0] = 7
    array = host.split(".")
    for a in range(len(array)):
        packet.append(int(array[a]))
    for stream in available_streams:
        packet.append(stream)
    #print(available_streams)
    sock.sendto(packet, (addr[0], port))

def isAlive(addr):
    while True:
        sleep(1)
        #print('IP: ' + str(addr[0]) + 'Time_beacon: ' + str(times_beacon[addr[0]]) + ' time: ' + str(time.time()))
        if(time.time() - times_beacon[addr[0]]> 2):
            print("OFFLINE: " + str(addr[0])) 
            topology[addr[0]] = "OFF"
            del times_beacon[addr[0]]
            for ip_topology in topology.keys():
                if(topology[ip_topology] == "ON"):
                    sendNeighbours(ip_topology)
            sys.exit(1)

if __name__ == "__main__":
    print("------ SC ------")
    loadTopology()
    while True:
        #sock.sendto(msg,(host, port))
        packet_received, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
        if(packet_received[0] == 1):                # CONNECTION REQUEST
            topology[addr[0]] = "ON"
            #print("received message: %s" % packet_received)
            ip = socket.inet_ntoa(packet_received[1:])
            for ip_topology in topology.keys():
                if(topology[ip_topology] == "ON"):
                    sendNeighbours(ip_topology)
        if(packet_received[0] == 2):                # RECEIVE LIST OF STREAMS
            #print("received message: %s" % packet_received)
            ip = socket.inet_ntoa(packet_received[1:5])
            n_available_strems = packet_received[5]
            for a in range(0,n_available_strems):
                available_streams.append(packet_received[6+a])
            #print(available_streams)
        if(packet_received[0] == 4):               # RECEIVE BEACON
            #print("Beacon received from: " + str(addr))
            if(addr[0] not in times_beacon):
                times_beacon[addr[0]] = time.time()
                _thread.start_new_thread(isAlive, (addr,))
            else:
                times_beacon[addr[0]] = time.time()
        if(packet_received[0] == 6):               # REQUEST FOR STREAMS AVAILABLE
            print("List of streams asked from: " + str(addr))
            sendListosStreams(addr)
            