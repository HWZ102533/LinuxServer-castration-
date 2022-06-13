//
// Created by hwz on 10/9/21.
//

#ifndef HWZ_SERVERDFC_CSERVICE_H
#define HWZ_SERVERDFC_CSERVICE_H
#include <string>
#include "CServiceRegister.h"
#include "boost/json.hpp"
#include "CCache.h"
#include "CPoolThread.h"
#include "CSql.h"

class CSocket;
class CService {
public:
    virtual int HandleMsg(CSocket* pSocket,boost::json::object &decode_val,const CService* pService) const =0;
    //virtual int SendComplete(CSocket *pSocket) const =0 ;//0:read 1:write
    virtual void HandleSend(CSocket* pSocket) const = 0;
public:
    static CPoolThread* m_pThreadPool;
    //get data from database
    static CCache *m_pCache;
    static CSql* m_pSQL;

};


#endif //HWZ_SERVERDFC_CSERVICE_H
