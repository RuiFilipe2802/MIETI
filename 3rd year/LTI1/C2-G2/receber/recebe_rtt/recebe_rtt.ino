#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#define N 2000

RF24 radio(4,5);    // Configuração CE, CSN

byte address[2][6] = {"1Node","2Node"};       // Endereços para a comunicação entre os transceivers
bool recebe = true;
unsigned int x = 1;
char msg[32]="";

void setup(void){
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(60);                     // Canal 60 (0-125) f0 = 2460 MHz         
  radio.openWritingPipe(address[1]);        // Abre o canal de escrita
  radio.openReadingPipe(1, address[0]);     // Abre o canal de leitura
  radio.disableCRC();                       // Desabilita o controlo de erros
  radio.setAutoAck(false);
  radio.setDataRate(RF24_2MBPS );           // Taxa de transmissão
  radio.setPALevel(RF24_PA_LOW);            // Potência de transmissão
  radio.startListening();
}

void loop(void){
  if (recebe){
    while ( radio.available()==false);
      radio.read( &msg, sizeof(msg));       // Recebe a mensagem
      radio.stopListening();
      while (radio.write( &msg, sizeof(msg))==false); // Envia a confirmação
      radio.startListening();
      Serial.print("\n");
      Serial.print(x);
      Serial.print(" - Enviada a confirmação.");
      Serial.print("\nConteúdo do pacote: ");
      Serial.print(msg);
      x++;
          
      if(x>N){      
          radio.stopListening();
          recebe=false;   
          Serial.print("\nFIM");         
          delay(5000);          
      }    
  }    
}
