#include <CRCx.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
  
RF24 radio(4, 5);                         // Configuração CE, CSN

byte address[2][6] = {"1Node", "2Node"};  // Endereços para a comunicação entre os transceivers
byte receiver[32];
byte tamanho;
byte payload[30];
byte tipo;
int nPacotes = 1;
float helpPacotes =  1.0;
byte header;
int retransmissoes = 0;
float helpRetransmissoes =  0.0;
long tIni, tFim;
int bytesRec = 0;                         // Numero de bytes recebidos
float debito, per;
boolean start = false;
int sn = 1;
int snn = 1;

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.setChannel(60);                   // Canal 60 (0-125) f0 = 2460 MHz
  radio.disableCRC();                     // Desabilita o controlo de erros
  radio.setPALevel(RF24_PA_LOW);          // Potência de transmissão
  radio.setDataRate(RF24_2MBPS);          // Taxa de transmissão
  radio.setAutoAck(false);
  radio.openWritingPipe(address[1]);      // Abre o canal de escrita
  radio.openReadingPipe(1, address[0]);   // Abre o canal de leitura
  radio.startListening();
}

void loop() {
  recebe();
}

void recebe() {                            // Função para receber
  byte ack;
  radio.startListening();
  while (!radio.available());
  radio.read(receiver, sizeof(receiver));
  if (start == false) {
    tIni = micros();
    start = true;
  }
  header = receiver[0];
  Serial.println("--------------");
  if (bitRead(receiver[0], 7) == 1 && bitRead(receiver[0], 6) == 0 && bitRead(receiver[0], 5) == 0 && bitRead(receiver[0], 4) == 0) {
    if (bitRead(receiver[0], 3) == 0 && bitRead(receiver[0], 2) == 0 && bitRead(receiver[0], 1) == 1) { // Recebe trama normal
      bitWrite(tipo, 0, bitRead(receiver[0], 1));
      bitWrite(tipo, 1, bitRead(receiver[0], 2));
      bitWrite(tipo, 2, bitRead(receiver[0], 3));
      for (int i = 0; i < 31; i++) {
        payload[i] = receiver[i + 1];
      }
      //Serial.println(payload[0], BIN);
      byte crc = crcx::crc8(payload, sizeof(payload));
      if (crc == receiver[31]) {    //SEND ACK
        ack = 0b10000100;
        radio.stopListening();
        radio.write(&ack, sizeof(ack));
        Serial.print("Header: ");
        Serial.println(header, BIN);
        Serial.print("ACK enviado: ");
        Serial.println(ack, BIN);
        Serial.print("Pacote: ");
        Serial.println(nPacotes);
        nPacotes++;
        sn = bitRead(receiver[0], 0);
        //Serial.print("SN RECEBIDO:");
        //Serial.println(sn);
        //Serial.print("SNN ANTEIOR:");
        //Serial.println(snn);
        if (sn == snn) {
          //Serial.println("Ja tenho esse pacote");
          //Serial.println("----------------------------------------------");
          nPacotes--;
        }
        snn = bitRead(receiver[0], 0);
        //Serial.print("SNN DEPOIS:");
        //Serial.println(snn);

      }else{                      //SEND NACK
        ack = 0b10000110;
        radio.stopListening();
        radio.write(&ack, sizeof(ack));
        Serial.print("Header NACK: ");
        Serial.println(header, BIN);
        Serial.print("NACK enviado: ");
        Serial.println(ack, BIN);
        retransmissoes++;
      }
    }
    if (bitRead(receiver[0], 3) == 1 && bitRead(receiver[0], 2) == 1 && bitRead(receiver[0], 1) == 1 ) { // Recebe trama final
      ack = 0b10000100;
      radio.stopListening();
      radio.write(&ack, sizeof(ack));
      Serial.print("Header do ultimo: ");
      Serial.println(header, BIN);
      Serial.print("ACK do ultimo enviado: ");
      Serial.println(ack, BIN);
      Serial.print("Pacote: ");
      Serial.println(nPacotes);
      Serial.print("Recebido o ultimo pacote");
      acabou();
      nPacotes++;
    }
  }
}

void acabou() {
  tFim = micros() - tIni;
  tFim = tFim * 0.000001;
  Serial.println("\n\n\n*******************************");
  Serial.print("Pacotes recebidos: ");
  Serial.println(nPacotes);
  Serial.print("Pacotes retransmitidos :");
  Serial.println(retransmissoes);
  Serial.print("Tempo Total: ");
  Serial.print(tFim);
  Serial.println(" segundos");
  Serial.print("Numero de bytes recebidos: ");
  bytesRec = nPacotes * 32;
  Serial.println(bytesRec);
  Serial.print("Debito: ");
  debito = (bytesRec * 8) / tFim;
  Serial.print(debito);
  Serial.println(" bps");
  Serial.print("PER medido: ");
  helpPacotes = nPacotes;
  helpRetransmissoes = retransmissoes;
  per = (helpRetransmissoes / helpPacotes);
  Serial.print(per);
  Serial.println("%");
  Serial.println("*******************************");
}
