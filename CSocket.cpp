//
// Created by hwz on 9/26/21.
//
#include "CSocket.h"
#define NAMEMAX 512-8-8//(UDP推荐长度-UDP报头长度-结构体的长度)
#include <sys/sendfile.h>
#include "CFileAio.h"

CSocket::CSocket():m_iPort(-1){

}

CSocket::CSocket(int iPort,CServiceManager* pServiceManager,int iFd):m_iPort(iPort),m_pServiceManager(pServiceManager),
                                                                     m_socketFD(iFd),m_iBufOffset(0),m_iPriority(0),m_CallbackWrite(nullptr),m_FunctionAddress(nullptr),m_pNext(nullptr),m_pPrev(
                nullptr),m_iFileFD(-1),m_pFile(nullptr){
    memset(m_strIP, 0, SOCKET_IP);
    memset(m_strBuf, 0, SOCKET_BUF);
}

CSocket::~CSocket(){
    CloseSocket();
}

void CSocket::Alive(){
    m_time = time(NULL);
}

void CSocket::CloseSocket(){
    if(m_socketFD!=-1)
        close(m_socketFD);
//    if(m_iFileFD!=-1)
//        close(m_iFileFD);
    if(m_pFile)
        fclose(m_pFile);
}

void CSocket::SetStateReadUDP(){
    //m_FunctionAddress = std::bind(&CSocket::ReceiveUDP, this);
    m_iBufOffset = 0;
    m_FunctionAddress = [this]{this->ReceiveUDP();};
    m_pPoll->PollRead(this);
}

void CSocket::SetStateWriteUDP(){
    //m_FunctionAddress = std::bind(&CSocket::SendUDP, this);
    m_iBufOffset = 0;
    m_FunctionAddress = [this]{this->SendUDP();};
    m_pPoll->PollWrite(this);
}

void CSocket::SetStateReadTCP(){
    //m_FunctionAddress = std::bind(&CSocket::ReceiveTCP, this);
    m_iBufOffset = 0;
    m_FunctionAddress = [this]{this->ReceiveTCP();};
    m_pPoll->PollRead(this);
}

void CSocket::SetStateRead(){
    m_pPoll->PollRead(this);
}

void CSocket::SetStateWrite(){
    m_pPoll->PollWrite(this);
}

void CSocket::SetStateWriteTCP(int iSize){
    //m_FunctionAddress = std::bind(&CSocket::SendTCP, this);
    m_iBufOffset = 0;
    //m_iSendAll = strlen(m_strBuf);//结尾必须有'\0'否则会出错
    m_iSendAll = iSize;//结尾必须有'\0'否则会出错 加密后可能出现\0字符
    m_FunctionAddress = [this]{this->SendTCP();};
    m_pPoll->PollWrite(this);
}

bool CSocket::CheckPort() {
    struct addrinfo hints;
    memset (&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
    hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
    hints.ai_flags = AI_PASSIVE;     /* All interfaces */

    struct addrinfo *result;
    std::string str = std::to_string(m_iPort);
    int s = getaddrinfo (nullptr, str.c_str(), &hints, &result);
    if (s != 0){
        syslog(LOG_ERR ,"CSocket::CheckPort():%s", gai_strerror(s));
        //fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
        return false;
    }
    return true;
}

int CSocket::NonBlock(int sfd)
{
    int flags, s;
    //得到文件状态标志
    flags = fcntl (sfd, F_GETFL, 0);
    if (flags == -1){
        //perror ("fcntl");
        syslog(LOG_WARNING ,"CSocket::NonBlock():%s", gai_strerror(sfd));
        return -1;
    }
    //设置文件状态标志
    flags |= O_NONBLOCK;
    s = fcntl (sfd, F_SETFL, flags);
    if (s == -1){
        //perror ("fcntl");
        syslog(LOG_WARNING ,"CSocket::NonBlock():%s", gai_strerror(sfd));
        return -1;
    }
    return 0;
}

void CSocket::CreateTCP(){
    if(!CheckPort())
        exit(1);
    int on = 1;
    struct sockaddr_in name;

    m_socketFD = socket(PF_INET, SOCK_STREAM, 0/*IPPROTO_TCP*/);
    if (m_socketFD == -1){
        syslog(LOG_ERR ,"CSocket::socket():%s", strerror(errno));
        exit(1);
    }
    //error_die("socket");
    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(m_iPort);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if ((setsockopt(m_socketFD, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0)
    {
        syslog(LOG_ERR ,"CSocket::setsockopt()");
        //error_die("setsockopt failed");
        exit(1);
    }
    if (bind(m_socketFD, (struct sockaddr *)&name, sizeof(name)) < 0){
        syslog(LOG_ERR ,"CSocket::bind()");
        exit(1);
        //error_die("bind");
    }
    NonBlock(m_socketFD);
//    if (m_iPort == 0)  /* if dynamically allocating a port */
//    {
//        socklen_t namelen = sizeof(name);
//        if (getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1)
//            error_die("getsockname");
//        *port = ntohs(name.sin_port);
//    }
    if (listen(m_socketFD, 128/*4096*/) < 0){
        syslog(LOG_ERR ,"CSocket::listen()");
        exit(1);
        //error_die("listen");
    }
    //m_FunctionAddress = std::bind(&CSocket::AcceptListen, this);
    m_FunctionAddress = [this]{this->AcceptListen();};
}

//void CSocket::SetSocketSateHandle(Function &handlefunction){
//    m_FunctionAddress = handlefunction;
//    //m_FunctionAddress = std::bind(&CSocket::AcceptListen, this);
//}

void CSocket::CreateUDP() {
    if(!CheckPort())
        exit(1);
    struct sockaddr_in laddr;//local addr

    m_socketFD = socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
    if (m_socketFD < 0){
        syslog(LOG_ERR ,"CSocket::CreateUDP()");
        exit(1);
    }

    laddr.sin_family = AF_INET;//指定协议
    laddr.sin_port = htons(m_iPort);//指定网络通信端口
    laddr.sin_addr.s_addr = INADDR_ANY;
    //inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);//IPv4点分式转二进制数

    if(bind(m_socketFD,(struct sockaddr *)&laddr,sizeof(laddr)) < 0){
        syslog(LOG_ERR ,"CSocket::CreateUDP()");
        exit(1);
    }
    NonBlock(m_socketFD);
    //m_FunctionAddress = std::bind(&CSocket::RecieveUDP, this);
}

void CSocket::Receive() {
    int optval = 0;
    socklen_t optlen = sizeof(int);
    getsockopt(m_socketFD, SOL_SOCKET, SO_TYPE, &optval, &optlen);
    switch(optval){
        case SOCK_STREAM:
            ReceiveTCP();
            break;
        case SOCK_DGRAM:
            ReceiveUDP();
            break;
        default:
            break;
    }
}

void CSocket::Send() {
    int optval = 0;
    socklen_t optlen = sizeof(int);
    getsockopt(m_socketFD, SOL_SOCKET, SO_TYPE, &optval, &optlen);
    switch(optval){
        case SOCK_STREAM:
            SendTCP();
            break;
        case SOCK_DGRAM:
            SendUDP();
            break;
        default:
            break;
    }
}

void CSocket::AcceptListen(){
    struct sockaddr in_addr;
    socklen_t in_len;

    in_len = sizeof in_addr;
    int infd =-1;
    //int infd = accept (m_socketFD, &in_addr, &in_len);
    while((infd = accept4 (m_socketFD, &in_addr, &in_len,SOCK_NONBLOCK))>0){
        char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
        //将地址转化为主机名或者服务名
        //主机地址和服务地址
        //if (getnameinfo (&in_addr, in_len,hbuf, sizeof hbuf,sbuf, sizeof sbuf,NI_NUMERICHOST | NI_NUMERICSERV) == 0)
        //std::cout<<"accept no:"<<infd<<std::endl;
        getnameinfo (&in_addr, in_len,hbuf, sizeof hbuf,sbuf, sizeof sbuf,NI_NUMERICHOST | NI_NUMERICSERV);
        m_pPoll->Accept(infd,this,hbuf);
        //m_CallbackAccept(infd,m_strIP);
    }

    if (infd == -1){
        if (errno != EAGAIN &&
            errno != ECONNABORTED &&
            errno != EPROTO &&
            errno != EINTR)
            syslog(LOG_WARNING ,"CSocket::accept():%s", gai_strerror(m_socketFD));
//        if ((errno == EAGAIN) ||
//            ){
//            /* We have processed all incomingconnections. */
//            //break;
//        }
//        else
//            syslog(LOG_WARNING ,"CSocket::accept():%s", gai_strerror(m_socketFD));
//        return;
    }
}

//一个线程在读取完某个socket上的数据后开始处理这些数据，而数据的处理过程中该socket
// 又有新数据可读，此时另外一个线程被唤醒来读取这些新的数据。
//于是，就出现了两个线程同时操作一个socket的局面。可以使用epoll的EPOLLONESHOT事件
// 实现一个socket连接在任一时刻都被一个线程处理。
void CSocket::ReceiveTCP() {
    /* We have data on the fd waiting to be read. Read and
//      display it. We must read whatever data is available
//       completely, as we are running in edge-triggered mode
//       and won't get a notification again for the same
//       data. */
    //int done = 0;
    while (true){
        ssize_t count = read (m_socketFD, m_strBuf+m_iBufOffset, SOCKET_BUF-m_iBufOffset-1);
        if(count>0){
            //strcat(m_strIP,"3");
            m_iBufOffset += count;
            if(m_pServiceManager->HandleRev(m_strBuf,m_iBufOffset,this)==0)
                break;
        }else if (count == -1){
            /* If errno == EAGAIN, that means we have read all
               data. So go back to the main loop. */
            //if(errno == EAGAIN) {
            //    m_pProtocol->get()->HandleRev(m_strBuf,m_iBufOffset,this);
            //}else/*(errno != EAGAIN)*/
            //strcat(m_strIP,"4");
            if(errno != EAGAIN){
                printf("ReceiveTCP\n");
                syslog(LOG_WARNING ,"CSocket::ReceiveTCP():%s", gai_strerror(m_socketFD));
            }
            m_pPoll->PollRead(this);
            break;
        }
        else if (count == 0){
            /* End of file. The remote has closed the
               connection. */
            //CloseSocket();
            //strcat(m_strIP,"5");
            //printf("%s",m_strIP);
            //m_iPort = 0;
            m_pPoll->FreeSocket(this);
            break;
        }
    }
}

void CSocket::SendTCP() {
    int iSend = m_iSendAll-m_iBufOffset;
    while(iSend>0){
        int iWrite = write(m_socketFD, m_strBuf+m_iBufOffset, iSend);
        if(iWrite>0){
            //strcat(m_strIP,"6");
            m_iBufOffset += iWrite;
            iSend -= iWrite;
        }
        if (iWrite < iSend) {
            //strcat(m_strIP,"7");
            if (iWrite == -1 && errno != EAGAIN)
                syslog(LOG_WARNING ,"CSocket::SendTCP():%s", gai_strerror(m_socketFD));
            break;
        }
    }
    if(m_iBufOffset==m_iSendAll&&m_CallbackWrite)
        m_CallbackWrite(this);
    else
        m_pPoll->PollWrite(this);
}

void CSocket::ReceiveUDP() {

}

void CSocket::SendUDP() {

}

void CSocket::FileInit(const std::string &strFile,int iSize,char* pMode){

}

void CSocket::SendFileAio(){
}

void CSocket::SendFile(){
}

std::string CSocket::get_file_name (const int fd)
{
//    if (0 >= fd) {
//        return std::string ();
//    }
    char buf[1024] = {'\0'};
    char file_path[PATH_MAX] = {'0'}; // PATH_MAX in limits.h
    snprintf(buf, sizeof (buf), "/proc/self/fd/%d", fd);
    if (readlink(buf, file_path, sizeof(file_path) - 1) != -1)
        return std::string (file_path);
    return std::string ();
}

void CSocket::ReceiveFile(){

}


void CSocket::ReceiveFileAio(){

}

