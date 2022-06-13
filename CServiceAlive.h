//
// Created by hwz on 1/17/22.
//

#ifndef HWZ_SERVERDFC_CSERVICEALIVE_H
#define HWZ_SERVERDFC_CSERVICEALIVE_H
#include "CService.h"

class CServiceAlive : public CService{
public:
    CServiceAlive();
    virtual ~CServiceAlive();
    int HandleMsg(CSocket* pSocket, boost::json::object &decode_val,const CService* pService) const override;
    //int SendComplete(CSocket *pSocket) const override;
    void HandleSend(CSocket* pSocket) const override;
};



#endif //HWZ_SERVERDFC_CSERVICEALIVE_H
