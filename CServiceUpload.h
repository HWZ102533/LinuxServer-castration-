//
// Created by admin on 3/8/22.
//

#ifndef HWZ_SERVERDFC_CSERVICEUPLOAD_H
#define HWZ_SERVERDFC_CSERVICEUPLOAD_H
#include "CService.h"

class CServiceUpload:public CService {
public:
    CServiceUpload();

    int HandleMsg(CSocket *pSocket, boost::json::object &decode_val, const CService *pService) const override;

    void HandleSend(CSocket *pSocket) const override;

    virtual ~CServiceUpload();
};


#endif //HWZ_SERVERDFC_CSERVICEUPLOAD_H
