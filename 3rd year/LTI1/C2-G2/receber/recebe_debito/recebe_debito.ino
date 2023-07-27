#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

RF24 radio(4,5);  // Configuração CE, CSN

byte address[2][6] = {"1Node","2Node"};   // Endereços para a comunicação entre os transceivers

char mensagem[32];                        // Mensagem recebida 
char a[32] = "ola";                       // Mensagem a receber
char message[32] = "acabou";              // Mensagem de término
int pacCorr = 0;                          // Pacotes Corrompidos
float pacRec = 0;                         // Pacotes Recebidos
float pacSuc = 0;                         // Pacotes Recebidos com Sucesso
int bytesRec = 0;                         // Numero de bytes recebidos
float per;
boolean passou = false;
long tIni,tFim; 
float total,totalSemDelay;
boolean tempoInicial = false;
float debito;
boolean acabouX = false;

void setup() {
  
  Serial.begin(9600);                    // Taxa de transmissão
  radio.begin();                         
  radio.setPALevel(RF24_PA_MIN);         // Potência de transmissão
  radio.setDataRate(RF24_2MBPS);         // Velocidade de trannsmissão
  radio.disableCRC();                    
  radio.setAutoAck(false);               
  radio.setChannel(60);                  // Canal 60 (0-125)
  radio.openReadingPipe(1, address[0]);  // Canal para ouvir o outro transceiver
  radio.startListening();                
}

void loop() {
    if (radio.available() && pacRec <=250  && acabouX == false){
      int k=0;
      radio.startListening();
      radio.read(&mensagem, sizeof(mensagem)); // Le a mensagem a receber
      for(int i=0;i<sizeof(a);i++){            // Compara o array recebido com o previamente declarado
        tIni = millis();
        if (a[i] == mensagem[i]){
         }
         else{
          k=1;
        } 
      }
      if(k==1){
        pacCorr++;                            // Se as mensagens não forem iguais, incrementa o numero de pacotes corrompidos
      }
      if(strcmp(mensagem, message) == 0){
        acabou();
        acabouX = true;
        tFim = millis();
        while(1);
      }
      pacRec++;                                // Incrementa os pacotes recebidos
      Serial.print("Pacote numero: ");
      Serial.println(pacRec);
      memset(mensagem,0,sizeof(mensagem));      // Limpa o array para poder voltar a guardar 
  } 
}

void acabou(){
    Serial.print("\nPacotes recebidos: ");
    Serial.println(pacRec);
    Serial.print("com sucesso: ");
    pacSuc=pacRec-pacCorr;                
    Serial.println(pacSuc);
    Serial.print("corrompidos: ");
    Serial.println(pacCorr);
    Serial.print("Tempo Total: ");
    totalSemDelay = (tFim-tIni);
    total = -(totalSemDelay + 2500)/1000;
    Serial.println(total);
    Serial.print("N bytes recebidos: ");
    bytesRec = pacRec * 32;
    Serial.println(bytesRec);
    Serial.print("Debito: ");
    debito = (bytesRec*8)/total;
    Serial.println(debito);
    Serial.print("PER medido: ");
    per = ((pacCorr +(250.00-pacRec))/250.00)*100.00;    // PER = (nPacotes perdidos + nPacotes corrompidos) / nTotal pacotes
    Serial.print(per);
}
