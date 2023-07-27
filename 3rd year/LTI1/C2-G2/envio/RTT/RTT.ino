#include <SPI.h>
#include <math.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

RF24 radio(4, 5);     // Configuração CE, CSN

byte address[2][6] = {"1Node","2Node"};       // Endereços para a comunicação entre os transceivers
unsigned long inicio, fim; 
char msg [32]= "abc";
float tempoRTT;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(60);                   // Canal 60 (0-125) f0 = 2460 MHz
  radio.openWritingPipe(address[0]);      // Abre o canal de escrita
  radio.openReadingPipe(1, address[1]);   // Abre o canal de leitura
  radio.disableCRC();                     // Desabilita o controlo de erros
  radio.setAutoAck(false);                
  radio.setPALevel(RF24_PA_LOW);          // Potência de transmissão
  radio.setDataRate(RF24_2MBPS) ;         // Taxa de transmissão
}

void loop(){
          tempoRTT = 0;
          radio.stopListening();      
          inicio = micros();              
          while(radio.write(&msg, sizeof(msg))==false);      // Envio da mensagem 
          radio.startListening();                            // Espera da resposta
          while(radio.available()==false);                    
          fim = micros();
          tempoRTT=fim - inicio;                             // Cálculo do tempo
          Serial.print(" - Round-trip time: ");
          Serial.print(tempoRTT);
          Serial.print(" microssegundos.");
          Serial.print("\n");
          delay(3000);
      }
