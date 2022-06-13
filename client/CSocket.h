//
// Created by admin on 4/14/22.
//

#ifndef CLIENT_CSOCKET_H
#define CLIENT_CSOCKET_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include "CCryptoAES.h"
#include "CProtocolHttp.h"
#include "boost/json.hpp"

#define MAXN  512/* 16384 max # bytes to request from server */
#define MAXLINE 512 /* max # bytes to request from server */

class CSocket {

public:
    CSocket(const char *pIP, const char *pPort);
    virtual ~CSocket();
//    char* RegisterUser(char* name,char* password,char* phone,char* email,char* pStr1,char* pStr2,char* pStr3,char* pStr4);
//    char* Login(char* name,char* password,char* pApp,char* pVersion);
//    char* RePassword(char* password);
//    void SetAlive(int iSecond);
//    void Download(char* pApp);
//    void Upload();
    bool Connect();
    char* RequestServer(const std::string& StrJson,std::function<void(char*)> &&callback);
    void ReceiveFile(const std::string& StrJson,std::string& StrApp,std::function<void(char*)> &&callback) ;
    void SendFile(const std::string& StrJson,long iSize,std::function<void(char*)> &&callback) ;
    void Stop(){
        m_bStop = true;
    }
    bool IsStop(){
        return m_bStop;
    }
    pthread_t m_tid_reader;
private:
    CSocket();
    int NonBlock(int sfd) ;
    void ReceiveOneFile(std::string& StrJson,long iSize,std::function<void(char*)> &callback);
    void Loop();
    void AddEvent(CSocket *pSocket,int iType);
    void ModifyEvent(CSocket *pSocket,int iType);

    std::queue<std::function<void(char*)>> m_funcList;//prevent memory leaked
    std::queue<std::string> m_RequestList;//prevent memory leaked

    int m_iPollfd;
    int m_sockfd;
    int m_iState;
    char m_strBuf[MAXN];
    std::string m_strIP;
    std::string m_strPort;
    std::mutex m_queue_mutex;
    std::condition_variable m_condition;
    std::atomic<bool> m_bStop;
    CCryptoAES m_CryptoAES;
    CProtocolHttp m_protocol;
};


#endif //CLIENT_CSOCKET_H
