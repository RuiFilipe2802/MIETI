#include <CRCx.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

byte pacote[32];
byte pacotecrc[30];
byte ack = 0b00000000;
byte address[2][6] = {"1Node", "2Node"};      // Endereços para a comunicação entre os transceivers

boolean t = true;
boolean timeout = false;
size_t size = sizeof(pacotecrc);
RF24 radio(4, 5);                             // Configuração CE, CSN

int sn = 0;
int nP = 1;
int timer;
int timeIni;
int timeRTT;

byte final = 0b10001110;   

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.setChannel(60);                   // Canal 60 (0-125) f0 = 2460 MHz
  radio.openWritingPipe(address[0]);      // Abre o canal de escrita
  radio.openReadingPipe(1, address[1]);   // Abre o canal de leitura
  radio.disableCRC();                     // Desabilita o controlo de erros
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_LOW);          // Potência de transmissão
  radio.setDataRate(RF24_250KBPS) ;       // Taxa de transmissão
}

void loop() {
  delay(100);
  radio.stopListening();
  Serial.println("---------------");
  creatPacote(sn);
  if (radio.write(&pacote, sizeof(pacote))) {
    Serial.print("Enviei o pacote:");
    Serial.println(nP);
    timeIni = micros();
  }
  timer = micros();
  radio.startListening();
  while (!radio.available()){
    if((micros()-timer) > 10000){
        delay(100);
        Serial.println("---------------");
        Serial.println("TIMEOUT");
        Serial.println("---------------");
        timeout = true;
        break;
     } 
  }
  if(timeout == false){
    radio.read(&ack, sizeof(ack));
    checkAck();
    ack = 0b00000000;
   }
   if (nP == 50) { 
      Serial.println("ola");  
      radio.stopListening();
      if (radio.write(&final, sizeof(final))){
      Serial.print("Enviei o pacote Final:");
      Serial.println(nP);
    }
      while (1);
   } 
   timeout = false; 
}

void creatPacote(int snn) {                   // Função para criar pacote
  pacote[0] = 0b10000010;;
  bitWrite(pacote[0], 0, snn);
  for (int j = 1; j < 31; j++) {
    if(t==true){
    pacote[j] = 0b11111111;
    pacotecrc[j - 1] = pacote[j];
    }else{
     pacote[j] = 0b11000011;
    pacotecrc[j - 1] = pacote[j];
      }
  }
  pacote[31] = crcx::crc8(pacotecrc, size);
  /*for (int k = 0; k < 32; k++) {
    if (k == 0) {
      Serial.print("Header:");
      Serial.println(pacote[k], BIN);
    }
    if(k==1){
      Serial.print("Payload:");
       Serial.println(pacote[1],BIN);
      }
    if (k == 31) {
      Serial.print("CRC:");
      Serial.println(pacote[31], BIN);
    }
  }*/
  
}

void checkAck() {                   // Função para verificar o ACK
  /*Serial.println("---------------");
  Serial.print("ACK para verificar:");
  Serial.println(ack, BIN);
  Serial.print("Pacote numero:");
  Serial.println(nP);*/
  if (bitRead(ack, 1) == 0 && bitRead(ack, 2) == 1 && bitRead(ack, 3) == 0) {
    Serial.println("Enviado com sucesso");
    Serial.println("---------------");
    if (sn == 0) {
      sn = 1;
      nP++;
      t=true;
    } else if (sn == 1) {
      sn = 0;
      nP++;
      t=false;
    }
  }else if(bitRead(ack, 1) == 1 && bitRead(ack, 2) == 1 && bitRead(ack, 3) == 0) {
    Serial.println("Enviado sem sucesso");
    Serial.println("---------------");
  }
  timeRTT = micros();
  timeRTT = timeRTT - timeIni;
  Serial.print(timeRTT);
  Serial. println(" Micros");
}
