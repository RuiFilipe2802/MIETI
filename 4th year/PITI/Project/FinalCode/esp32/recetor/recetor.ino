/***************************************/
/***************************************/
/***********Grupo 2 - PITI**************/
/***************RECETOR*****************/
/***************************************/
/***************************************/
#include <WiFi.h>
#include <math.h>
#include <CRCx.h>

#define AMP_PIN 32
#define SAMPLING_TIME 2
#define PAYLOAD_SIZE 32
#define START_PACKET 1
#define NORMAL_PACKET 2
#define LAST_PACKET 3
#define READY_TO_SEND 5


bool previous_state = true;
bool current_state = true;
byte buff;

int byte_pacote;
byte file[624];
int posicao_ficheiro = 0;
int packet_type;
int packet_number;
int packet_payload_size;
int packet_checksum;
byte payload[32];
byte * lastPayload;
int aux_payload = 0;

int printt = 0;


/***************************************/
/***************WIFI********************/
/***************************************/
const char* ssid = "OnePlus 8T";
const char* password =  "*****";
WiFiServer wifiServer(80);
WiFiClient client;
/***************************************/
/***************************************/


char get_byte() {
  char data_byte = 0;
  delay(SAMPLING_TIME * 1.5);
  for (int i = 0; i < 8; i++)
  {
    data_byte = data_byte | (char)digitalRead(AMP_PIN) << i;
    delay(SAMPLING_TIME);
  }
  return data_byte;
}

int verify_byte(byte byte_recebido) {
  Serial.println(byte_recebido, BIN);
  if (byte_recebido == 0b10101011) {
    Serial.println("SFD");
    byte_pacote = 1;
    return 1;
  }
  if ( byte_pacote == 1 ) {
    packet_type = byte_recebido;
    Serial.print("Packet type:");
    Serial.println(packet_type);
    //byte_pacote = 2;
    return 2;
  }
  if ( byte_pacote == 2) {
    packet_number = byte_recebido;
    Serial.print("Packet number:");
    Serial.println(packet_number);
    //byte_pacote = 3;
    return 3;
  }
  if ( byte_pacote == 3 ) {
    packet_payload_size = byte_recebido;
    Serial.print("Packet payload size:");
    Serial.println(packet_payload_size);
    //byte_pacote = 4;
    return 4;
  }
  if ( byte_pacote == 4 ) {
    payload[aux_payload] = byte_recebido;
    aux_payload++;
    //byte_pacote = 4;
    if (aux_payload == packet_payload_size) {
      Serial.println("Recebi o payload completo");
      return 5;
    } else {
      return 4;
    }
  }
  if ( byte_pacote == 5 ) {
    byte crc_array[packet_payload_size];
    for (int i = 0; i < packet_payload_size; i++) {
      crc_array[i] = payload[i];
    }
    Serial.println("Vou calcular CRC");
    packet_checksum = crcx::crc8(crc_array, packet_payload_size);
    Serial.print("CRC:");
    Serial.println(packet_checksum, BIN);
    if (byte_recebido == packet_checksum){
      posicao_ficheiro = (packet_number - 1) * 32; //Falta decidir tamanho payload
      Serial.println("CRC Correto");
      int j = 0;
      for (int i = posicao_ficheiro; i < packet_payload_size + posicao_ficheiro; i++) {
        //Serial.print((char)payload[j]);
        Serial.print(i);
        file[i] = payload[j];
        j++;
      }
      Serial.println();
      aux_payload = 0;
      if (packet_type == LAST_PACKET) {
        printt = 1;
      }
    }
    //byte_pacote = 0;
    return 0;
  }
  return 8;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(AMP_PIN, INPUT);
}

void loop() {
  current_state = digitalRead(AMP_PIN);
  if (!current_state && previous_state)
  {
    byte_pacote = verify_byte(get_byte());
  }
  previous_state = current_state;

  if (printt == 1) {
    for (int i = 0; i < 624; i++) {
      Serial.print((char) file[i]);
    }
    printt = 0;
  }
}
