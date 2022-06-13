//
// Created by hwz on 9/26/21.
//

#include "CLoopHandle.h"
#include <sys/epoll.h>
#include "CSocket.h"
#include "string.h"
#include <time.h>
#define EVENTS 4096

CLoopHandle::CLoopHandle():m_iPollfd(-1),m_vecEvent(EVENTS){

}

CLoopHandle::CLoopHandle(boost::pool<> *poolSocket):m_poolSocket(poolSocket),m_iPollfd(-1),m_vecEvent(EVENTS),m_pFirst(
        nullptr),m_pLast(nullptr){

}

CLoopHandle::~CLoopHandle(){
    if(m_iPollfd!=-1)
        close(m_iPollfd);
}

void CLoopHandle::AddEvent(CSocket *pSocket,int iType)//EPOLLIN | EPOLLET
{
    pSocket->SetLoop(this);
    struct epoll_event event;
    event.data.ptr = reinterpret_cast<void*>(pSocket);
    event.events = iType;//读入,边缘触发方式
    if (epoll_ctl (m_iPollfd, EPOLL_CTL_ADD, pSocket->GetSocketFd(), &event) == -1)
        syslog(LOG_WARNING ,"AddEvent:%s", strerror(errno));
}

void CLoopHandle::DeleteEvent(CSocket *pSocket) const{
    if(epoll_ctl(m_iPollfd, EPOLL_CTL_DEL, pSocket->GetSocketFd(), nullptr) == -1)
        //perror("epoll del error");
        syslog(LOG_WARNING,"DeleteEvent error:%s", strerror(errno));
    else{
        pSocket->CloseSocket();
        //delete pSocket;//this class should be memory pool
    }
}

void CLoopHandle::ModifyEvent(CSocket *pSocket,int iType) {
    AddSocket(pSocket);//be sure before epoll event , socket in used socket list.
    struct epoll_event event;
    event.data.ptr = reinterpret_cast<void*>(pSocket);
    event.events = iType;//读入,边缘触发方式
    if (epoll_ctl (m_iPollfd, EPOLL_CTL_MOD, pSocket->GetSocketFd(), &event) == -1)
        syslog(LOG_WARNING ,"ModifyEvent:%s", strerror(errno));
}

void CLoopHandle::ClearConnect(){
    time_t now = time(NULL);
    std::unique_lock<std::mutex> lock(m_MemoryPool_mutex);
    while(m_pLast){
        clock_t now2 =  m_pLast->GetTime();
        if(now-m_pLast->GetTime()>300){
            //printf("ClearConnect()\n");
            CSocket *pSocket = m_pLast->GetNext();
            if(pSocket)
                pSocket->SetPrev(nullptr);
            m_pLast->~CSocket();
            m_poolSocket->free(m_pLast);
            m_pLast = pSocket;
        }else
            break;
    }
    if(m_pLast== nullptr)
        m_pFirst = nullptr;
}

void CLoopHandle::AddSocket(CSocket *pSocket){
    {
        std::unique_lock<std::mutex> lock(m_MemoryPool_mutex);
        //strcat(pSocket->GetIP(),"8");
        pSocket->SetNext(nullptr);
        pSocket->SetPrev(m_pFirst);
        if(m_pLast== nullptr)
            m_pLast = pSocket;
        else
            m_pFirst->SetNext(pSocket);
        m_pFirst = pSocket;
    }
    pSocket->Alive();
}

void CLoopHandle::RemoveSocket(CSocket *pSocket){
    {
        std::unique_lock<std::mutex> lock(m_MemoryPool_mutex);
        if (pSocket->GetNext())
            pSocket->GetNext()->SetPrev(pSocket->GetPrev());
        if (pSocket->GetPrev())
            pSocket->GetPrev()->SetNext(pSocket->GetNext());
        if(pSocket==m_pFirst)
            m_pFirst = pSocket->GetPrev();
        if(pSocket==m_pLast)
            m_pLast = pSocket->GetNext();
    }
    pSocket->SetNext(nullptr);
    pSocket->SetPrev(nullptr);
}

void CLoopHandle::FreeSocket(CSocket* pSocket){
    pSocket->~CSocket();
    std::unique_lock<std::mutex> lock(m_MemoryPool_mutex);
    //strcat(pSocket->GetIP(),"9");
    //pSocket->~CSocket();
    m_poolSocket->free(pSocket);
}

CSocket *CLoopHandle::MallocSocket(int iFD,CSocket *pSocketListen){
    CSocket *pSocket = nullptr;
    {
        std::unique_lock<std::mutex> lock(m_MemoryPool_mutex);
        //printf("1\n");
        //try { new (ret) element_type(); }
        pSocket =  new (reinterpret_cast<CSocket*>(m_poolSocket->malloc()))CSocket(-1, pSocketListen->GetService(),iFD);
        pSocket->SetPrev(m_pFirst);
        if(m_pLast== nullptr)
            m_pLast = pSocket;
        else
            m_pFirst->SetNext(pSocket);
        m_pFirst = pSocket;
        //printf("1-end\n");
    }
    return  pSocket;
}

void CLoopHandle::FreeSocketInlist(CSocket* pSocket) {
    std::unique_lock<std::mutex> lock(m_MemoryPool_mutex);
    if (pSocket->GetNext())
        pSocket->GetNext()->SetPrev(pSocket->GetPrev());
    if (pSocket->GetPrev())
        pSocket->GetPrev()->SetNext(pSocket->GetNext());
    if(pSocket==m_pFirst)
        m_pFirst = pSocket->GetPrev();
    if(pSocket==m_pLast)
        m_pLast = pSocket->GetNext();
    pSocket->~CSocket();
    m_poolSocket->free(pSocket);
}

void CLoopHandle::ActiveSocket(CSocket *pSocket){

    std::unique_lock<std::mutex> lock(m_MemoryPool_mutex);
    if(pSocket== m_pFirst)
        return;
    //printf("2\n");
    if (pSocket->GetNext())
        pSocket->GetNext()->SetPrev(pSocket->GetPrev());
    if (pSocket->GetPrev())
        pSocket->GetPrev()->SetNext(pSocket->GetNext());
    if(pSocket==m_pLast)
        m_pLast = pSocket->GetNext();

    pSocket->SetPrev(m_pFirst);
    m_pFirst->SetNext(pSocket);
    m_pFirst= pSocket;
    pSocket->SetNext(nullptr);
    //printf("2-end\n");
}

void CLoopHandle::Accept(int iFD,CSocket *pSocketListen,char* strIP){
    //auto *pNewSocket = new CSocket(-1,pSocketListen->GetProtocol(),iFD);
    CSocket *pNewSocket = MallocSocket(iFD,pSocketListen);
    if (pNewSocket == nullptr){
        close(iFD);
        syslog(LOG_WARNING ,"CPoolMemory:%s", strerror(errno));
        return;
    }
    pNewSocket->Alive();
    //strcat(pNewSocket->GetIP(),"0");
    pNewSocket->SetIP(strIP);
    //pNewSocket->m_FunctionAddress = std::bind(&CSocket::ReceiveTCP, pNewSocket);
    pNewSocket->m_FunctionAddress = [pNewSocket]{pNewSocket->ReceiveTCP();};
    AddEvent(pNewSocket,EPOLLIN | EPOLLET|EPOLLONESHOT);
}

void CLoopHandle::PollRead(CSocket *psocket) {
    ModifyEvent(psocket,EPOLLIN | EPOLLET|EPOLLONESHOT);
}

void CLoopHandle::PollWrite(CSocket *psocket) {
    ModifyEvent(psocket,EPOLLOUT | EPOLLET|EPOLLONESHOT);
}

void CLoopHandle::loop(CSocket *pSocketListen,CPoolThread &ThreadsPool){
    m_iPollfd = epoll_create1 (0);
    if (m_iPollfd == -1){
        syslog(LOG_ERR ,"epoll_create:%s", strerror(errno));
        abort ();
    }
    //pSocketListen->m_FunctionAddress = std::bind(&CSocket::AcceptListen, pSocketListen);
    //pSocketListen->m_FunctionAddress = [pSocketListen]{pSocketListen->AcceptListen();};
    AddEvent(pSocketListen,EPOLLIN | EPOLLET);
    //Listen socket don't need read and write callback function
    //m_pSocketListen->m_CallbackRead = std::bind(&CLoopHandle::callbackRead, this,std::placeholders::_1);
    //pSocketListen->m_CallbackWrite = std::bind(&CLoopHandle::PollWrite, this,std::placeholders::_1);
    time_t  seconds = time(NULL);
    int nn = 0;
    while(true){
        int n = epoll_wait (m_iPollfd, &*m_vecEvent.begin(), m_vecEvent.capacity(), -1);
        if( n<0&&errno!=EINTR)
            syslog(LOG_WARNING ,"epoll_wait:%s", strerror(errno));
        //std::cout<<"epoll_wait no:"<<n <<"all-"<< nn+n<<std::endl;
       for (int i = 0; i < n; ++i) {
            auto *pSocket = reinterpret_cast<CSocket *>(m_vecEvent[i].data.ptr);
            if ((m_vecEvent[i].events & EPOLLERR) ||
                (m_vecEvent[i].events & EPOLLHUP)/*||(!(m_vecEvent[i].events & EPOLLIN))*/) {
                /* An error has occured on this fd, or the socket is not
                   ready for reading (why were we notified then?) */
                std::cout << "socket error!"<<pSocket << std ::endl;
                //printf("socket error!\n",pSocket);
                //RemoveSocket(pSocket);
                //FreeSocket(pSocket);
                FreeSocketInlist(pSocket);
                continue;
            }
            //thread pool or process pool
            //reinterpret_cast<CSocket*>(m_vecEvent[i].data.ptr)->m_FunctionAddress();
            bool bListen = pSocketListen->GetSocketFd() == pSocket->GetSocketFd();
            if (bListen)
                ThreadsPool.enqueue(bListen, [pSocket] { pSocket->m_FunctionAddress(); });
            else{
                //strcat(pSocket->GetIP(),"1");
                RemoveSocket(pSocket);
                ThreadsPool.enqueue(bListen,[pSocket,this]{pSocket->m_FunctionAddress();
//                    if(pSocket->GetPort()!=0)
//                        this->ActiveSocket(pSocket);
//                    else
//                        this->FreeSocketInlist(pSocket);
//                    if(pSocket->GetPort()==0)
//                        this->FreeSocket(pSocket);
//                    else     //this code cadded maybe after epool event;
//                        this->AddSocket(pSocket);
                });
            }
        }
        //free not alive socket
        if(difftime(seconds,time(NULL))>300){
            seconds = time(NULL);
            ClearConnect();
        }
        if (m_vecEvent.size() == n) {
            m_vecEvent.clear();
            m_vecEvent.resize(0);
            m_vecEvent.resize(n * 2);
        }
    }
}