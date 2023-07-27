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
#define SAMPLING_TIME 250 // MICROSEGUNDOS
#define PAYLOAD_SIZE 128
#define START_PACKET 1
#define NORMAL_PACKET 2
#define LAST_PACKET 3
#define READY_TO_SEND 5

#define fator_espalhamento 4
#define fator_amostragem 4
#define tamanho_codigo 10

int codigo[10] = {1, -1, 1, -1, 1, -1, 1, -1, 1, -1};
int byte_cdma[8 * fator_espalhamento * fator_amostragem];
int codigo_tamanho[8 * fator_espalhamento];
int codigo_amostrado[8 * fator_espalhamento * fator_amostragem];
int array_para_integrar[8 * fator_espalhamento * fator_amostragem];


bool previous_state = true;
bool current_state = true;
byte buff;

int byte_pacote;
byte file[2000];
int posicao_ficheiro = 0;
int packet_type;
int packet_id;
int packet_number;
int packet_payload_size;
int packet_checksum;
byte payload[PAYLOAD_SIZE];
int aux_payload = 0;
int tamanho_ultimo_payload = 0;

int printt = 0;
int file_id = 0;

int total_pacotes[255] = {0};
int ultimo_pacote = 0;

int ola = 1;
/***************************************/
/***************WIFI********************/
/***************************************/
const char* ssid = "POCO F2 Pro";
const char* password =  "*****";

WiFiServer wifiServer(80);
WiFiClient client;
/***************************************/
/***************************************/

bool get_ldr()
{
//  int voltage = analogRead(LDR_PIN);
//  Serial.println(voltage);
   if(digitalRead(AMP_PIN) == true){
      return false;
   }else{
      return true; 
   }
  //return analogRead(LDR_PIN) > 2000 ? true : false;
}

void get_byte() {
  delayMicroseconds(SAMPLING_TIME * 1.5);
  for (int i = 0; i < (8 * fator_espalhamento * fator_amostragem); i++)
  {
    if(/*get_ldr()*/ get_ldr() == true){
      byte_cdma[i] = 1;
    }else{
      byte_cdma[i] = 0;
    }
    //Serial.print(byte_cdma[i]);
    delayMicroseconds(SAMPLING_TIME);
  }
}

int verify_byte(byte byte_recebido) {
  //Serial.println(byte_recebido, BIN);
  if (byte_recebido == 0b10101011) {
    //Serial.println("SFD");
    byte_pacote = 1;
    return 1;
  }
  if ( byte_pacote == 1) {
    packet_id = byte_recebido;
    //Serial.print("Packet id:");
    //Serial.println(packet_id);
    if(packet_id == file_id){
      return 2;
    }
  }
  if ( byte_pacote == 2) {
    packet_type = byte_recebido;
    //Serial.print("Packet type:");
    //Serial.println(packet_type);
    //byte_pacote = 2;
    return 3;
  }
  if ( byte_pacote == 3) {
    packet_number = byte_recebido;
    Serial.print("Packet number:");
    Serial.println(packet_number);
    //byte_pacote = 3;
    return 4;
  }
  if ( byte_pacote == 4 ) {
    packet_payload_size = byte_recebido;
    //Serial.print("Packet payload size:");
    //Serial.println(packet_payload_size);
    //byte_pacote = 4;
    return 5;
  }
  if ( byte_pacote == 5 ) {
    payload[aux_payload] = byte_recebido;
    aux_payload++;
    //byte_pacote = 4;
    if (aux_payload == packet_payload_size) {
      //Serial.println("Recebi o payload completo");
      return 6;
    } else {
      return 5;
    }
  }
  if ( byte_pacote == 6 ) {
    byte crc_array[packet_payload_size];
    for (int i = 0; i < packet_payload_size; i++) {
      crc_array[i] = payload[i];
    }
    //    Serial.println("Vou calcular CRC");
    packet_checksum = crcx::crc8(crc_array, packet_payload_size);
    Serial.print("CRC:");
    Serial.println(packet_checksum, BIN);
    Serial.print("CRC Recebido:");
    Serial.println(byte_recebido, BIN);
    if (byte_recebido == packet_checksum) {
      posicao_ficheiro = (packet_number - 1) * PAYLOAD_SIZE; //Falta decidir tamanho payload
      Serial.println("CRC Correto");
      int v = verify_packet(packet_number);
      if (packet_type == LAST_PACKET) {
        ultimo_pacote = packet_number;
        tamanho_ultimo_payload = packet_payload_size;
      }
      int j = 0;
      for (int i = posicao_ficheiro; i < packet_payload_size + posicao_ficheiro; i++) {
        //Serial.print((char)payload[j]);
        //Serial.print(i);
        file[i] = payload[j];
        j++;
      }
      Serial.println();
      verify_packet(packet_number);
      /*if (verify_packet(packet_number) == 2) {
        printt = 1;
      }*/
    }
    //byte_pacote = 0;
    aux_payload = 0;
    packet_checksum = 0;
    memset(crc_array,0,packet_payload_size);
    memset(payload,0,packet_payload_size);
    return 0;
  }
  return 8;
}

void tcpConnection() {
  while (!(client = wifiServer.available())) {}
  if (client) {
    while (!client.connected()) {
    }
    Serial.println("Client connected");
  }
}

int verify_packet(int packet_number) {
  for (int i = 0; i < 255 ; i++) {
    if (total_pacotes[i] == packet_number and packet_number != ultimo_pacote) {
      //Serial.println("Ja existe");
      return 1;
    }
  }
  total_pacotes[packet_number] = packet_number;
  int soma = 0;
  int soma2 = 0;
  if (ultimo_pacote > 0) {
    for (int i = 0; i < ultimo_pacote ; i++) {
      soma = soma + total_pacotes[i];
      soma2 = soma2 + i;
    }
    Serial.println("------------");
    Serial.println(soma);
    Serial.println(soma2);
    if (soma == soma2) {
      Serial.println("Ja tenho todos");
      printt = 1;
      return 2;
    }
  }

  return 0;
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
  pinMode(AMP_PIN, INPUT);
}

void loop() {
  if (file_id == 0) {
    tcpConnection();
    String file_id_pc;
    file_id_pc = client.readString();
    file_id = (int) file_id_pc[1];
    Serial.print("File ID:");
    Serial.println(file_id);
  }
  if (ola == 1) {
    int y = 0;
    for (int i = 0; i < 8 * fator_espalhamento; i++) {
      if (y == tamanho_codigo) {
        y = 0;
      }
      codigo_tamanho[i] = codigo[y];
      y = y + 1;
    }
    int aux = 0;
    int k = 0;
    for (int i = 0; i < 8 * fator_espalhamento; i++) {
      for (int t = 0; t < fator_amostragem; t++) {
        codigo_amostrado[aux + t] = codigo_tamanho[i];
      }
      aux = aux + fator_amostragem;
    }
    ola = 0;
  }
  //current_state =!(digitalRead(AMP_PIN)); 
  current_state = get_ldr();
  if (!current_state && previous_state)
  {
    get_byte();
    //Serial.println();
    int array_para_integrar[8 * fator_espalhamento * fator_amostragem];
    for (int q = 0; q < 8 * fator_espalhamento * fator_amostragem; q++) {
      array_para_integrar[q] = byte_cdma[q] * codigo_amostrado[q];
    }
    for (int i = 0; i < 8 * fator_espalhamento * fator_amostragem; i++) {
      if (byte_cdma[i] == -1) {
        byte_cdma[i] = 0;
      }
    }
    char data_byte = 0;
    int soma = 0;
    int w = 0;
    for (int i = 0; i < 8; i++) {
      for (int k = 0; k < fator_espalhamento * fator_amostragem; k++) {
        soma = soma + array_para_integrar[k + w];
      }
      w = w + fator_espalhamento * fator_amostragem;
      if (soma > 0) {
        bitWrite(data_byte, 7-i, 1);
      } else {
        bitWrite(data_byte, 7-i,0);
      }
      soma = 0;      
    }
    //Serial.println();
    //Serial.println(data_byte, BIN);
    byte_pacote = verify_byte(data_byte);
  }
  previous_state = current_state;

  if (printt == 1) {
    Serial.println("*****************************");
    Serial.println("*****************************");
    Serial.println("*************FILE************");
    client.write('a');
    int tamanho = 0;
    for (int i = 0; i < ((ultimo_pacote - 1) * PAYLOAD_SIZE) + tamanho_ultimo_payload; i++) {
      Serial.print((char)file[i]);
      tamanho++;
    }
    client.write(file,tamanho);

    memset(payload,0,PAYLOAD_SIZE);
    memset(file,0,2000);
    printt = 0;
    file_id = 0;
    memset(total_pacotes,0,255);
    ultimo_pacote = 0;
    tamanho_ultimo_payload = 0;
  }
}
