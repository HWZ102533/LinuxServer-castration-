//
// Created by hwz on 10/9/21.
//

#ifndef HWZ_SERVERDFC_CSERVICEMANAGER_H
#define HWZ_SERVERDFC_CSERVICEMANAGER_H
#include "CService.h"
#include "CProtocol.h"

class CServiceManager :public CService{
public:
    CServiceManager() =delete;
    CServiceManager(CCache *pCache,CPoolThread* pThreadPool,CProtocol* pProtocol,CSql* m_pSQL);
    ~CServiceManager();
    //virtual int SendComplete(CSocket* pSocket) const;
    virtual int HandleMsg(CSocket* pSocket,boost::json::object &decode_val,const CService* pService) const;
    virtual int HandleRev(char* pDataSocket,int iOffSize,CSocket *pSocketTmp) const;
    virtual void HandleSend(CSocket* pSocket) const;
    void SetCache(CCache *pCache){
        m_pCache = pCache;
    }
    void SetThreadPool(CPoolThread* pThreadPool){
        m_pThreadPool = pThreadPool;
    }
    void SetProtocol(CProtocol* pProtocol){
        m_pProtocol = pProtocol;
    }
private:
    CProtocol*  m_pProtocol;
//    int Register(boost::json::object &obj) const;
};


#endif //HWZ_SERVERDFC_CSERVICEMANAGER_H
