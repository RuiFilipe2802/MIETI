#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>

char newFile[50][100];

typedef enum {False,True} boolean;

#define MAXLINE 200
boolean mibExists = False;
int port = 0, arguments = 0;
char communityString[10];
char mibOID[100];
char oidFromManager[10][100];
char valueFromManager[100];
char valueToManager[10][100];
   
void getFromFile(){
    char c[20];
    FILE *fptr;
    if ((fptr = fopen("MIB.txt", "r")) == NULL) {
        printf("Error! File cannot be opened.");
        exit(1);
    }
    fscanf(fptr,"%d %s %s",&port,communityString,mibOID);
    fclose(fptr);
}

int verifySyntax(char command[MAXLINE]){
    char splitWords[5][20];
    int i, j, cnt;
    j = 0;
    cnt = 0;
    for (i = 0; i <= (strlen(command)); i++) {
        if (command[i] == ' ' || command[i] == '\0') {
            splitWords[cnt][j] = '\0';
            cnt++; 
            j = 0; 
        }
        else {
            splitWords[cnt][j] = command[i];
            j++;
        }
    }
    // CHECK FOR SNMP COMMAND ERRORS
    if(strcmp(splitWords[0],"snmpget") == 0 || strcmp(splitWords[0],"snmpset") == 0 || strcmp(splitWords[0],"snmpgetnext") == 0 || strcmp(splitWords[0],"snmpgetbulk") == 0){
        ;
    }else{
        return 1;   // ERROR 1 (COMMAND UNKNOWN)
    }
    //  CHECK FOR VERSION
    if(strcmp(splitWords[1],"-v2c") == 0){
        ;
    }else{
        return 2;   // ERROR 2 (VERSION NOT SUPPORTED)
    }
    //  CHECK FOR COMMUNITY STRING
    if(strcmp(splitWords[2],communityString) == 0){
        ; 
    }else{
        return 3;   // ERROR 3 (WRONG COMMUNITY STRING)
    }
    //  CHECK FOR NUMBER OF ARGUMENTS SNMPSET (5)
    if(strcmp(splitWords[0],"snmpset") == 0){
        if(cnt == 5){
            memset(oidFromManager[0],'\0',sizeof(oidFromManager[0])); 
            memset(valueFromManager,'\0',sizeof(valueFromManager)); 
            strcpy(oidFromManager[0],splitWords[3]);
            strcpy(valueFromManager,splitWords[4]);
        }else{
            return 4;   // ERROR 4 (WRONG NUMBER OF ARGUMENTS)
        }
    }
    //  CHECK FOR NUMBER OF ARGUMENTS SNMGET,SNMPGETNEXT,SNMPGETBULK (4)
    if(strcmp(splitWords[0],"snmpget") == 0 || strcmp(splitWords[0],"snmpgetnext") == 0){
        if(cnt == 4){
            memset(oidFromManager[0],'\0',sizeof(oidFromManager[0]));
            strcpy(oidFromManager[0],splitWords[3]); 
        }else{
            return 4;   // ERROR 4 (WRONG NUMBER OF ARGUMENTS)
        }
    }
    if(strcmp(splitWords[0],"snmpgetbulk") == 0){
        if(cnt >= 4){
            for(int x = 4;x <= cnt;x++){
                strcpy(oidFromManager[x - 4],splitWords[x - 1]);
            }
        }else{
            return 4;
        }
    }
    // CHECK FOR CORRECT MIB OID
    char checkOID[11];
    memset(checkOID,'\0',sizeof(checkOID));
    strncpy(checkOID,splitWords[3],11);
    // Remove and '\n' from checkOID
    checkOID[strcspn(checkOID,"\n")] = 0;
    if(strcmp(checkOID,mibOID) == 0){
        ;
    }else{
        return 5;   //  ERROR 5 (OID NOT COMPATIBLE)
    }
    arguments = cnt;
    return 0;
}

int snmpGet(int pointer){
    FILE *mibFile = fopen("MIB.txt","r");
    mibExists = 0;
    char subOID[10];
    char currentline[100];
    if (mibFile == NULL) {   
        printf("Error! Could not open file\n"); 
        exit(-1);
    }
    memcpy(subOID,&oidFromManager[pointer][11],10);
    subOID[10] = '\0';
    // READ EVERY LINE FROM MIB
    while(fgets(currentline,sizeof(currentline),mibFile) != NULL){
        //  SEE IF STARTS WITH A '.'
        if(currentline[0] == 46){
            char oidFromFile[100];
            strcpy(oidFromFile,currentline);
            oidFromFile[strcspn(oidFromFile," ")] = 0;
            subOID[strcspn(subOID,"\n")] = 0;
            if(strcmp(subOID,oidFromFile) == 0){    //  IF WITH THE CORRECT LINE FROM MIB
                mibExists = True;
                char correctLine[5][20];            //  SLIPT LINE INTO WORDS
                int i, j, cnt;
                j = 0;
                cnt = 0;
                for (i = 0; i <= (strlen(currentline)); i++) {
                    if (currentline[i] == ' ' || currentline[i] == '\0') {
                        correctLine[cnt][j] = '\0';
                        cnt++; 
                        j = 0; 
                    }
                    else {
                        correctLine[cnt][j] = currentline[i];
                        j++;
                    }
                }
                strcpy(valueToManager[pointer],correctLine[3]);
                if(cnt > 4){
                    for(int x=4;x<cnt;x++){
                        strcat(valueToManager[pointer]," ");
                        strcat(valueToManager[pointer],correctLine[x]);
                    }
                }
            }
        }
    }
    if(mibExists == False){
        return 1;
    }
    fclose(mibFile);
    return 0;
}

int snmpSet(){
    FILE *mibFileIn = fopen("MIB.txt","r");
    mibExists = 0;
    int line = 0,contador = 0;
    char subOID[10];
    char currentline[100];
    char lineToOverwrite[100] = "\0";
    if (mibFileIn == NULL) {   
        printf("Error! Could not open file\n"); 
        exit(-1);
    }
    memcpy(subOID,&oidFromManager[0][11],10);
    subOID[10] = '\0';
    // READ EVERY LINE FROM MIB
    while(fgets(currentline,sizeof(currentline),mibFileIn) != NULL){
        contador++;
        //  SEE IF STARTS WITH A '.'
        if(currentline[0] == 46){
            char oidFromFile[100];
            strcpy(oidFromFile,currentline);
            oidFromFile[strcspn(oidFromFile," ")] = 0;
            subOID[strcspn(subOID,"\n")] = 0;
            if(strcmp(subOID,oidFromFile) == 0){    //  IF WITH THE CORRECT LINE FROM MIB
                line = contador;
                mibExists = True;
                char correctLine[5][50];            //  SLIPT LINE INTO WORDS
                int i, j, cnt;
                j = 0;
                cnt = 0;
                for (i = 0; i <= (strlen(currentline)); i++) {
                    if (currentline[i] == ' ' || currentline[i] == '\0') {
                        correctLine[cnt][j] = '\0';
                        cnt++; 
                        j = 0; 
                    }
                    else {
                        correctLine[cnt][j] = currentline[i];
                        j++;
                    }
                }
                if(strcmp(correctLine[2],"rw") != 0){    // Check for permissions to write
                    return 1;
                }
                int number = 0,alphabet = 0,total = 0;
                if(strcmp(correctLine[1],"Integer") == 0 || strcmp(correctLine[1],"Counter32") == 0 || strcmp(correctLine[1],"Gauge32") == 0 || 
                    strcmp(correctLine[1],"Timeticks") == 0){
                    for(int z=0;valueFromManager[z] != '\0';z++){
                        total++;
                        if(isdigit(valueFromManager[z]) != 0){
                            number++;
                        }
                    }
                    if(number == total && total < 6){    // If all characters are numbers and below 6 digits
                        ;
                    }else{
                        return 2;
                    }
                }
                for(int word = 0;word < 3;word++){
                    strcat(lineToOverwrite,correctLine[word]);
                    strcat(lineToOverwrite," ");
                }
                strcat(lineToOverwrite,valueFromManager);
                strcat(lineToOverwrite,"\n");
            }
        }
        if(line == contador){
            strcpy(newFile[contador],lineToOverwrite);
        }else{
            strcpy(newFile[contador],currentline);
        }
    }
    FILE *mibFileOut = fopen("MIB_tmp.txt","w");
    for(int y=0;y<20;y++){
        fprintf(mibFileOut,"%s",newFile[y]);
    }
    fclose(mibFileIn);
    fclose(mibFileOut);

    remove("MIB.txt");
    rename("MIB_tmp.txt","MIB.txt");
    return 0;
}

int snmpGetNext(){
    FILE *mibFile = fopen("MIB.txt","r");
    char subOID[10];
    char currentline[100];
    char returnedMIB[100];
    boolean variavelControlo = False;
    boolean enteredcorrectMIB = False;
    if (mibFile == NULL) {   
        printf("Error! Could not open file\n"); 
        exit(-1);
    }
    strcpy(returnedMIB,mibOID);
    memcpy(subOID,&oidFromManager[0][11],10);
    subOID[10] = '\0';
    // READ EVERY LINE FROM MIB
    while(fgets(currentline,sizeof(currentline),mibFile) != NULL){
        //  SEE IF STARTS WITH A '.'
        if(currentline[0] == 46){
            char oidFromFile[100];
            strcpy(oidFromFile,currentline);
            oidFromFile[strcspn(oidFromFile," ")] = 0;
            subOID[strcspn(subOID,"\n")] = 0;
            if(strcmp(subOID,oidFromFile) == 0){    //  IF WITH THE CORRECT LINE FROM MIB
                variavelControlo = True;
            }else if(variavelControlo == True && strcmp(subOID,oidFromFile) != 0){
                char correctLine[5][20];            //  SLIPT LINE INTO WORDS
                int i, j, cnt;
                j = 0;
                cnt = 0;
                for (i = 0; i <= (strlen(currentline)); i++) {
                    if (currentline[i] == ' ' || currentline[i] == '\0') {
                        correctLine[cnt][j] = '\0';
                        cnt++; 
                        j = 0; 
                    }
                    else {
                        correctLine[cnt][j] = currentline[i];
                        j++;
                    }
                }
                strcat(returnedMIB,oidFromFile);
                strcpy(oidFromManager[0],returnedMIB);
                strcpy(valueToManager[0],correctLine[3]);
                if(cnt > 4){
                    for(int x=4;x<cnt;x++){
                        strcat(valueToManager[0]," ");
                        strcat(valueToManager[0],correctLine[x]);
                    }
                }
                variavelControlo = False;
                enteredcorrectMIB = True;
            }
        }
    }
    if(enteredcorrectMIB == False){
        return 1;
    }
    fclose(mibFile);
    return 0;
}

void communication(){
    int sockfd;
    char line[MAXLINE];
    char *snmpCommand;
    char *hello = "ACK";
    struct sockaddr_in servaddr;
   
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
   
    memset(&servaddr, 0, sizeof(servaddr));
       
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int n, len, error;
    // START COMMUNICATION
    sendto(sockfd,(const char*)hello,strlen(hello),MSG_CONFIRM, (const struct sockaddr *)&servaddr,sizeof(servaddr));
    
    while (1)
    {
        //printf("WAITING...\n");
        n = recvfrom(sockfd,(char *)line,MAXLINE,MSG_WAITALL,(struct sockaddr*)&servaddr,&len);
        line[strcspn(line,"\n")] = 0;
        //printf("Line received: %s\n",line);
        error = verifySyntax(line);
        if(error == 1){     // SENT BACK AN ERROR (COMMAND UNKNOWN)
            char errorStr[27] = "Command unknown, try again";
            sendto(sockfd,(const char*)errorStr,strlen(errorStr),MSG_CONFIRM,(const struct sockaddr *) &servaddr, sizeof(servaddr));
        }else if(error == 2){     // SEND BACK AN ERROR (VERSION NOT SUPPORTED)
            char errorStr[33] = "Version not supported, try again";
            sendto(sockfd,(const char*)errorStr,strlen(errorStr),MSG_CONFIRM,(const struct sockaddr *) &servaddr, sizeof(servaddr));
        }else if(error == 3){     // SEND BACK AN ERROR (WRONG COMMUNITY STRING)
            char errorStr[34] = "Wrong community string, try again";
            sendto(sockfd,(const char*)errorStr,strlen(errorStr),MSG_CONFIRM,(const struct sockaddr *) &servaddr, sizeof(servaddr));
        }else if(error == 4){     // SEND BACK AN ERROR (WRONG NUMBER OF ARGUMENTS)
            char errorStr[46] = "Wrong number of arguments, check instructions";
            sendto(sockfd,(const char*)errorStr,strlen(errorStr),MSG_CONFIRM,(const struct sockaddr *) &servaddr, sizeof(servaddr));
        }else if(error == 5){     // SEND BACK AN ERROR (OID NOT COMPATIBLE)
            char errorStr[46] = "OID not compatible, try again";
            sendto(sockfd,(const char*)errorStr,strlen(errorStr),MSG_CONFIRM,(const struct sockaddr *) &servaddr, sizeof(servaddr));
        }
        snmpCommand = strtok(line," ");
        if(strcmp(snmpCommand,"snmpget") == 0){
            int mibError = snmpGet(0);
            char stringBack[] = "MIB:-";
            oidFromManager[0][strcspn(oidFromManager[0],"\n")] = 0;
            strcat(stringBack,oidFromManager[0]);
            strcat(stringBack,": "); 
            valueToManager[0][strcspn(valueToManager[0],"\n")] = 0;
            strcat(stringBack,valueToManager[0]);   
            if(mibError == 1){
                char errorStr[25] = "OID not found, try again";
                sendto(sockfd,(const char*)errorStr,strlen(errorStr),MSG_CONFIRM,(const struct sockaddr *) &servaddr, sizeof(servaddr));
            }else{
                sendto(sockfd,(const char*)stringBack,strlen(stringBack),MSG_CONFIRM,(const struct sockaddr *) &servaddr, sizeof(servaddr));
            }
        }else if(strcmp(snmpCommand,"snmpset") == 0){
            int mibError = snmpSet();
            char stringBack[] = "Value changed successfully.\nMIB:-";
            oidFromManager[0][strcspn(oidFromManager[0],"\n")] = 0;
            strcat(stringBack,oidFromManager[0]);
            strcat(stringBack,": "); 
            strcat(stringBack,valueFromManager); 
            if(mibError == 1){
                char errorStr[48] = "Do not have permission to change this OID value";
                sendto(sockfd,(const char*)errorStr,strlen(errorStr),MSG_CONFIRM,(const struct sockaddr *) &servaddr, sizeof(servaddr));
            }else if (mibError == 2){
                char errorStr[38] = "Wrong declaration or values too large";
                sendto(sockfd,(const char*)errorStr,strlen(errorStr),MSG_CONFIRM,(const struct sockaddr *) &servaddr, sizeof(servaddr));
            }else{
                sendto(sockfd,(const char*)stringBack,strlen(stringBack),MSG_CONFIRM,(const struct sockaddr *) &servaddr, sizeof(servaddr));
            }  
        }else if(strcmp(snmpCommand,"snmpgetnext") == 0){
            int mibError = snmpGetNext();
            char stringBack[] = "MIB:-";
            oidFromManager[0][strcspn(oidFromManager[0],"\n")] = 0;
            strcat(stringBack,oidFromManager[0]);
            strcat(stringBack,": "); 
            valueToManager[0][strcspn(valueToManager[0],"\n")] = 0;
            strcat(stringBack,valueToManager[0]);   
            if(mibError == 1){
                char errorStr[67] = "Next object not found, object received is the last MIB object";
                sendto(sockfd,(const char*)errorStr,strlen(errorStr),MSG_CONFIRM,(const struct sockaddr *) &servaddr, sizeof(servaddr));
            }else{
                sendto(sockfd,(const char*)stringBack,strlen(stringBack),MSG_CONFIRM,(const struct sockaddr *) &servaddr, sizeof(servaddr));
            }  
        }else if(strcmp(snmpCommand,"snmpgetbulk") == 0){
            char stringBack[] = "\0";
            int erro = 0;
            //  RUN ALL OIDS RECEIVED
            for(int z = 0;z < arguments - 3;z++){
                int mibError = snmpGet(z);
                strcat(stringBack,"MIB:-");
                oidFromManager[z][strcspn(oidFromManager[z],"\n")] = 0;
                strcat(stringBack,oidFromManager[z]);
                strcat(stringBack,": "); 
                valueToManager[z][strcspn(valueToManager[z],"\n")] = 0;
                strcat(stringBack,valueToManager[z]);
                if(mibError == 1){
                    erro = 1;
                }
                if(z == arguments - 4){
                    ;
                }else{
                    strcat(stringBack,"\n"); 
                }                 
            }
            if(erro == 1){
                char errorStr[25] = "OID not found, try again";
                sendto(sockfd,(const char*)errorStr,strlen(errorStr),MSG_CONFIRM,(const struct sockaddr *) &servaddr, sizeof(servaddr));
            }else{
                sendto(sockfd,(const char*)stringBack,strlen(stringBack),MSG_CONFIRM,(const struct sockaddr *) &servaddr, sizeof(servaddr));
            }
        }
    }
    close(sockfd);
}

int main() {

    // READ FIRST LINE FROM MIB TO GET PORT, COMMUNITY STRING AND OID MIB
    getFromFile();

    // FUNCTION TO COMMUNICATE WITH MANAGER
    communication();
    
    return 0;
}