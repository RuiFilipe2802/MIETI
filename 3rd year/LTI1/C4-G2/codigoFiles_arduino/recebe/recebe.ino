#include <CRCx.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

RF24 radio(4, 5);

byte address[2][6] = {"1Node", "2Node"};
byte tamanho;
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

int snn = 1;

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.setChannel(60);
  radio.disableCRC();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_2MBPS);
  radio.setAutoAck(false);
  radio.openWritingPipe(address[1]);
  radio.openReadingPipe(1, address[0]);
  radio.startListening();
}

void loop() {

  recebe();

}

void recebe() {
  int sn = 1;
  byte receiver[32];
  byte payload[30];
  byte ack;
  radio.startListening();
  while (!radio.available());
  radio.read(receiver, sizeof(receiver));
  if (start == false) {
    tIni = micros();
    start = true;
  }
  header = receiver[0];
  if (bitRead(receiver[0], 7) == 1 && bitRead(receiver[0], 6) == 0 && bitRead(receiver[0], 5) == 0 && bitRead(receiver[0], 4) == 0) {
    if (bitRead(receiver[0], 3) == 0 && bitRead(receiver[0], 2) == 0 && bitRead(receiver[0], 1) == 1) { // Recebe trama normal
      bitWrite(tipo, 0, bitRead(receiver[0], 1));
      bitWrite(tipo, 1, bitRead(receiver[0], 2));
      bitWrite(tipo, 2, bitRead(receiver[0], 3));
      for (int i = 0; i < 30; i++) {
        payload[i] = receiver[i + 1];
      }

      byte crc = crcx::crc8(payload, sizeof(payload));
      if (crc == receiver[31]) {    //SEND ACK
        ack = 0b10000100;
        radio.stopListening();
        radio.write(&ack, sizeof(ack));
        nPacotes++;
        sn = bitRead(receiver[0], 0);
        if (sn == snn) {
          nPacotes--;
        }else{
          Serial.write(payload, sizeof(payload));  
        }
        snn = bitRead(receiver[0], 0);

      } else {                      //SEND NACK
        ack = 0b10000110;
        radio.stopListening();
        radio.write(&ack, sizeof(ack));
        retransmissoes++;
      }
    }
    if (bitRead(receiver[0], 3) == 1 && bitRead(receiver[0], 2) == 1 && bitRead(receiver[0], 1) == 1 ) { // Recebe trama final
      ack = 0b10000100;
      radio.stopListening();
      radio.write(&ack, sizeof(ack));
      for (int i = 0; i < 30; i++) {
        payload[i] = receiver[i + 1];
      }
      sn = bitRead(receiver[0], 0);
        if (sn == snn) {
          nPacotes--;
        }else{
          Serial.write(payload, sizeof(payload));  
        }
        snn = bitRead(receiver[0], 0);
      nPacotes++;
    }
  }
}
