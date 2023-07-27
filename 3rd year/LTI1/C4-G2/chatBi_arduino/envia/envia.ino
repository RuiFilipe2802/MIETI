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
byte dadosR[31];
boolean t = false;
boolean timeout = false;
boolean sucesso = false;
size_t size = sizeof(pacotecrc);
RF24 radio(4, 5);     // Configuração CE, CSN

int sn = 0;
int nP = 1;
int timer;
int z = 0;

byte final = 0b10001110;

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
boolean receber = false;

boolean e = false;
boolean r = false;
void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.setChannel(60);                   // Canal 60 (0-125) f0 = 2460 MHz
  radio.openWritingPipe(address[0]);      // Abre o canal de escrita
  radio.openReadingPipe(1, address[1]);   // Abre o canal de leitura
  radio.disableCRC();                     // Desabilita o controlo de erros
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_LOW);          // Potência de transmissão
  radio.setDataRate(RF24_1MBPS) ;         // Taxa de transmissão

}

void loop() {
  while ( e == false){
    while (Serial.available()) {
      if (Serial.available() > 0) {
        dadosR[z] = Serial.read();
        z++;
      }
    }
    if (z > 0) {
      while (sucesso == false) {
        envia();
      }
      sucesso = false;
      memset(dadosR, '\0', sizeof(dadosR));
      z = 0;
      Serial.write(final);
      Serial.write(final);
      e = true;
    }
    
  }
  e = false;
  while (r == false) {
    while (receber == false) {
      recebe();
    }
    receber = false;
  }
  r = false;
}

void envia() {
  delay(100);
  radio.stopListening();
  creatPacote(sn);
  if (radio.write(&pacote, sizeof(pacote))) {
  }
  timer = micros();
  radio.startListening();
  while (!radio.available()) {
    if ((micros() - timer) > 5000) {
      timeout = true;
      break;
    }
  }
  if (timeout == false) {
    radio.read(&ack, sizeof(ack));
    checkAck();
    ack = 0b00000000;
  }

  timeout = false;
}

void creatPacote(int snn) {
  if (dadosR[0] == 0b0110001) {
    pacote[0] = 0b10000010;
  } else if (dadosR[0] == 0b0110010) {
    pacote[0] = 0b10001110;
  }
  bitWrite(pacote[0], 0, snn);
  for (int j = 1; j < 31; j++) {
    pacote[j] = dadosR[j];
    pacotecrc[j - 1] = pacote[j];
  }
  pacote[31] = crcx::crc8(pacotecrc, size);

}

void checkAck() {

  if (bitRead(ack, 1) == 0 && bitRead(ack, 2) == 1 && bitRead(ack, 3) == 0) {
    if (sn == 0) {
      sn = 1;
      nP++;
    } else if (sn == 1) {
      sn = 0;
      nP++;
    }
    sucesso = true;
  } else if (bitRead(ack, 1) == 1 && bitRead(ack, 2) == 1 && bitRead(ack, 3) == 0) {

  }
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
        } else {
          Serial.write(payload, sizeof(payload));
          receber = true;
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
      } else {
        Serial.write(payload, sizeof(payload));
        r = true;
        receber = true;

      }
      snn = bitRead(receiver[0], 0);
      nPacotes++;
    }
  }
}
