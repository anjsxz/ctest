#include <iostream>
#include<cstring>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 1234
#define BACKLOG 5 //number of allowed connections
#define MAXDATASIZE 1000

void process(FILE*fp,int sockfd);
int main(int argc,char*argv[])
{
    int fd;
    struct hostent *he;
    struct sockaddr_in server;
    if(argc !=2){
        printf("Usage:%s <IP Address>\n",argv[0]);
        exit(1);
    }
    if((he=gethostbyname(argv[1]))==NULL){
        printf("gethostbyname() error\n");
        exit(1);
    }
    if((fd=socket(AF_INET,SOCK_STREAM,0)) == -1){
        printf("socket() error\n");
        exit(1);
    }
    bzero(&server,sizeof(server));
    server.sin_family =AF_INET;
    server.sin_port =htons(PORT);
    server.sin_addr = *((struct in_addr*)he->h_addr);
    if(connect(fd,(struct sockaddr*)&server,sizeof(struct sockaddr)) ==-1){
        printf("connect() error\n");
        exit(1);
    }
    process(stdin,fd);
    close(fd);
}
void process(FILE*fp,int sockfd)
{
    char sendline[MAXDATASIZE],recvline[MAXDATASIZE];
    int numbytes;
    printf("Connected to server .\n");
    while (fgets(sendline, MAXDATASIZE, fp)!= NULL) {
        send(sockfd, sendline, strlen(sendline), 0);
        if ((numbytes = recv(sockfd, recvline, MAXDATASIZE, 0)) == 0) {
            printf("Server terminated.\n");
            return;
        }
        recvline[numbytes]='\n';
        printf("Server Message: %s\n",recvline);
    }
    
}
