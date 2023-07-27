int codigo[4]{1,-1,1,-1};
int tamanho_cod = 4;
int fe = 4;
int tamanho_msg = 16;
int tamanho_sinal_cdma = fe * tamanho_msg;
int codigo_tam[64];

void cod_tamanho_msg(){
  int y = 0;
  for(int i = 0; i < tamanho_sinal_cdma; i++){
    if(y == tamanho_cod){
      y = 0;
    }
    codigo_tam[i] = codigo[y];
    y = y + 1; 
  }
  // for(int i = 0; i < 8; i++){
  //   Serial.print(codigo_tam[i]);
  // }
}

void cdma(byte array[]){
  int arrayMsg[tamanho_msg];
  int bytes = 0;
  for(int i = 0; i < tamanho_msg; i = i + 8){
    for(int j = 0; j < 8; j++){
      int bit = bitRead(array[bytes],j);
      if(bit == 0){
        bit = -1;
      }
      arrayMsg[i + j]= bit;
    }
    bytes = bytes + 1;
  }

  cod_tamanho_msg();
  int array_final[tamanho_sinal_cdma];
  int k = 0;
  int l = 0;
  int y = 0;
  for(int x = 0; x < tamanho_msg; x++){
    for(y = 0; y < fe; y++){
      array_final[l] =  arrayMsg[x] * codigo_tam[k + y];
      l = l + 1;
    }
    k = k + y;
    y = 0;
  }
  Serial.print("Msg:");
  for(int i = 0; i < tamanho_msg; i++){
    Serial.print(arrayMsg[i]);
  } 
  // for(int i = 0; i < 32; i++){
  //   Serial.print(codigo_tam[i]);
  // } 
  Serial.println(); 
  Serial.print("Sinal CDMA:");
  for(int i = 0; i < tamanho_sinal_cdma; i++){
    Serial.print(array_final[i]);
  }
}
