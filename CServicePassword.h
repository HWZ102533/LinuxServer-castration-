//
// Created by admin on 3/8/22.
//

#ifndef HWZ_SERVERDFC_CSERVICEPASSWORD_H
#define HWZ_SERVERDFC_CSERVICEPASSWORD_H


#include "CService.h"

class CServicePassword : public CService {
public:
    CServicePassword();

    virtual ~CServicePassword();

    int HandleMsg(CSocket *pSocket, boost::json::object &decode_val, const CService *pService) const override;

    void HandleSend(CSocket *pSocket) const override;
};


#endif //HWZ_SERVERDFC_CSERVICEPASSWORD_H
