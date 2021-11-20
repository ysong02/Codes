#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/time.h>

#define RECVSIZE 1024


int main(int argc, char *argv[]){
  if(argc<3){
    perror("Enter with form NAME_OF_FILE IP PORT\n");
    return 0;
  }

  //创建socket对象
  int socketUDP = socket(AF_INET, SOCK_DGRAM, 0);
  if(socketUDP < 0){
    perror("Cannot creat socket\n");
    return -1;
  }

  int valid = 1;
  setsockopt(socketUDP, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));
  struct timeval tv = {5,0};//设置接受超时时长为5秒
  setsockopt(socketUDP, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

  //创建网络通信对象
  struct sockaddr_in adresse;
  int port = atoi(argv[2]);
  adresse.sin_family = AF_INET;
  adresse.sin_port = htons(port);
  inet_aton(argv[1], &adresse.sin_addr);
  socklen_t len = sizeof(adresse);

  char msg[RECVSIZE];
  char blanmsg[RECVSIZE];
  char handshake[RECVSIZE];

  int flag = 1;
  strcpy(handshake,"DEMAND: HS1");
  sendto(socketUDP,handshake,RECVSIZE,0,(struct sockaddr *)&adresse, len);
  int hs2 = recvfrom(socketUDP,blanmsg,RECVSIZE,0,(struct sockaddr *)&adresse, &len);
  if(hs2 < 0){
    // if(errno == EWOULDBLOCK){
    //   perror("ERROR:Failed to connect\n");
    //   return -1;
    // }
    perror("ERROR: Connection Error HANDSHAKE2\n");
    return -1;
  }

  strcpy(handshake, "DEMAND:HS2;ACK:");
  strncpy(msg, blanmsg,15);
  int portNew;
  if(strcmp(handshake, msg)==0){
    printf("HANDSHAKE2-RECV: %s",blanmsg);
    memset(msg,0,RECVSIZE);
    strncpy(msg, blanmsg+15,4);
    printf("%s\n",msg);
    portNew = atoi(msg);
  }else{
    perror("ERROR: Connection Error HANDSHAKE2\n");
    return -1;
  }
  strcpy(handshake,"DEMAND: HS3");
  sendto(socketUDP,handshake,RECVSIZE,0,(struct sockaddr *)&adresse, len);
  adresse.sin_port=htons(portNew);
  //printf("\nEnter your message:\n");
  printf("\nEnter your filename:\n");
  while(flag){


    memset(blanmsg, 0, RECVSIZE);
     fgets(blanmsg,RECVSIZE,stdin);
     sendto(socketUDP,blanmsg,RECVSIZE,0,(struct sockaddr *)&adresse, len);
    // memset(blanmsg,0,RECVSIZE);
    // int recv = recvfrom(socketUDP,blanmsg,RECVSIZE,0,(struct sockaddr *)&adresse, &len);
    // if(recv < 0){
    //   perror("Error on receving message\n");
    //   return -1;
    // }
    // printf("RECV: %s\n",blanmsg);
    strncpy(msg,blanmsg,strlen(msg)-1);
    //strcat(msg,"2");

    FILE *fp = fopen("copie.pdf", "w");
    if (fp==NULL){
      printf("%s File can not open to write \n",msg);
      exit(1);
    }
    memset(msg,0,RECVSIZE);
    int length = 0;
    while ((length = recvfrom(socketUDP, msg, RECVSIZE,0,(struct sockaddr *)&adresse, &len))){
      printf("length = %d\n",length );

      if (length<0){
        printf("Receive Data Failed!\n");
        break;
      }

      int write_length = fwrite(msg,1,length,fp);
      if(write_length<length){
        printf("Write Failed!\n" );
        break;
      }
      bzero(msg, RECVSIZE);
    }
    printf("Transfer finished.\n");
    fclose(fp);

    // if(strcmp(msg,"stop\n")==0){
    //   while(strcmp(handshake,"Server:END-HANDSHAKE")!=0){
    //     strcpy(handshake,"Client:END-HANDSHAKE");
    //     sendto(socketUDP,handshake,RECVSIZE,0,(struct sockaddr *)&adresse, len);
    //     recvfrom(socketUDP,handshake,RECVSIZE,0,(struct sockaddr *)&adresse, &len);
    //   }
    //   flag = 0;
    // }
  }
  close(socketUDP);
  return 0;

}
