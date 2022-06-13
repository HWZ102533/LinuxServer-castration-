//
// Created by hwz on 11/24/21.
//

#include "CCryptoAES.h"
#include <cstring>

CCryptoAES::CCryptoAES():m_key(){
    AES_set_encrypt_key((const unsigned char*)"a0d8hjyu6vn,op49", 128, &m_key);
}

CCryptoAES::~CCryptoAES(){

}

void CCryptoAES::SetKey(char * ptr16){
    AES_set_encrypt_key((const unsigned char*)ptr16, 128, &m_key);
}

void CCryptoAES::Encrypt(const char *pStrIn,char *ptrOut){
    unsigned char ivec[AES_BLOCK_SIZE] = {0x7e, 0xcf, 0xd5, 0x6f, 0x1b, 0xad, 0xf3, 0xa1, 0xc9, 0xab, 0x27, 0xc7,
                                          0x5f, 0x2d, 0xbd, 0xe7};
    unsigned char ecount[AES_BLOCK_SIZE];
    unsigned int iNum = 0;
    auto* pIn = reinterpret_cast<const unsigned char*>(pStrIn);
    auto* pOut = reinterpret_cast<unsigned char*>(ptrOut);
    //int iLength = strlen(pStrIn);
    CRYPTO_ctr128_encrypt(pIn, pOut, strlen(pStrIn), &m_key, ivec, ecount, &iNum, (block128_f)AES_encrypt);
}

 void CCryptoAES::Decrypt(const char *pStrIn,char *ptrOut,int iLength){
     unsigned char ivec[AES_BLOCK_SIZE] = {0x7e, 0xcf, 0xd5, 0x6f, 0x1b, 0xad, 0xf3, 0xa1, 0xc9, 0xab, 0x27, 0xc7,
                                           0x5f, 0x2d, 0xbd, 0xe7};
     unsigned char ecount[AES_BLOCK_SIZE];
     unsigned int iNum = 0;
     auto* pIn = reinterpret_cast<const unsigned char*>(pStrIn);
     auto* pOut = reinterpret_cast<unsigned char*>(ptrOut);
     CRYPTO_ctr128_encrypt(pIn, pOut, iLength, &m_key, ivec, ecount, &iNum, (block128_f)AES_encrypt);
}