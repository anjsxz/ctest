#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#define PORT            4321
#define BUFFER_SIZE        1024
#define MAX_QUE_CONN_NM   5

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

typedef struct bat
{
    char group[20];
    int  num;
}my;
int main()
{
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    
    driver = sql::mysql::MySQL_Driver::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "user", "password");
    
    delete con;
    return 0;
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
        my buff;
        //调用recv()函数接收客户端请求
        memset(buf , 0, sizeof(buf));
        if ((recvbytes = recv(client_fd,&buff,sizeof(buff), 0)) == -1)
        {
            perror("recv");
            exit(1);
        }
        printf("Received a message: group=%s num=%d\n", buff.group,buff.num);
        memset(buf , 0, sizeof(buf));
        strcpy(buf,"return to client");
        
         strcpy(buff.group,"server");
        buff.num =232;
        if ((sendbytes = send(client_fd,&buff, strlen(buf),0)) == -1)
        {
            perror("send");
            exit(1);
        }
        printf("send message :%s\n",buf);
    }
    close(sockfd);
    exit(0);
}

