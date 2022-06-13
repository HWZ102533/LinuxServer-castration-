//
// Created by hwz on 11/24/21.
//

#ifndef HWZ_SERVERDFC_CCRYPTOAES_H
#define HWZ_SERVERDFC_CCRYPTOAES_H
#include "CCrypto.h"
#include <openssl/aes.h>

class CCryptoAES :public CCrypto{
public:
    CCryptoAES();
    virtual ~CCryptoAES();
    virtual void Encrypt(const char *pStrIn,char *ptrOut);
    virtual void Decrypt(const char *pStrIn,char *ptrOut,int iLength);
    void SetKey(char * ptr16);
private:
    AES_KEY m_key;
};


#endif //HWZ_SERVERDFC_CCRYPTOAES_H
