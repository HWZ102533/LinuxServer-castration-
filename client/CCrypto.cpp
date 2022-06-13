//
// Created by hwz on 11/24/21.
//

#include <cstring>
#include "CCrypto.h"
CCrypto::CCrypto(){

}

CCrypto::~CCrypto(){

}

 void CCrypto::Encrypt(const char *pStrIn,char *ptrOut){
     strcpy(ptrOut,pStrIn);
}
 void CCrypto::Decrypt(const char *pStrIn,char *ptrOut,int iLength){
     strncpy(ptrOut,pStrIn,iLength);
 }