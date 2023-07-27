#include <WiFi.h>
#include <CRCx.h>
#include <math.h>
#include <cdma.ino>

#define LED 25
#define PERIOD 10

const char* ssid = "POCO F2 Pro";
const char* password =  "*****";

int bytesPayload = 32;

WiFiServer wifiServer(80);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
  wifiServer.begin();
  pinMode(LED, OUTPUT);
}

void tcpConnection() {
  while (!(client = wifiServer.available())) {}
  if (client) {
    while (!client.connected()) {
    }
    Serial.println("Client connected");
  }
}

void packet_creation(byte byteArray[], float sizeOfByteArray) {
  float numberOfPackets = ceil(sizeOfByteArray / bytesPayload);
  Serial.println(numberOfPackets);
  int sizeOfByteArrayPackets = sizeOfByteArray + numberOfPackets * 5;
  int lastPacketPayload = sizeOfByteArray - ((numberOfPackets - 1) * bytesPayload);
  Serial.println(sizeOfByteArrayPackets);
  byte byteArrayPackets[sizeOfByteArrayPackets];
  byte preamble = 0b10101010;
  int number_packet = 1;
  int aux = 0;
  for (int i = 0;i<sizeOfByteArrayPackets ;i=i+bytesPayload + 5) {
    byteArrayPackets[i] = 0b10101011;                   // START FRAME DELIMITER
    if (i == sizeOfByteArrayPackets - (lastPacketPayload + 5)) {                            // LAST PACKET
      byte byteArrayCRC[lastPacketPayload]; 
      byteArrayPackets[i + 1] = 3;
      byteArrayPackets[i + 2] = number_packet;
      byteArrayPackets[i + 3] = lastPacketPayload;
      for ( int j = 0; j < lastPacketPayload ; j++) {
        byteArrayPackets[i + 4 + j] = byteArray[j + aux];
        byteArrayCRC[j] = byteArray[j + aux];
      }
      byteArrayPackets[i + (bytesPayload + 4)] = crcx::crc8(byteArrayCRC, lastPacketPayload);
    } else if(i == 0){                                                                      //  FIRST PACKET
      byte byteArrayCRC[bytesPayload];
      byteArrayPackets[i + 1] = 1;
      byteArrayPackets[i + 2] = number_packet;
      byteArrayPackets[i + 3] = bytesPayload;
      for ( int j = 0; j < bytesPayload ; j++) {
        byteArrayPackets[i + 4 + j] = byteArray[j + aux];
        byteArrayCRC[j] = byteArray[j + aux];
      }
      byteArrayPackets[i + (bytesPayload + 4)] = crcx::crc8(byteArrayCRC, bytesPayload);
    }else{                                                                                  //  NORMAL PACKET
      byte byteArrayCRC[bytesPayload];
       byteArrayPackets[i + 1] = 2;
       byteArrayPackets[i + 2] = number_packet;
       byteArrayPackets[i + 3] = bytesPayload;
       for ( int j = 0; j < bytesPayload ; j++) {
        byteArrayPackets[i + 4 + j] = byteArray[j + aux];
        byteArrayCRC[j] = byteArray[j + aux];
      }
      byteArrayPackets[i + (bytesPayload + 4)] = crcx::crc8(byteArrayCRC, bytesPayload);
    }
    number_packet = number_packet + 1;
    aux = aux + bytesPayload;
  }
  for(int z = 0;z < 2;z++){
    send_byte(preamble);
    Serial.print("Preamble[");
    Serial.print(z);
    Serial.print("]:");
    Serial.println(preamble, BIN);
  }
  for(int i = 0; i < sizeOfByteArrayPackets; i ++){
    send_byte(byteArrayPackets[i]);
    Serial.print("Packet[");
    Serial.print(i);
    Serial.print("]:");
    Serial.println(byteArrayPackets[i], BIN);
  }
}

void loop() {
  tcpConnection();
  String packet_received;
  packet_received = client.readString();
  packet_received.remove(0,2);    // remove 2 bytes from position 0 (type and ids)
  Serial.println(packet_received);
  Serial.println("Packet received:");
  Serial.println(packet_received.length());
  byte byteArray[packet_received.length()];
  packet_received.getBytes(byteArray, packet_received.length() + 1);
  for(int i = 0; i < packet_received.length(); i ++){
    Serial.print("Packet[");
    Serial.print(i);
    Serial.print("]:");
    Serial.println(packet_received[i], BIN);
  }
  
  packet_creation(byteArray, packet_received.length());
}

void send_byte(char my_byte){
  digitalWrite(LED, LOW);
  delay(PERIOD);
  for(int i = 0; i < 8; i++){
    digitalWrite(LED,(my_byte&(0x01<<i))!=0);
    delay(PERIOD);
  }
  digitalWrite(LED, HIGH);
  delay(PERIOD);
}
