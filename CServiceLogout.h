//
// Created by admin on 3/8/22.
//

#ifndef HWZ_SERVERDFC_CSERVICELOGOUT_H
#define HWZ_SERVERDFC_CSERVICELOGOUT_H

#include "CService.h"

class CServiceLogout: public CService  {

public:
    CServiceLogout();

    virtual ~CServiceLogout();

    int HandleMsg(CSocket *pSocket, boost::json::object &decode_val, const CService *pService) const override;

    void HandleSend(CSocket *pSocket) const override;
};


#endif //HWZ_SERVERDFC_CSERVICELOGOUT_H
