//
// Created by hwz on 9/26/21.
//

#ifndef HWZ_SERVERDFC_CPROTOCOL_H
#define HWZ_SERVERDFC_CPROTOCOL_H
#include "CCrypto.h"
class CProtocol {
public :
    //CProtocol(const CSocket *pSocket);
    CProtocol(CCrypto* pCrypto);
    virtual ~CProtocol();
    virtual int HandleRev(char* pDataSocket,int iBufOffset,char* pStrOut)=0;
    virtual int HandleSend(char* pDataSocket,int iBufMaxSize)=0;
    //int GetSocketFd();
protected:
    CCrypto *m_pCrypto;
};


#endif //HWZ_SERVERDFC_CPROTOCOL_H
