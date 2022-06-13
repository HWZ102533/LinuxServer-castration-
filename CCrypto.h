//
// Created by hwz on 11/24/21.
//

#ifndef HWZ_SERVERDFC_CCRYPTO_H
#define HWZ_SERVERDFC_CCRYPTO_H

#include <openssl/modes.h>

class CCrypto {
public:
    CCrypto();
    virtual ~CCrypto();
    virtual void Encrypt(const char *pStrIn,char *ptrOut);
    virtual void Decrypt(const char *pStrIn,char *ptrOut,int iLength);
};


#endif //HWZ_SERVERDFC_CCRYPTO_H
