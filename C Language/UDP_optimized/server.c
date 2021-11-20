#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>

#define RECVSIZE 1024
#define FILE_NAME_MAX_SIZE 512

int main(int argc, char *argv[]){
  if(argc<2){
    perror("Enter with form NAME_OF_FILE PORT\n");
    return 0;
  }

  int port = atoi(argv[1]);
  char msg[RECVSIZE];
  char blanmsg[RECVSIZE];

  //创建socket对象
  int socketC = socket(AF_INET, SOCK_DGRAM, 0);
  if(socketC < 0){
    perror("Cannot creat socket\n");
    return -1;
  }

  int valid = 1;
  setsockopt(socketC, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

  //创建网络通信对象
  struct sockaddr_in adresseC;
  adresseC.sin_family = AF_INET;
  adresseC.sin_port = htons(port);
  adresseC.sin_addr.s_addr = htonl(INADDR_ANY);
  socklen_t lenC = sizeof(adresseC);

  if(bind(socketC, (struct sockaddr *)&adresseC, lenC)==-1){
    perror("Bind Failed\n");
    close(socketC);
    return -1;
  }

  while(1){

    //创建socket对象
    int socketUDP = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketUDP < 0){
      perror("Cannot creat socket\n");
      return -1;
    }

    int valid = 1;
    setsockopt(socketUDP, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

    //创建网络通信对象
    struct sockaddr_in adresse;
    adresse.sin_family = AF_INET;
    port++;
    adresse.sin_port = htons(port);
    adresse.sin_addr.s_addr = htonl(INADDR_ANY);
    socklen_t len = sizeof(adresse);

    if(bind(socketUDP, (struct sockaddr *)&adresse, len)==-1){
      perror("Child Bind Failed\n");
      close(socketUDP);
      return -1;
    }
     //创建客户地址
     struct sockaddr_in ad_client;
     socklen_t len_client = sizeof(ad_client);

     char handshake[RECVSIZE];

     recvfrom(socketC, blanmsg, RECVSIZE, 0, (struct sockaddr *)&ad_client, &len_client);
     if(strcmp(blanmsg,"DEMAND: HS1")==0){
          strcpy(handshake, "DEMAND:HS2;ACK:");
          char portString[10];
          sprintf(portString,"%d\n",port);
          strcat(handshake, portString);
          printf("RECV: Client\n");
          sendto(socketC, handshake, RECVSIZE, 0, (struct sockaddr *)&ad_client, len_client);
      }
      memset(blanmsg,0,RECVSIZE);
      recvfrom(socketC, blanmsg, RECVSIZE, 0, (struct sockaddr *)&ad_client, &len_client);
     if(strcmp(blanmsg,"DEMAND: HS3")==0){
          pid_t fpid;
          fpid = fork();
          if(fpid == 0){
            printf("Child Process: Id = %d\n",getpid());

            while(strcmp(blanmsg, "Client:END-HANDSHAKE")!=0){
              memset(blanmsg,0,RECVSIZE);
              recvfrom(socketUDP, blanmsg, RECVSIZE, 0, (struct sockaddr *)&ad_client, &len_client);
              printf("RECV:%s\n",blanmsg);
              memset(msg,0,RECVSIZE);
              strncpy(msg, blanmsg, strlen(blanmsg)-1);

              FILE *fp = fopen(msg, "r");
              if (fp==NULL){
                printf("files not found %s\n",msg);
              }else{
                memset(blanmsg, 0, RECVSIZE);
                int file_block_length = 0;
                while((file_block_length = fread(msg, 1, RECVSIZE, fp))>0){
                  printf("file_block_length = %d\n", file_block_length);

                  sendto(socketUDP, msg, file_block_length,0,(struct sockaddr *)&ad_client, len_client);
                  // <0 ){
                  //   printf("fail to send files");
                  //   break;
                  // }
                  memset(msg,0,RECVSIZE);
                }
                fclose(fp);
                printf("finish sending files");
              }
              //strcpy(msg,"Recieved\n");
              // if(strcmp(blanmsg, "Client:END-HANDSHAKE")==0){
              //   strcpy(handshake,"Server:END-HANDSHAKE");
              //   sendto(socketUDP, handshake, RECVSIZE, 0, (struct sockaddr *)&ad_client, len_client);
              //   return 0;
              // }
              //sendto(socketUDP, msg, RECVSIZE, 0, (struct sockaddr *)&ad_client, len_client);
            }
          }else{
            close(socketUDP);
          }
      }else{
          perror("ERROR:UNKNOWN ERROR\n");
          return -1;
      }
   }
   close(socketC);
   return 0;
}
