#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>          /* These are the usual header files */
#include <strings.h>          /* for bzero() */
#include <unistd.h>         /* for close() */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "ZipUtils.h"
#include "CDatPacket.h"
#define PORT 6001   /* Port that will be opened */
#define BACKLOG 5   /* Number of allowed connections */
#define MAXDATASIZE 1000

void marshal(const char* code);

CommRequest m_commfield;
char *pSend;
CMemBuffer *pMyBuf;
CMemBuffer *m_pData;
char *pRecv;
int len;
int m_reclen;
bool m_bzip;

void process_cli(int connectfd, sockaddr_in client);
/* function to be executed by the new thread */
void* start_routine(void* arg);
struct  ARG  {
    int connfd;
    sockaddr_in client;
};
void doRegist(){
/*
 {
 "commresponse": {
 "appver": "1.0",
 "code": "01",
 "msg": "Success",
 "retcode": 1,
 "systime": 1384335562
 },
 "lang": "en",
 "otherid": "0",
 "othername": "Player-9145",
 "sex": 1,
 "userid": 9145,
 "username": "Player-9145"
 }*/
   
}
void doLogin(){
}
void doUpdateUserInfo(){
}
void doGetAction(){
}
void doGiveAction(){
}
void doInstalledGame(){
}
int	UnMarshal(int connectfd, sockaddr_in client,char *pData,int len, bool zip)
{
    DAT_HEAD	m_head;
    char *pBuf = pData;
    CommRequest m_commField;
    /////
    memcpy(&m_head, pData, sizeof(m_head));
    pBuf += sizeof(m_head);
    //////
    char *pBuffer;
    
    if(m_head.cZip)
    {
        char *pbuf2;
        pbuf2 = (char*)malloc(m_head.uLen );
        memset(pbuf2, 0, m_head.uLen);
        memcpy(pbuf2, pBuf, m_head.uLen);
        
        for(int i=0;i<m_head.uLen;i++)
            pbuf2[i]-=(BYTE)i;
        
        int length = ZipUtils::ccInflateMemory((unsigned char*)pbuf2, m_head.uLen, (unsigned char**) &pBuffer);
        
        free(pbuf2);
        
        if(length < 0)
        {
            return 0;
        }
    }
    else
    {
        pBuffer = new char[m_head.uLen + 1];
        memset(pBuffer, 0, m_head.uLen + 1);
        memcpy(pBuffer, pBuf, m_head.uLen);
        
    }
    
    
    printf("recv:%s",pBuffer);
    

    json::Json *pJson = NULL;
    json::Json jComm;
    json::JsonValue jv;
    if(json::Json::Parse(pJson, pBuffer) != 0)
    {
        if(pJson)
            delete pJson;
        if(pBuffer)
            delete [] pBuffer;
        return 0;
    }
    
    try {
        jv = (*pJson)["commrequest"];
        jComm = jv;
        m_commField.appver =(char*) jComm["appver"];
        m_commField.snnum = (char*)jComm["snnum"];
        m_commField.verions = (char*)jComm["version"];
        m_commField.sysver = (char*)jComm["sysver"];
        
        const   char* code =   (char*)(*pJson)["code"];
        if (strcmp(code, "01") ==0) {
            CDatRegistResponse *res =new CDatRegistResponse();
            
//            doRegist();
            marshal("01");
        }else if (strcmp(code, "02") ==0) {
            doLogin();
        }else if (strcmp(code, "06") ==0){
            doUpdateUserInfo();
        }else if (strcmp(code, "11") ==0){
            doGetAction();
        }else if (strcmp(code, "17") ==0){
//            Access is already installed little account of the game   17
            doInstalledGame();
        }else if (strcmp(code, "21") ==0){
//            向好友索取或者赠送行动时通知服务器形成一个message（21
            doGiveAction();
        }
        
    } catch (json::JsonException *e) {
        
        printf("%s",e->what());
        if(pJson)
            delete pJson;
        if(pBuffer)
            delete [] pBuffer;
        return 0;
    }
    catch(...)
    {
        
    }
    delete pJson;
	delete [] pBuffer;
	return 1;
    
}


void marshal(const char* code){
    json::Json jsonSend;
    json::Json jsonCommon;
    json::JsonArray jsonList;

  
    
    jsonCommon["appver"] = "1.0";
    jsonCommon["code"] = "01";
    jsonCommon["msg"] = "Success";
    jsonCommon["retcode"] = 1;
    jsonCommon["systime"] = (int)time(NULL);
    
    json::JsonValue jv = jsonCommon;
    jsonSend["commrequest"] = jv;
    
    
    
    jsonSend["lang"] = "en";
    jsonSend["otherid"] = "0";
    jsonSend["othername"] = "Player-9145";
    jsonSend["sex"] = 1;
    jsonSend["userid"] = 9145;
    jsonSend["username"] = "Player_9145";

    std::string strJson;
    jsonSend.Dump(strJson);
    
    printf("%s",strJson.c_str());
    
    DAT_HEAD m_head;
    m_head.key = HEAD_KEY;
    m_head.uLen = strlen(strJson.c_str()) ;
    m_head.cZip = 1;
    
    
    
    char *pBuf = m_pData->GetBuffer(sizeof(m_head) + m_head.uLen);
    memcpy(pBuf, &m_head, sizeof(m_head));
    
    pBuf += sizeof(m_head);
    memcpy(pBuf, strJson.c_str(), m_head.uLen);
    
    char *out = NULL;
    char *in = m_pData->GetData()+sizeof(DAT_HEAD);
    len = ZipUtils::ccDeflateMemory((unsigned char*)in, m_pData->GetLength()-sizeof(DAT_HEAD),(unsigned char**) &out);
    
    if(len<=0)
        return ;
    
    for(int i=0;i<len;i++)
    {
        out[i]+=(BYTE)i;
    }
    
    len+=sizeof(DAT_HEAD);
    pSend = (char*)malloc(len);
    memcpy(pSend, m_pData->GetData(), sizeof(DAT_HEAD));
    memcpy( pSend+sizeof(DAT_HEAD), out, len-sizeof(DAT_HEAD));
    
    DAT_HEAD *pHead = (DAT_HEAD*)pSend;
    pHead->uLen = len-sizeof(DAT_HEAD);

}
int main()
{
	int listenfd, connectfd; /* socket descriptors */
	pthread_t  thread;
	ARG *arg;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in client; /* client's address information */
	socklen_t sin_size;
    
	/* Create TCP socket */
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        /* handle exception */
        perror("Creating socket failed.");
        exit(1);
	}
    
	int opt = SO_REUSEADDR;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(PORT);
	server.sin_addr.s_addr = htonl (INADDR_ANY);
	if (bind(listenfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
        /* handle exception */
        perror("Bind error.");
        exit(1);
    }
    
	if(listen(listenfd,BACKLOG) == -1){  /* calls listen() */
        perror("listen() error\n");
        exit(1);
    }
    
	sin_size=sizeof(struct sockaddr_in);
	while(1)
	{
        printf("listening...\n");
        /* Accept connection */
        if ((connectfd = accept(listenfd,(struct sockaddr *)&client,&sin_size))==-1) {
            perror("accept() error\n");
            exit(1);
        }
        /*  Create thread*/
        
        arg = new ARG;
        arg->connfd = connectfd;
        memcpy((void *)&arg->client, &client, sizeof(client));
        
        if (pthread_create(&thread, NULL, start_routine, (void*)arg)) {
            /* handle exception */
            perror("Pthread_create() error");
            exit(1);
        }
	}
	close(listenfd);   /* close listenfd */
}

void process_cli(int connectfd, sockaddr_in client)
{
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    //    recv
    int flag = setsockopt( connectfd, SOL_SOCKET, SO_RCVTIMEO, ( char * )&tv, sizeof( tv ) );
    if(flag != 0)
    {
        perror("error");
        close(connectfd);
        return ;
    }
    
    
    long byte_num;
    DAT_HEAD head;
    
    {
        byte_num = recv(connectfd, &head, sizeof(DAT_HEAD), 0);
    }
    
    if(byte_num < 0)
    {
        perror("error");
        close(connectfd);
        return ;
    }
    else if(byte_num < sizeof(head) || byte_num > (512 * 1024))
    {
        perror("error");
        close(connectfd);
        return ;
    }
    
    
    char*   pRecv = new char[head.uLen+sizeof(head)];
    
    int read;
    byte_num = sizeof(head);
    
    memcpy(pRecv, &head, sizeof(head));
    
    while(byte_num < (head.uLen + sizeof(head)))
    {
        read = recv(connectfd, (pRecv + byte_num), ((head.uLen + sizeof(head)) - byte_num), 0);
        if(read == 0)
            break;
        byte_num += read;
    }
    
    if(byte_num != (head.uLen + sizeof(head)))
    {
        perror("error");
        close(connectfd);
        return ;
    }
    //    UnMarshal
    UnMarshal( connectfd,  client,pRecv,byte_num,1);
    
    
    //    send
     flag = setsockopt(connectfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv,sizeof(tv) );
    if(flag != 0)
    {
        perror("error");
        close(connectfd);
        return ;
    }
    
    int sent = 0;
    int st = 0;
    
    while(sent<len)
    {
        st =send(connectfd, pSend+sent, len, 0);
        if(st <0)
        {
            perror("send error");
            close(connectfd);
            return ;
        }
        
        sent+=st;
    }
    

	close(connectfd); /*  close connectfd */
}

void* start_routine(void* arg)
{
	ARG *info;
	info = (ARG *)arg;
    
	/* handle client¡¯s requirement */
	process_cli(info->connfd, info->client);
    
	delete info;
	pthread_exit(NULL);
}


