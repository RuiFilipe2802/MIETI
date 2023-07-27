#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
   
#define MAXLINE 200
int port = 0;

void getPortFromFile(){
    char c[20];
    FILE *fptr;
    if ((fptr = fopen("MIB.txt", "r")) == NULL) {
        printf("Error! File cannot be opened.");
        exit(1);
    }
    fscanf(fptr, "%[^ ]", c);
    port = atoi(c);
    fclose(fptr);
}
   
void communication() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
       
    if((sockfd = socket(AF_INET, SOCK_DGRAM,0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
       
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
       
    servaddr.sin_family    = AF_INET;   // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);
       
    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr*)&servaddr, 
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    int len, n, x;
    len = sizeof(cliaddr);

    // START COMMUNICATION
    n = recvfrom(sockfd,(char*)buffer,MAXLINE, MSG_WAITALL,(struct sockaddr*)&cliaddr,&len);
    buffer[n] = '\0';

    char command[MAXLINE];
    while (1)
    {
        fflush(stdin);
        // INPUT FROM COMMAND LINE
        printf("Input: ");
        fgets(command, sizeof(command), stdin);
        // SEND COMMAND TO AGENT
        x = sendto(sockfd,command,strlen(command),MSG_CONFIRM,(const struct sockaddr*)&cliaddr,len);
        memset(&command, 0, sizeof(command));

        // WAIT FOR RESPONSE
        n = recvfrom(sockfd,(char*)buffer,MAXLINE, MSG_WAITALL,(struct sockaddr*)&cliaddr,&len);
        buffer[n] = '\0';
        printf("%s\n",buffer);
    } 
}

int main(int argc, char const *argv[])
{
    printf("\nInstructions:");
    printf("\nsnmpget [VERSION] [COMMUNITY STRING] [OID]");
    printf("\nsnmpset [VERSION] [COMMUNITY STRING] [OID] [VALUE]");
    printf("\nsnmpgetnext [VERSION] [COMMUNITY STRING] [OID]");
    printf("\nsnmpgetbulk [VERSION] [COMMUNITY STRING] [OID]\n");

    // READ FIRST LINE FROM MIB TO GET PORT
    getPortFromFile();
    // FUNCTION TO COMMUNICATE WITH AGENT
    communication();

    return 0;
}
