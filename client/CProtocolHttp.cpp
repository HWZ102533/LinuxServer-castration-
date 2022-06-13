//
// Created by hwz on 9/26/21.
//

#include "CProtocolHttp.h"
#include <string.h>
#include <cstdio>

#define SERVER_STRING "Server: hwzhttpd/0.1.0\r\n"

CProtocolHttp::CProtocolHttp(CCrypto* pCrypto): CProtocol(pCrypto){
}

CProtocolHttp::~CProtocolHttp(){

}
//仅支持post
int CProtocolHttp::HandleRev(char* pDataSocket,int iBufOffset,char* pStrOut){
    m_pCrypto->Decrypt(pDataSocket,pStrOut,iBufOffset);
    //pDataSocket[iBufOffset] = '\0';
    pStrOut[iBufOffset]= '\0';
    char* iPosF = strstr(pStrOut,"\r\n\r\n");
    char* iPosL = nullptr;
    char* iPosNext = nullptr;
    if(iPosF!=NULL){
        iPosNext = iPosF+4;
        iPosL = strstr(iPosNext,"\r\n\r\n");
    }

    if((iPosF&&iPosL)){
        iPosL[0] = '\0';
        //strcpy(pStrOut,iPosNext);//there is some problems
        //return iPosL - pStrOut+4;
        return iPosF - pStrOut+4;
    }
    return -1;
}

int CProtocolHttp::HandleSend( char* pDataSocket,int iBufMaxSize){
    char pSend[iBufMaxSize];
    sprintf(pSend, "HTTP/1.1 200 OK\r\nContent-Length:%d\r\n\r\n%s\r\n\r\n", strlen(pDataSocket),pDataSocket);
    //printf("%s\n", pSend);
    int iLen = strlen(pSend);
    iLen = iLen==iBufMaxSize?iLen-1:iLen;
    m_pCrypto->Encrypt(pSend,pDataSocket);
    pDataSocket[iLen] = '\0';
    return iLen;
}

void CProtocolHttp::headers(int client, const char *filename)
{
//    char buf[1024];
//    (void)filename;  /* could use filename to determine file type */
//    strcpy(buf, "HTTP/1.0 200 OK\r\n");
//    send(client, buf, strlen(buf), 0);
//    strcpy(buf, SERVER_STRING);
//    send(client, buf, strlen(buf), 0);
//    sprintf(buf, "Content-Type: text/html\r\n");
//    send(client, buf, strlen(buf), 0);
//    strcpy(buf, "\r\n");
//    send(client, buf, strlen(buf), 0);
}