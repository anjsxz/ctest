#include <strings.h>
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
  if(arc !=2){
    printf("Usage:%s <IP Address>\n",argv[0]);
    exit(1);
  }
  if((he=gethostbyname(argv[1]))==NULL){
    printf("gethostbyname() error\n");
    exit(1);
  }
  if((fd=socket(AF_IENT,SOCK_STREAM,0)) == -1){
    printf("socket() error\n");
    exit(1);
  }
  bzero(&server,sizeof(server));
  server.sin_family =AF_INET;
  server.sin_port =htons(PORT);
  server.sin_addr = *((struct in_addr*)he->h_addr);
  if((connect(fd,(struct sockaddr*)&server,sizeof(struct sockaddr)) ==-1){
      print("connect() error\n");
      eixt(1);
    }
    process(stdin,fd);
    clsoe(fd);    
}
  void process(FILE*fp,int sockfd)
  {
  
  }

  int listenfd,connectfd;
  struct sockaddr_in server;
  struct sockaddr_in clinet;
  int sin_size;

  //create TCP socket
  if((listenfd = socket(AF_INET,SOCK_STREAM,0))== -1){
    perror("creating socket faild.");
    exit(1);
  }

  int opt = SO_REUSEADDR;
  setsockopt(listenfd,SQL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
  bzero(&server,sizeof(server));
  server.sin_family=AF_INET;
  server.sin_port=htons(PORT);
  server.sin_addr.s_addr=htonl(INADDR_ANY);
  if(bind(listenfd,(struct sockaddr*)&server,sizeof(struct sockaddr))==-1){
    perror("Bind error");
    exit(1);  
  }
  if(listen(listenfd,BACKLOG) == -1){
    perror("listen() error\n";
	   exit(1);
	   }
      sin_size = sizeof(struct sockaddr_in);
    while(1){
      if((connectfd = accept(listenfd,(struct sockaddr*)&client,&sin_size)) ==-1){
	perror("accept() error\n");
	exit(1);
      }
      process_cli(connectfd,client);    
    }
    close(listenfd);

  }
}
void process_cli(int connectfd,sockaddr_in client)
{
  int num;
  char recvbuf[MAXDATASIZE],sendbuf[MAXDATASIZE];
  printf("You got a connection from %s\n",inet_ntoa(client.sin_addr));
  while(num = recv(connectfd,recvbuf,MAXDATASIZE,0)){
    recvbuf[num] ='\0';
    printf("Received client message:%s",recvbuf);
    for(int i=0;i<num-1;i++){
      sendbuf[i] = recvbuf[num-i-2];
    }
    sendbuf[num-1] ='\0';
    send(connectfd,sendbuf,strlen(sendbuf),0);
  }
  close(connectfd);
}