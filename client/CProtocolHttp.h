//
// Created by hwz on 9/26/21.
//

#ifndef HWZ_SERVERDFC_CPROTOCOLHTTP_H
#define HWZ_SERVERDFC_CPROTOCOLHTTP_H

#include "CProtocol.h"

class CProtocolHttp : public CProtocol{
public:
    CProtocolHttp(CCrypto* pCrypto);
    virtual ~CProtocolHttp();
    virtual int HandleRev(char* pDataSocket,int iBufOffset,char* pStrOut);
    virtual int HandleSend(char* pDataSocket,int iBufMaxSize);
private:
    void headers(int iClient,const char *filename);
};


#endif //HWZ_SERVERDFC_CPROTOCOLHTTP_H
