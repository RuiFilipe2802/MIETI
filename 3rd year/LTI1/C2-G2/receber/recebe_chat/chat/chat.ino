#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(4,5); // Configuração CE, CSN

const byte address[6] = "1Node";  // Endereço para a comunicação entre os transceivers

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.disableCRC();                   // Desabilita o controlo de erros
  radio.setAutoAck(false);    
  radio.setChannel(60);                 // Channel 60 (0-125)    f0 = 2460 MHz
  radio.setPALevel(RF24_PA_MIN);        // Min/Low/Med/High
  radio.openReadingPipe(0, address);    // Canal para receber os pacotes
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[32] = "";                 // Mensagem recebida
    radio.read(&text,sizeof(text));     // Ouve mensagem recebida
    Serial.print(text);
  }
}
