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
int i = 0;

byte final = 0b10001110;

boolean erroB = false;
boolean erroC = false;
boolean erroD = false;
boolean erroE = false;
boolean erroF = false;

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.setChannel(60);                   // Canal 60 (0-125) f0 = 2460 MHz
  radio.openWritingPipe(address[0]);      // Abre o canal de escrita
  radio.openReadingPipe(1, address[1]);   // Abre o canal de leitura
  radio.disableCRC();                     // Desabilita o controlo de erros
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_LOW);          // Potência de transmissão
  radio.setDataRate(RF24_2MBPS) ;         // Taxa de transmissão

}

void loop() {


  while (Serial.available()) {
    if (Serial.available() > 0) {
      dadosR[i] = Serial.read();
      i++;
    }
  }
  if (i > 0) {
    while (sucesso == false) {
      envia();
    }
    sucesso = false;
    memset(dadosR, '\0', sizeof(dadosR));
    i = 0;
    Serial.write(final);
    Serial.write(final);
  }
}

void envia() {
  delay(100);
  radio.stopListening();
  creatPacote(sn);
  if (radio.write(&pacote, sizeof(pacote))) {
    //Serial.print("Enviei o pacote:");
    //Serial.println(nP);
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
  erros();
}

void checkAck() {
  delay(100);
  if (bitRead(ack, 1) == 0 && bitRead(ack, 2) == 1 && bitRead(ack, 3) == 0) {
    //Serial.println("Enviado com sucesso");
    //Serial.println("---------------");
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

void erros() {                //Erros bits corrompidos
  if (nP == 10 && erroB == false) {
    erroB = true;
    pacote[3] = 0b11110000;
  }
  if (nP == 1 && erroC == false) {
    erroC = true;
    pacote[3] = 0b11110000;
  } if (nP == 2 && erroD == false) {
    erroD = true;
    pacote[3] = 0b11110000;
  } if (nP == 3 && erroE == false) {
    erroE = true;
    pacote[3] = 0b11110000;
  } if (nP == 4 && erroF == false) {
    erroF = true;
    pacote[3] = 0b11110000;
  }
}
