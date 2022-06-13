//
// Created by hwz on 1/17/22.
//

#ifndef HWZ_SERVERDFC_CServiceLogin_H
#define HWZ_SERVERDFC_CServiceLogin_H
#include "CService.h"

class CServiceLogin : public CService{
public:
    CServiceLogin();
    virtual ~CServiceLogin();
    int HandleMsg(CSocket* pSocket, boost::json::object &decode_val,const CService* pService) const override;
    //int SendComplete(CSocket *pSocket) const override;
    void HandleSend(CSocket* pSocket) const override;
private:
    bool CheckValidTime(std::string &strValidTime) const;

};



#endif //HWZ_SERVERDFC_CServiceLogin_H
