#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(4, 5);                  // Configuração CE, CSN

const byte address[6] = "1Node";   // Endereço para a comunicação entre os transceivers
char c[32];                        // Mensagem a enviar
boolean newData = false;    

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.disableCRC();               // Desabilita o controlo de erros
  radio.setAutoAck(false);          
  radio.setChannel(60);             // Canal 60 (0-125) f0 = 2460 MHz
  radio.setPALevel(RF24_PA_MIN);    // Min/Low/Med/High
  radio.openWritingPipe(address);   // Canal para o envio dos pacotes
  radio.stopListening();            
}

void loop() {
  leTeclado();                      // Função para ler input no Serial Monitor
  memset(c,0,sizeof(c));            // Limpa o array para uma próxima vez
}

void leTeclado(){
  while(Serial.available() > 0){
    String str = Serial.readString();
    Serial.print(str);
    str.toCharArray(c, 32);
    radio.write(&c, sizeof(c));
 }
}
