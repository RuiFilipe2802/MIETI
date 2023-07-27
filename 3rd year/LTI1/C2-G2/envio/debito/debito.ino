#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#define N 250
 
RF24 radio(4,5);      // Configuração CE, CSN

byte address[2][6] = {"1Node","2Node"};   // Endereços para a comunicação entre os transceivers
char msg[32] = "ola";                     // Mensagem a transmitir (Max 32 bytes)
char message[32] = "acabou";
int p = N;                                // Numero de pacotes
int i=1;
boolean acabou = false;

void setup(void){
    Serial.begin(9600);
    printf_begin();
    radio.begin();
    radio.setPALevel(RF24_PA_MIN);        // Min/Low/Med/High
    radio.setDataRate(RF24_2MBPS);        // Velocidade de transmissão
    radio.setChannel(60);                 // Canal 60 (0-125)
    radio.disableCRC();                   
    radio.setAutoAck(false);
    radio.openWritingPipe(address[0]);    // Canal para o envio dos pacotes
    radio.stopListening();
}

void loop(void){                          // Loop para o envio dos N pacotes
      if(i<=250){
        radio.write(&msg, sizeof(msg));
        Serial.println(i);
        i++;
        delay(100);
      }
      else{
        radio.write(&message,sizeof(message));
      }
}   
