#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#include "mysql.h"
#include "CDatConnect.h"

#define PORT            1234
#define BUFFER_SIZE        1024
#define MAX_QUE_CONN_NM   5
typedef struct bat
{
  char group[20];
  int  num;
}my;
int main()
{
  //    
    
    
  struct sockaddr_in server_sockaddr,client_sockaddr;
  socklen_t sin_size;
  int recvbytes,sendbytes;
  int sockfd, client_fd;
  char buf[BUFFER_SIZE];
  my buff;
  buff.num=10;
  strcpy(buff.group,"bat001");
  //建立socket连接
  if ((sockfd = socket(AF_INET,SOCK_STREAM,0))== -1)
    {
      perror("socket");
      exit(1);
    }
  printf("Socket id = %d\n",sockfd);
    
  //设置sockaddr_in 结构中相关参数
  server_sockaddr.sin_family = AF_INET;
  server_sockaddr.sin_port = htons(PORT);
  server_sockaddr.sin_addr.s_addr = INADDR_ANY;
  bzero(&(server_sockaddr.sin_zero), 8);
    
  int i = 1;//允许重复使用本地址与套接字进行绑定
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));
    
  //调用bind()函数
  if (bind(sockfd, (struct sockaddr *)&server_sockaddr,
	   sizeof(struct sockaddr)) == -1)
    {
      perror("bind");
      exit(1);
    }
  printf("Bind success!\n");
    
  //调用listen()函数，创建未处理请求的队列
  if (listen(sockfd, MAX_QUE_CONN_NM) == -1)
    {
      perror("listen");
      exit(1);
    }
    
    
  char *pRecv;
  char *pSend;
  int len;
  int m_reclen;
  struct timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;
  while(1)
    {

        
      printf("Listening....\n");
      //调用accept()函数，等待客户端连接
      sin_size=sizeof(client_sockaddr);
      if ((client_fd = accept(sockfd,(struct sockaddr *)&client_sockaddr, &sin_size)) == -1)
        {
	  perror("accept");
	  exit(1);
        }
       
        

      //recive
      int flag = setsockopt( client_fd, SOL_SOCKET, SO_RCVTIMEO, ( char * )&tv, sizeof( tv ) );
      if(flag != 0)
        {
	  perror("error");
	  close(client_fd);
	  return false;
        }
        
        
      long byte_num;
      DAT_HEAD head;
        
      {
	byte_num = recv(client_fd, &head, sizeof(DAT_HEAD), 0);
      }
        
      if(byte_num < 0)
        {
	  perror("error");
	  close(client_fd);
	  return false;
        }
      else if(byte_num < sizeof(head) || byte_num > (512 * 1024))
        {
	  perror("error");
	  close(client_fd);
	  return false;
        }
      printf("revice head len: %ld\n",byte_num);
        
      pRecv = new char[head.uLen+sizeof(head)];
        
      int read;
      byte_num = sizeof(head);
        
      memcpy(pRecv, &head, sizeof(head));
        
      while(byte_num < (head.uLen + sizeof(head)))
        {
	  read = recv(client_fd, (pRecv + byte_num), ((head.uLen + sizeof(head)) - byte_num), 0);
	  if(read == 0)
	    break;
	  byte_num += read;
        }
        
      if(byte_num != (head.uLen + sizeof(head)))
        {
	  perror("error");
	  close(client_fd);
	  return false;
        }
       
      //send
      flag = setsockopt(client_fd, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv,sizeof(tv) );
      if(flag != 0)
        {
	  perror("error");
	  close(client_fd);
	  return false;
        }
        
      int sent = 0;
      int st = 0;
        
      while(sent<len)
        {
	  st =send(client_fd, pSend+sent, len, 0);
	  if(st <0)
            {
	      perror("send error");
	      close(client_fd);
	      return false;
            }
            
	  sent+=st;
        }
    
    }
  close(sockfd);
  exit(0);
}
void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}
