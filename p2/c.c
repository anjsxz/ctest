#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define PORT   4321
#define BUFFER_SIZE 1024
typedef struct bat
{
  char group[20];
  int  num;
}my;
int main(int argc, char *argv[])
{
  int sockfd,sendbytes,recvbytes;
  char buf[BUFFER_SIZE];
  struct hostent *host;
  struct sockaddr_in serv_addr;
  if(argc < 3)
    {
      fprintf(stderr,"USAGE: ./client Hostname(or ip address) Text\n");
      exit(1);
    }
  //地址解析函数
  if ((host = gethostbyname(argv[1])) == NULL)
    {
      perror("gethostbyname");
      exit(1);
    }
  memset(buf, 0, sizeof(buf));
  sprintf(buf, "%s", argv[2]);
  //创建socket
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror("socket");
      exit(1);
    }     
  //设置sockaddr_in 结构体相关参数    
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
  bzero(&(serv_addr.sin_zero), 8);         
  //调用connect函数主动发起对服务器的连接
  if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr))== -1)
    {
      perror("connect");
      exit(1);
    }
  my buff;
  strcpy(buff.group,"bat001");
  buff.num=24;
  //发消息给服务器端
  if ((sendbytes = send(sockfd,&buff,sizeof(buff), 0)) == -1)
    {
      perror("send");
      exit(1);
    }
  memset(buf,0,sizeof(buf));
  //接收来自服务器端的返回
  if ((recvbytes = recv(sockfd,&buff,sizeof(buf), 0)) == -1)
    {
      perror("recv");
      exit(1);
    }
  printf("Received a message : %s,%d\n",buff.group,buff.num);
  close(sockfd);
  exit(0);
}
