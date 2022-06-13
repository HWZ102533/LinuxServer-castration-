//
// Created by hwz on 9/26/21.
//

#ifndef HWZ_SERVERDFC_CLOOPHANDLE_H
#define HWZ_SERVERDFC_CLOOPHANDLE_H
#include <vector>
#include "CPoolThread.h"
#include <boost/pool/pool.hpp>

class CSocket;

class CLoopHandle {
public:
    CLoopHandle(boost::pool<> *m_poolSocket);
    ~CLoopHandle();
    void loop(CSocket *pSocketListen,CPoolThread &ThreadsPool);
    void AddEvent(CSocket *pSocket,int iType);
    void DeleteEvent(CSocket *pSocket) const;
    void ModifyEvent(CSocket *pSocket,int iType) ;

    void PollRead(CSocket *psocket) ;
    void PollWrite(CSocket *psocket) ;
    void Accept(int iFD,CSocket *pSocketListen,char* strIP) ;
    void FreeSocket(CSocket* pSocket);
private:
    void AddSocket(CSocket *pSocket);
    void RemoveSocket(CSocket *pSocket);//Remove Socket from list
    void ActiveSocket(CSocket *pSocket);
    void FreeSocketInlist(CSocket* pSocket);

    void ClearConnect();

    CLoopHandle();
    CSocket *MallocSocket(int iFD,CSocket *pSocketListen);
    int m_iPollfd;
    std::vector<struct epoll_event>  m_vecEvent;
    boost::pool<> *m_poolSocket;
    CSocket *m_pFirst;
    CSocket *m_pLast;
    std::mutex m_MemoryPool_mutex;
    //std::atomic<long> m_iOnline;
};

#endif //HWZ_SERVERDFC_CLOOPHANDLE_H
