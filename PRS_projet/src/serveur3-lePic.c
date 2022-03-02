#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/time.h>
#include<unistd.h>
#include<netinet/in.h>

#define RECVSIZE 1024

int main(int argc, char *argv[]){
    //Verification of pamameters
    //Execute in terminal ./server1-LePic PortNumber
    if(argc<2){
        perror("./server1-LePic NumeroPort\n");
        return 0;
    }

    //Initialisation
    int port = atoi(argv[1]);
    char sendmsg[RECVSIZE];
    char recvmsg[RECVSIZE];
    //char transmsg[RECVSIZE];

    //Create Socket for Server's Controler
    int socketC = socket(AF_INET,SOCK_DGRAM,0);
    if(socket<0){
        perror("ERROR: Cannot creat socket for Server's Controler.\n");
        return -1;
    }

    int validC = 1;
    setsockopt(socketC, SOL_SOCKET,SO_REUSEADDR,&validC,sizeof(int));

    //Create Server' Controler Address
    struct sockaddr_in addressC;
    addressC.sin_family = AF_INET;
    addressC.sin_port = htons(port);
    addressC.sin_addr.s_addr = htonl(INADDR_ANY);
    socklen_t lenC = sizeof(addressC);

    //Bind the Server's Controler
    if(bind(socketC, (struct sockaddr *)&addressC, lenC)==-1){
        perror("Bind for Server's Controler Failed.\n");
        return -1;
    }



    //Create Client's Address
    struct sockaddr_in addressClient;
    socklen_t lenClient = sizeof(addressClient);

    while(1){
        //3-way Handshake
        memset(recvmsg, 0, RECVSIZE);
        recvfrom(socketC, recvmsg, RECVSIZE, 0, (struct sockaddr *)&addressClient, &lenClient);
        if(strcmp(recvmsg, "SYN")==0){
            printf("Server: HANDSHAKE 1 Recieved.\n");
            memset(sendmsg, 0, RECVSIZE);
            strcpy(sendmsg, "SYN-ACK");
            //Give the new port
            char portString[5];
            port ++;
            sprintf(portString, "%4d", port);
            strcat(sendmsg,portString);
            sendto(socketC, sendmsg, RECVSIZE, 0, (struct sockaddr *)&addressClient, lenClient);
            printf("Server: HANDSHAKE 2 Sended.\n");
        }else if(strcmp(recvmsg, "ACK")==0){
            printf("Server: HANDSHAKE 3 Recieved.\n");

            pid_t fpid;
            fpid = fork();
            if(fpid == 0){
              close(socketC);
              //Creat Socket for Client
        	    int socketU = socket(AF_INET,SOCK_DGRAM,0);
        	    if(socket<0){
          		  perror("ERROR: Cannot creat socket for Client.\n");
          		  return -1;
              }

        	    int validU = 1;
        	    setsockopt(socketU, SOL_SOCKET, SO_REUSEADDR, &validU, sizeof(int));

        	    //Creat Socket Client's Address
        	    struct sockaddr_in addressU;
        	    addressU.sin_family = AF_INET;
        	    addressU.sin_addr.s_addr = htonl(INADDR_ANY);
              addressU.sin_port = htons(port);
        	    socklen_t lenU = sizeof(addressU);

              if(bind(socketU, (struct sockaddr *)&addressU,lenU)==-1){
                  perror("Bind for Client Failed.\n");
                  close(socketU);
                  close(socketC);
                  return -1;
              }

              //Open the File
              memset(recvmsg, 0, RECVSIZE);
              recvfrom(socketU, recvmsg, RECVSIZE, 0, (struct sockaddr *)&addressClient, &lenClient);
              FILE *fp = fopen(recvmsg, "r");
              if(fp == NULL){
                  memset(sendmsg, 0, RECVSIZE);
                  strcpy(sendmsg, "Cannot Find Your File!");
                  printf("Cannot find the file!\n");
                  sendto(socketU, sendmsg, RECVSIZE, 0, (struct sockaddr *)&addressClient, lenClient);
              }else{
                  //Create Sequence Number
                  int seqRecv = 0;
                  int seqSend = 1;
                  int endFile = 0;

                  //Create Window Size
                  int wSizeCste = 1;
                  int wOccupiedSize = 0;

                  //Create Congestion Controle
                  int ssthred=64;
                  int congst = 0;
                  float wSizeFloat = 1.0;


                  //Create RTT;
                  double srtt = 3.0;
                  double timeuse = 0.0;
                  struct timeval starttime;
                  struct timeval endtime;

                  //Tansmission of File
                  struct timeval totalStarttime;
                  struct timeval totalEndtime;
                  gettimeofday(&totalStarttime,NULL);

                  //Transmission of File
                  char buffer[RECVSIZE-6];
                  memset(buffer, 0, RECVSIZE-6);
                  int len = 1;
                  int lenEnd = 0;
                  gettimeofday(&starttime,NULL);
                  if((len = fread(buffer, sizeof(char), RECVSIZE-6, fp))!=0){
                    lenEnd = len;
                    memset(sendmsg, 0, RECVSIZE);
                    sprintf(sendmsg,"%.6d",seqSend);
                    memcpy(sendmsg+6, buffer, len);
                    //Send to Client
                    printf("SEND TO CLIENT: Segement %.6d, Size: %ld \n", seqSend, sizeof(sendmsg));
                    sendto(socketU, sendmsg, len+6, 0,(struct sockaddr *)&addressClient, lenClient);
                    seqSend=seqSend+1;
                    wOccupiedSize = wOccupiedSize+1;
                    memset(buffer, 0, RECVSIZE-6);
                  }

                  //Parameters of select
                  fd_set fds;
                  struct timeval timeout={3,0}; //Also srtt

                  //Fast Recovery
                  int seqDup = 0;
                  int nSeqDup = 0;

                  while(endFile ==0){
                    FD_ZERO(&fds);
                    FD_SET(socketU, &fds);
                    int maxfdp = socketU+1;

                    timeout.tv_sec = (int)srtt;
                    timeout.tv_usec = (srtt - timeout.tv_sec)*1000000;
                    //printf("Valeur for TIMEOUT is: %ld.%ld\n", timeout.tv_sec, timeout.tv_usec);
                    switch(select(maxfdp,&fds,NULL,NULL,&timeout)){
                      case -1: exit(-1);
                        break;
                      case 0:

                        //printf("\nTIMEOUT!!! Fast Retransmit Here!\n");
                        gettimeofday(&endtime,NULL);
                        timeuse = (endtime.tv_sec - starttime.tv_sec) + (endtime.tv_usec - starttime.tv_usec)/1000000.0;
                        srtt = 0.8*srtt + 0.2*1;

                        fseek(fp, -(wOccupiedSize-1)*(RECVSIZE-6)-lenEnd, SEEK_CUR);
                        seqSend  = seqSend - wOccupiedSize;
                        wOccupiedSize = 0;
                        if(congst == 1){
                          ssthred = (int)wSizeCste/2;
                          congst = 0;
                        }
                        wSizeCste = 1;
                        wSizeFloat = wSizeCste;
                        gettimeofday(&starttime,NULL);
                        while((wOccupiedSize<wSizeCste)&&((len = fread(buffer, sizeof(char), RECVSIZE-6, fp))!=0)){
                          lenEnd = len;
                          memset(sendmsg, 0, RECVSIZE);
                          sprintf(sendmsg,"%.6d",seqSend);
                          memcpy(sendmsg+6, buffer, len);
                          //printf("SEND TO CLIENT: Segement %.6d, Size: %d \n", seqSend, len);
                          sendto(socketU, sendmsg, len+6, 0,(struct sockaddr *)&addressClient, lenClient);
                          seqSend=seqSend+1;
                          wOccupiedSize = wOccupiedSize + 1;
                          memset(buffer, 0, RECVSIZE-6);
                        }
                        break;
                      default:
                        if(FD_ISSET(socketU,&fds)){
                          if(wSizeCste>=ssthred){
                            congst = 1;
                          }
                          if(congst == 0){
                            wSizeCste = wSizeCste + 1;
                            wSizeFloat = wSizeCste;
                          }else{
                            wSizeFloat = wSizeFloat + 1.0/wSizeCste;
                            wSizeCste = (int)wSizeFloat;
                          }
                          memset(recvmsg, 0, RECVSIZE);
                          recvfrom(socketU, recvmsg, 9, 0, (struct sockaddr *)&addressClient, &lenClient);
                          //printf("RECV ACK: %s\n", recvmsg);
                          gettimeofday(&endtime,NULL);
                          timeuse = (endtime.tv_sec - starttime.tv_sec) + (endtime.tv_usec - starttime.tv_usec)/1000000.0;
                          //srtt = 0.8*srtt + 0.2*timeuse;

                          char seqRecvChar[7];
                          strncpy(seqRecvChar,recvmsg+3,6);
                          seqRecv = atoi(seqRecvChar);
                          if(seqRecv == seqSend-wOccupiedSize){
                            srtt = 0.8*srtt + 0.2*timeuse;
                            wOccupiedSize = wOccupiedSize - 1;
                            gettimeofday(&starttime,NULL);
                            while((wOccupiedSize<wSizeCste)&&((len = fread(buffer, sizeof(char), RECVSIZE-6, fp))!=0)){
                              lenEnd = len;
                              memset(sendmsg, 0, RECVSIZE);
                              sprintf(sendmsg,"%.6d",seqSend);
                              memcpy(sendmsg+6, buffer, len);
                              //printf("SEND TO CLIENT: Segement %.6d, Size: %d \n", seqSend, len);
                              sendto(socketU, sendmsg, len+6, 0,(struct sockaddr *)&addressClient, lenClient);
                              seqSend=seqSend+1;
                              wOccupiedSize = wOccupiedSize+1;
                              memset(buffer, 0, RECVSIZE-6);
                            }
                          }else if(seqRecv < seqSend-wOccupiedSize){
                            //Loss of Packet
                            if(seqDup == seqRecv){
                              nSeqDup = nSeqDup + 1;
                            }else{
                              seqDup = seqRecv;
                              nSeqDup = 0;
                            }
                            if(nSeqDup >= 3){
                              nSeqDup = 0;

                              //printf("\n3 Duplicated ACK!!! Fast Recovery Here! \n");

                              ssthred = (int)wSizeCste/2;
                              congst = 1;
                              wSizeCste = ssthred + 3;
                              wSizeFloat = wSizeCste;
                              fseek(fp, -(wOccupiedSize-1)*(RECVSIZE-6)-lenEnd, SEEK_CUR);
                              seqSend = seqSend - wOccupiedSize;
                              wOccupiedSize = 0;
                              gettimeofday(&starttime,NULL);
                              while((wOccupiedSize<wSizeCste)&&((len = fread(buffer, sizeof(char), RECVSIZE-6, fp))!=0)){
                                lenEnd = len;
                                memset(sendmsg, 0, RECVSIZE);
                                sprintf(sendmsg,"%.6d",seqSend);
                                memcpy(sendmsg+6, buffer, len);
                                //printf("SEND TO CLIENT: Segement %.6d, Size: %d \n", seqSend, len);
                                sendto(socketU, sendmsg, len+6, 0,(struct sockaddr *)&addressClient, lenClient);
                                seqSend=seqSend+1;
                                wOccupiedSize = wOccupiedSize + 1;
                                memset(buffer, 0, RECVSIZE-6);
                              }
                            }

                          }else{
                            //After refinding the lost packet
                            srtt = 0.8*srtt + 0.2*timeuse;
                            wOccupiedSize = seqSend - seqRecv - 1;
                            gettimeofday(&starttime,NULL);
                            while((wOccupiedSize<wSizeCste)&&((len = fread(buffer, sizeof(char), RECVSIZE-6, fp))!=0)){
                              lenEnd = len;
                              memset(sendmsg, 0, RECVSIZE);
                              sprintf(sendmsg,"%.6d",seqSend);
                              memcpy(sendmsg+6, buffer, len);
                              //Send to Client
                              //printf("SEND TO CLIENT: Segement %.6d, Size: %d \n", seqSend, len);
                              sendto(socketU, sendmsg, len+6, 0,(struct sockaddr *)&addressClient, lenClient);
                              seqSend=seqSend+1;
                              wOccupiedSize = wOccupiedSize+1;
                              memset(buffer, 0, RECVSIZE-6);
                            }
                          }
                          if((len==0)&&(wOccupiedSize==0)){
                            endFile = 1;
                          }
                        }
                    }
                  }
                  gettimeofday(&totalEndtime,NULL);
                  double difference;
                  difference = (totalEndtime.tv_sec-totalStarttime.tv_sec) + (totalEndtime.tv_usec-totalStarttime.tv_usec)/1000000.0;
                  int size;
                  size = (seqSend-2)*(RECVSIZE-6)+lenEnd;
                  double debit;
                  debit = size/difference;
                  printf("Time = %lf, Size = %d, Debit = %lf\n",difference,size,debit);
                }
                memset(sendmsg, 0, RECVSIZE);
                strcpy(sendmsg,"FIN");
                printf("FIN\n");
                sendto(socketU, sendmsg, RECVSIZE, 0,(struct sockaddr *)&addressClient, lenClient);
    	          close(socketU);
                return 0;
            }

        }else{
            perror("ERROR: UNKNOW ERROR\n");
            return -1;
        }
    }
    close(socketC);
    return 0;
}
