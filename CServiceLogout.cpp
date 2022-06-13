//
// Created by admin on 3/8/22.
//

#include "CServiceLogout.h"
#include "CSocket.h"
REGISTER(CServiceLogout)

IMPLEMENT_CLASS(logout,CServiceLogout)

CServiceLogout::CServiceLogout() {}

CServiceLogout::~CServiceLogout() {

}

int CServiceLogout::HandleMsg(CSocket *pSocket, boost::json::object &decode_val, const CService *pService) const {

    return 0;
}

void CServiceLogout::HandleSend(CSocket *pSocket) const {
}
