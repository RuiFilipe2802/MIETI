/***************************************/
/***************************************/
/***********Grupo 2 - PITI**************/
/***************EMISSOR*****************/
/***************************************/
/***************************************/
#include <WiFi.h>
#include <CRCx.h>
#include <math.h>

#define LED 32
#define SAMPLING_TIME 220 // MICROSEGUNDOS
#define fator_espalhamento 6
#define fator_amostragem 1
#define tamanho_codigo 10
/***************************************/
/***************WIFI********************/
/***************************************/
const char* ssid = "POCO F2 Pro";
const char* password =  "*****";
WiFiServer wifiServer(80);
WiFiClient client;
/***************************************/
/***************************************/

/***************************************/
/*************CDMA VARS*****************/
/***************************************/
int codigo[10] = {1, -1, 1, -1, 1, -1, 1, -1, 1, -1};
int byte_cdma[8 * fator_espalhamento * fator_amostragem];
int codigo_tamanho[8 * fator_espalhamento];
int codigo_amostrado[8 * fator_espalhamento * fator_amostragem];
/***************************************/
/***************************************/

int bytesPayload = 64;
int file_id = 0;

/*void send_preamble(){
  digitalWrite(LED, LOW);
  delayMicroseconds(SAMPLING_TIME);
  for (int i = 0; i < ((8 * fator_espalhamento * fator_amostragem))*3; i++) {
    //Serial.print(byte_cdma[i]);
    if(i%2 == 0){
      digitalWrite(LED, HIGH);
    }else{
      digitalWrite(LED, LOW);
    }
    delayMicroseconds(SAMPLING_TIME);
    
  }
  digitalWrite(LED, HIGH);
  delayMicroseconds(SAMPLING_TIME);
}*/

void send_byte(int byte_cdma[8 * fator_espalhamento * fator_amostragem]) {
  digitalWrite(LED, LOW);
  delayMicroseconds(SAMPLING_TIME);
  for (int i = 0; i < (8 * fator_espalhamento * fator_amostragem); i++) {
    //Serial.print(byte_cdma[i]);
    digitalWrite(LED, byte_cdma[i]);
    delayMicroseconds(SAMPLING_TIME);
  }
  digitalWrite(LED, HIGH);
  delayMicroseconds(SAMPLING_TIME);
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
  int sizeOfByteArrayPackets = sizeOfByteArray + numberOfPackets * 6;
  int lastPacketPayload = sizeOfByteArray - ((numberOfPackets - 1) * bytesPayload);
  Serial.println(sizeOfByteArrayPackets);
  byte byteArrayPackets[sizeOfByteArrayPackets];
  byte preamble = 0b10101010;
  int number_packet = 1;
  int aux = 0;
  for (int i = 0; i < sizeOfByteArrayPackets ; i = i + bytesPayload + 6) {
    byteArrayPackets[i] = 0b10101011;                   // START FRAME DELIMITER
    if (i == sizeOfByteArrayPackets - (lastPacketPayload + 6)) {                            // LAST PACKET
      byte byteArrayCRC[lastPacketPayload];
      byteArrayPackets[i + 1] = file_id;
      byteArrayPackets[i + 2] = 3;
      byteArrayPackets[i + 3] = number_packet;
      byteArrayPackets[i + 4] = lastPacketPayload;
      Serial.println("----------------CRC--------------");
      for ( int j = 0; j < lastPacketPayload ; j++) {
        byteArrayPackets[i + 5 + j] = byteArray[j + aux];
        byteArrayCRC[j] = byteArray[j + aux];
        Serial.println(byteArrayCRC[j], BIN);
      }
      Serial.println("----------------CRC--------------");
      byteArrayPackets[i + (lastPacketPayload + 5)] = crcx::crc8(byteArrayCRC, lastPacketPayload);
      Serial.println("----------------Last--------------");
      Serial.println(i + (bytesPayload + 5));
      Serial.println(lastPacketPayload);
      Serial.println(byteArrayPackets[i + (lastPacketPayload + 5)], BIN);
      Serial.println("----------------Last--------------");
    } else if (i == 0) {                                                                    //  FIRST PACKET
      byte byteArrayCRC[bytesPayload];
      byteArrayPackets[i + 1] = file_id;
      byteArrayPackets[i + 2] = 1;
      byteArrayPackets[i + 3] = number_packet;
      byteArrayPackets[i + 4] = bytesPayload;
      for ( int j = 0; j < bytesPayload ; j++) {
        byteArrayPackets[i + 5 + j] = byteArray[j + aux];
        byteArrayCRC[j] = byteArray[j + aux];
      }
      byteArrayPackets[i + (bytesPayload + 5)] = crcx::crc8(byteArrayCRC, bytesPayload);
    } else {                                                                                 //  NORMAL PACKET
      byte byteArrayCRC[bytesPayload];
      byteArrayPackets[i + 1] = file_id;
      byteArrayPackets[i + 2] = 2;
      byteArrayPackets[i + 3] = number_packet;
      byteArrayPackets[i + 4] = bytesPayload;
      for ( int j = 0; j < bytesPayload ; j++) {
        byteArrayPackets[i + 5 + j] = byteArray[j + aux];
        byteArrayCRC[j] = byteArray[j + aux];
      }
      byteArrayPackets[i + (bytesPayload + 5)] = crcx::crc8(byteArrayCRC, bytesPayload);
    }
    number_packet = number_packet + 1;
    aux = aux + bytesPayload;

  }
  int y = 0;
  for (int i = 0; i < 8 * fator_espalhamento; i++) {
    if (y == tamanho_codigo) {
      y = 0;
    }
    codigo_tamanho[i] = codigo[y];
    //Serial.print(codigo_tamanho[i]);
    y = y + 1;
  }
  //Serial.println("---------------");

  int codigo_amostrado[8 * fator_espalhamento * fator_amostragem];
  int auxx = 0;
  int k = 0;
  for (int i = 0; i < 8 * fator_espalhamento; i++) {
    for (int t = 0; t < fator_amostragem; t++) {
      codigo_amostrado[auxx + t] = codigo_tamanho[i];
      //Serial.print(codigo_tamanho[i]);
    }
    //Serial.println(aux);
    auxx = auxx + fator_amostragem;
    //Serial.println(aux);
  }
  //Serial.println("---------------");
  while(true){
  for (int i = 0; i < sizeOfByteArrayPackets; i++) {
    int aux2 = 0;
    for (int r = 0; r < 8; r++) {
      int bitvar = bitRead(byteArrayPackets[i], 7 - r);
      if (bitvar == 0) {
        bitvar = -1;
      }
      //Serial.print(bit);
      for (int j = 0; j < fator_espalhamento * fator_amostragem; j++) {
        byte_cdma[j + aux2] = bitvar * codigo_amostrado[j + aux2];
        //Serial.print(codigo_amostrado[j + aux2]);
      }
      aux2 = aux2 + fator_espalhamento * fator_amostragem;
    }
    for (int i = 0; i < 8 * fator_espalhamento * fator_amostragem; i++) {
      if (byte_cdma[i] == -1) {
        byte_cdma[i] = 0;
      }
    }
    send_byte(byte_cdma);
    Serial.print("Packet[");
    Serial.print(i);
    Serial.print("]:");
    Serial.println(byteArrayPackets[i], BIN);
  }
 }
}

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

void loop() {
  tcpConnection();
  String packet_received;
  packet_received = client.readString();
  file_id = (int)packet_received[1];
  Serial.println(file_id);
  packet_received.remove(0, 2);   // remove 2 bytes from position 0 (type and ids)
  Serial.println(packet_received);
  Serial.println("Packet received:");
  Serial.println(packet_received.length());
  byte byteArray[packet_received.length()];
  packet_received.getBytes(byteArray, packet_received.length() + 1);
  /*for (int i = 0; i < packet_received.length(); i ++) {
    Serial.print("Packet[");
    Serial.print(i);
    Serial.print("]:");
    Serial.println(packet_received[i], BIN);
  }*/
  packet_creation(byteArray, packet_received.length());

}
