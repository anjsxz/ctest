#include "CDatConnect.h"
//#include <error.h>
#include <errno.h>
#import "zlib.h"
CDatConnect::CDatConnect()
{
    m_bzip = false;
    m_reclen = 0;
    pRecv = NULL;
    pSend = NULL;
    len = 0;
}
CDatConnect::~CDatConnect()
{
    if(pRecv)
        delete [] pRecv;
    pRecv = NULL;
}

void CDatConnect::setCommon(CommRequest comm)
{
    m_commfield.appver = comm.appver;
    m_commfield.snnum = comm.snnum;
    m_commfield.sysver = comm.sysver;
    m_commfield.verions = comm.verions;
}

bool CDatConnect::DoRequest(CDatRequest *pReq, CDatResponse *pRes,int num)
{
    pReq->m_commField.appver = m_commfield.appver;
    pReq->m_commField.snnum = m_commfield.snnum;//m_commfield.snnum;
    pReq->m_commField.sysver = m_commfield.sysver;
    pReq->m_commField.verions = m_commfield.verions;
    
    pReq->Marshal();
    
    char *out = NULL;
//    char *in = pReq->m_pData->GetData()+sizeof(DAT_HEAD);
//    len = ZipUtils::ccDeflateMemory((unsigned char*)in, pReq->m_pData->GetLength()-sizeof(DAT_HEAD),(unsigned char**) &out);
    len = pReq->m_pData->GetLength()-sizeof(DAT_HEAD);
    out = pReq->m_pData->GetData()+sizeof(DAT_HEAD);
    if(len<=0)
        return false;
    
    for(int i=0;i<len;i++)
    {
        out[i]+=(BYTE)i;
    }
    
    len+=sizeof(DAT_HEAD);
    pSend = (char*)malloc(len);
    memcpy(pSend, pReq->m_pData->GetData(), sizeof(DAT_HEAD));
    memcpy( pSend+sizeof(DAT_HEAD), out, len-sizeof(DAT_HEAD));
    
    DAT_HEAD *pHead = (DAT_HEAD*)pSend;
    pHead->uLen = len-sizeof(DAT_HEAD);

    bool bflag;
    for(int i = 0; i < num; i++)
    {
        //bflag = ConnectUdp();
        bflag = ConnectTcp();
        if(bflag == true)
            break;
    }
    
    if(out) delete [] out;
    if(pSend) free(pSend),pSend = NULL;
    
    if(bflag != true)
        return false;
 
    pRes->UnMarshal(pRecv,m_reclen,m_bzip);

    
    return true;
}


bool CDatConnect::ConnectTcp()
{
    struct hostent *hptr;
    
    if((hptr = gethostbyname(LINK_MYURL)) == NULL)
    {
        printf(" gethostbyname error for host:%s\n", LINK_MYURL);
        return false;
    }
    
    if(!hptr->h_addr_list[0])
    {
        perror("host error");
        return false;
    }
    
    struct sockaddr_in server_addr;
#if CC_TEXTURE_ATLAS_USE_VAO
    server_addr.sin_len = sizeof(struct sockaddr_in);
#else
#endif
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(MYSERVER_PORT);
    server_addr.sin_addr = *((struct in_addr *)hptr->h_addr_list[0]);
    //server_addr.sin_addr.s_addr = inet_addr(LINK_MYURL);
    bzero(&(server_addr.sin_zero), 8);
    

    int server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock_fd == -1)
    {
        perror("socket error");
        return false;
    }
    
    if (connect(server_sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in))==0)
    {
        struct timeval tv;
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        
        int flag = setsockopt(server_sock_fd, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv,sizeof(tv) );
        if(flag != 0)
        {
            perror("error");
            close(server_sock_fd);
            return false;
        }
        
        int sent = 0;
        int st = 0;
        
        while(sent<len)
        {
            st =send(server_sock_fd, pSend+sent, len, 0);
            if(st <0)
            {
                perror("send error");
                close(server_sock_fd);
                return false;
            }
            
            sent+=st;
        }
        
        
        flag = setsockopt( server_sock_fd, SOL_SOCKET, SO_RCVTIMEO, ( char * )&tv, sizeof( tv ) );
        if(flag != 0)
        {
            perror("error");
            close(server_sock_fd);
            return false;
        }
        
        
        long byte_num;
        DAT_HEAD head;
        
        {
            byte_num = recv(server_sock_fd, &head, sizeof(DAT_HEAD), 0);
        }
        
        if(byte_num < 0)
        {
            perror("error");
            close(server_sock_fd);
            return false;
        }
        else if(byte_num < sizeof(head) || byte_num > (512 * 1024))
        {
            perror("error");
            close(server_sock_fd);
            return false;
        }
        
        
        pRecv = new char[head.uLen+sizeof(head)];
        
        int read;
        byte_num = sizeof(head);
        
        memcpy(pRecv, &head, sizeof(head));
        
        while(byte_num < (head.uLen + sizeof(head)))
        {
            read = recv(server_sock_fd, (pRecv + byte_num), ((head.uLen + sizeof(head)) - byte_num), 0);
            if(read == 0)
                break;
            byte_num += read;
        }
        
        if(byte_num != (head.uLen + sizeof(head)))
        {
            perror("error");
            close(server_sock_fd);
            return false;
        }
        else
            return true;
    }
    
    return false;
}

bool CDatConnect::ConnectUdp()
{
    struct sockaddr_in server_addr;
#if CC_TEXTURE_ATLAS_USE_VAO
    server_addr.sin_len = sizeof(struct sockaddr_in);
#else
#endif
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(MYSERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(LINK_MYURL);
    bzero(&(server_addr.sin_zero), 8);
    
    if(pRecv) delete [] pRecv;
    pRecv = new char[BUFFER_SIZE];
    memset(pRecv,0,BUFFER_SIZE);    
    /////////////////////
    int server_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(server_sock_fd == -1)
    {
        perror("socket error");
        return false;
    }
    
    struct timeval tv;
    tv.tv_sec = 15;
    tv.tv_usec = 0;
    
    int flag = setsockopt(server_sock_fd, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv,sizeof(tv) );
    if(flag != 0)
    {
        perror("error");
    }
    
    flag = sendto(server_sock_fd, pSend, len, 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
    
    if( flag<= 0)
    {
        
        perror("error");
        close(server_sock_fd);
        return true;
    }
    
    
    flag = setsockopt( server_sock_fd, SOL_SOCKET, SO_RCVTIMEO, ( char * )&tv, sizeof( tv ) );
    if(flag != 0)
    {
        perror("error");
    }

    socklen_t src_len = sizeof(server_addr);
    
    int byte_num = recvfrom(server_sock_fd, pRecv, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &src_len);
    

    
    if(byte_num > 0)
    {
        
       close(server_sock_fd);
        return true;
        
        
    }
    else if(byte_num < 0)
    {
        perror("error");
        close(server_sock_fd);
        return false;
  
    }
    else
    {
        perror("error");
        close(server_sock_fd);
        return false;

        
    }
    return true;
}


bool CDatConnect::Connect()
{
    

    struct sockaddr_in server_addr;
#if CC_TEXTURE_ATLAS_USE_VAO
    server_addr.sin_len = sizeof(struct sockaddr_in);
#else
#endif
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(MYSERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(LINK_MYURL);
    bzero(&(server_addr.sin_zero), 8);
    
    
    int server_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(server_sock_fd == -1)
    {
        perror("socket error");
        return false;
    }
    int isend = 2000;
    
//    if (connect(server_sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in))!=0)
//    {
//        printf("connect error");
//    }
//    
//    setsockopt(server_sock_fd, SOL_SOCKET, SO_SNDTIMEO, (char*)&isend,sizeof(int) );
    int error;
//    setsockopt(server_sock_fd, SOL_SOCKET, SO_ERROR, (char*)&error, sizeof(int));
    int flag = sendto(server_sock_fd, pSend, len, 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
    perror("error");
    if( flag< 0)
    {
        
        printf("send error %d %d",flag,error);
        close(server_sock_fd);
        return true;
    }

//    printf("%s",errno);
    
    long byte_num;
    
    struct timeval tv;
    tv.tv_sec = 15;
    tv.tv_usec = 0;
    
    pRecv = new char[sizeof(DAT_HEAD)];
    flag = setsockopt( server_sock_fd, SOL_SOCKET, SO_RCVTIMEO, ( char * )&tv, sizeof( tv ) );
    perror("error");
    socklen_t src_len = sizeof(server_addr);

    byte_num = recvfrom(server_sock_fd, pRecv, sizeof(DAT_HEAD), 0, (struct sockaddr *)&server_addr, &src_len);
    if(byte_num <= 0)
    {
        printf("recv error %d",byte_num);
    }
    else {
        printf("recv ok");
    }
    close(server_sock_fd);
    return true;
   
   
    if (connect(server_sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in))==0) {
//         printf("connet ok");
//        close(server_sock_fd);
//        return true;
        
        
        fd_set client_fd_set;
        struct timeval tv;
        tv.tv_sec = 2000;
        tv.tv_usec = 2000;
        
        int isend = 20;
        
        setsockopt(server_sock_fd, SOL_SOCKET, SO_SNDTIMEO, (char*)&isend,sizeof(isend) );
        if(send(server_sock_fd, pSend, len, 0) == -1)
        {
                printf("send error");
        }
        int flag = setsockopt(server_sock_fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&isend,sizeof(isend) );
        long byte_num;

        pRecv = new char[sizeof(DAT_HEAD)];
        byte_num = recv(server_sock_fd, pRecv, sizeof(DAT_HEAD), 0);
        if(byte_num <= 0)
        {
            printf("recv error %d",byte_num);
        }
        else {
            printf("recv ok ");
        }
        DAT_HEAD				m_head;
        memcpy(&m_head, pRecv, sizeof(m_head));
        m_reclen = m_head.uLen;
        m_bzip = m_head.cZip;
        byte_num =recv(server_sock_fd,pRecv,m_head.uLen,0);

        if(byte_num <= 0)
        {
            printf("recv error %d",byte_num);
        }
        else {
            printf("recv ok ");
        }
        
        
        close(server_sock_fd);
        return true;
        
        while (1) {
            FD_ZERO(&client_fd_set);
            FD_SET(server_sock_fd, &client_fd_set);
            
            int ret = select(server_sock_fd + 1, &client_fd_set, NULL, NULL, &tv);
            if (ret < 0) {
                printf("select error");
                close(server_sock_fd);
                return true;
            }
            else if(ret == 0)
            {
                printf("select time out");
                close(server_sock_fd);
                return true;
            }
            else
            {

                
                if(FD_ISSET(server_sock_fd, &client_fd_set))
                {
                    
                    
                    long byte_num;
                    if(m_reclen > 0)
                    {
                        pRecv = new char[m_reclen];
                        byte_num = recv(server_sock_fd, pRecv, m_reclen, 0);
                    }
                    else
                    {
                        pRecv = new char[sizeof(DAT_HEAD)];
                        byte_num = recv(server_sock_fd, pRecv, sizeof(DAT_HEAD), 0);
                    }
                    if(byte_num > 0)
                    {

//                        printf("recive");
                        break;
                        
                        if(m_reclen > 0)
                        {
                            break;
                        }

                        DAT_HEAD				m_head;
                        memcpy(&m_head, pRecv, sizeof(m_head));
                        m_reclen = m_head.uLen;
                        m_bzip = m_head.cZip;
                        
                        free(pRecv);
             

                    }
                    else if(byte_num < 0)
                    {
                        printf("recv error");
                        break;
                    }
                    else 
                    {
                        printf("server quit  ");
                        close(server_sock_fd);
                        break;
                       
                    }
                    
                }
                
            }
        }
    }
    else {
        printf("connet error");
    }
    close(server_sock_fd);
    return true;
    
}
