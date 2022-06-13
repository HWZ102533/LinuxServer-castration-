//
// Created by admin on 4/14/22.
//

#include <sys/syslog.h>
#include "CSocket.h"
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <sys/epoll.h>
#include <string.h>
#include <sys/unistd.h>
#include <stdlib.h>
#include <sys/sendfile.h>
CSocket::CSocket():m_protocol(&m_CryptoAES) {}

CSocket::~CSocket() {
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_bStop = true;
    }
    m_condition.notify_all();
    if(m_tid_reader!=-1)
    pthread_join(m_tid_reader, nullptr);
}

CSocket::CSocket(const char *pIP, const char *pPort):m_strIP(pIP),m_strPort(pPort),m_protocol(&m_CryptoAES),m_tid_reader(-1),m_bStop(false){

}

bool CSocket::Connect() {
    int len;
    struct sockaddr_in address;
    int result;

    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(m_strIP.c_str());//"127.0.0.1"
    address.sin_port = htons(atoi(m_strPort.c_str()));//1986
    len = sizeof(address);
    result = connect(m_sockfd, (struct sockaddr *)&address, len);
    if (result == -1){
        printf( "%s\n", strerror(errno));
        //perror("oops: client1");
        return false;
    }
    NonBlock(m_sockfd);
    pthread_create(&m_tid_reader,NULL,
                   [](void *arg)->void*{
                       signal(SIGPIPE, SIG_IGN);
                       CSocket *pThis = reinterpret_cast<CSocket*>(arg);
                       pThis->Loop();
                       return nullptr;
                   }
            ,this);
    //pthread_detach(m_tid_reader);
    return true;
}

void CSocket::ModifyEvent(CSocket *pSocket,int iType) {
    struct epoll_event event;
    event.data.ptr = reinterpret_cast<void*>(pSocket);
    event.events = iType;//读入,边缘触发方式
    if (epoll_ctl (m_iPollfd, EPOLL_CTL_MOD, m_sockfd, &event) == -1)
        syslog(LOG_WARNING ,"ModifyEvent:%s", strerror(errno));
}

void CSocket::AddEvent(CSocket *pSocket,int iType)//EPOLLIN | EPOLLET
{
    struct epoll_event event;
    event.data.ptr = reinterpret_cast<void*>(pSocket);
    event.events = iType;//读入,边缘触发方式
    if (epoll_ctl (m_iPollfd, EPOLL_CTL_ADD, m_sockfd, &event) == -1)
        syslog(LOG_WARNING ,"AddEvent:%s", strerror(errno));
}

void CSocket::ReceiveOneFile(std::string& StrJson,long iSize,std::function<void(char*)> &callback){

    //close(iFileFD);

}

void CSocket::ReceiveFile(const std::string& StrJson,std::string& StrApp,std::function<void(char*)> &&callback) {

}

void CSocket::SendFile(const std::string& StrJson,long iSize,std::function<void(char*)> &&callback){

    callback(nullptr);
}

void CSocket::Loop(){
    m_iPollfd = epoll_create1 (0);
    if (m_iPollfd == -1){
        syslog(LOG_ERR ,"epoll_create:%s", strerror(errno));
        abort ();
    }
    AddEvent(this,EPOLLIN | EPOLLET);
    time_t  seconds = time(NULL);
    int nn = 0;
    struct epoll_event  m_vecEvent;
    while(true){
        std::string strRequest;
        std::function<void(char*)> task;
        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);
            m_condition.wait(lock,[this]{ return this->m_bStop || !this->m_RequestList.empty(); });
            //if(m_bStop && m_RequestList.empty())
            if(m_bStop )
                break;
            strRequest  =  std::move(m_RequestList.front());
            task = std::move(m_funcList.front());
            m_funcList.pop();
            m_RequestList.pop();
        }
        ModifyEvent(this,EPOLLOUT | EPOLLET);
        strcpy(m_strBuf,strRequest.c_str());

        int data_size = m_protocol.HandleSend(m_strBuf,MAXN);
        //std::unique_ptr<char> pData(new char[data_size+1]);
        //m_protocol.HandleRev(m_strBuf,data_size,pData.get());
        int nwrite=0;
        int nLeft = data_size;
        while(true){
            int n = epoll_wait (m_iPollfd, &m_vecEvent, 1, -1);
            if( n<0&&errno!=EINTR)
                syslog(LOG_WARNING ,"epoll_wait:%s", strerror(errno));
            //for (int i = 0; i < n; ++i) {
            if(n>0){
                auto *pSocket = reinterpret_cast<CSocket *>(m_vecEvent.data.ptr);
                if ((m_vecEvent.events & EPOLLERR) ||
                    (m_vecEvent.events & EPOLLHUP)/*||(!(m_vecEvent[i].events & EPOLLIN))*/) {
                    /* An error has occured on this fd, or the socket is not
                       ready for reading (why were we notified then?) */
                    continue;
                }
                else if(m_vecEvent.events & EPOLLIN){
                    //std::unique_ptr<char> pData(new char[iOffSize+1]);
                    ssize_t count = 0;
                    while (true){
                        count = read (m_sockfd, m_strBuf+nLeft, MAXN-nLeft-1);
                        if(count>0){
                            nLeft += count;
                        }else if (count == -1){
                            /* If errno == EAGAIN, that means we have read all
                               data. So go back to the main loop. */
                            //if(errno == EAGAIN) {
                            //    m_pProtocol->get()->HandleRev(m_strBuf,m_iBufOffset,this);
                            //}else/*(errno != EAGAIN)*/
                            if(errno != EAGAIN){
                                printf("ReceiveTCP\n");
                                syslog(LOG_WARNING ,"CSocket::ReceiveTCP():%s", gai_strerror(m_sockfd));
                            }
                            break;
                        }
                        else if (count == 0){
                            /* End of file. The remote has closed the
                               connection. */
                            //CloseSocket();
                            m_bStop = true;
                            break;
                        }
                    }
                    if(m_bStop)
                        break;
                    std::unique_ptr<char> pData(new char[nLeft+1]);
                    int iPos = m_protocol.HandleRev(m_strBuf,nLeft,pData.get());
                    if(iPos!=-1){
                        char *pData2 = pData.get() + iPos;
                        task(pData2);
                        break;
                    }
                }else if(m_vecEvent.events & EPOLLOUT){
                    while (nLeft > 0) {
                        nwrite = write(m_sockfd, m_strBuf + data_size - nLeft, nLeft);
                        if(nwrite>0)
                            nLeft -= nwrite;
                        if (nwrite < nLeft) {
                            if (nwrite == -1 && errno != EAGAIN) {
                                perror("write error");
                            }
                            break;
                        }
                    }
                    if(nLeft==0){
                        nLeft = 0;
                        ModifyEvent(this,EPOLLIN | EPOLLET);
                    }
                }
            }
            //}
        }

    }
    close(m_iPollfd);
    close(m_sockfd);
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

char* CSocket::RequestServer(const std::string& StrJson,std::function<void(char*)> &&callback){
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_funcList.emplace(callback);
        m_RequestList.emplace(StrJson);
    }
    m_condition.notify_one();
    return nullptr;
}

