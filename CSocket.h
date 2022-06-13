//
// Created by hwz on 9/26/21.
//

#ifndef HWZ_SERVERDFC_CSocket_H
#define HWZ_SERVERDFC_CSocket_H
/*
 * TCP or UDP socket
 * handle IO
 * */
#include <sys/socket.h>
#include <netinet/in.h>
#include "Base.h"
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <functional>
#include <mutex>
#include <iostream>
#include "CServiceManager.h"
#include "CLoopHandle.h"

#define  SOCKET_IP  16
#define  SOCKET_BUF  512


class CSocket {//may be memory pool
public:
    CSocket(int iPort,CServiceManager* pServiceManager,int iFd=-1);
    virtual ~CSocket();

    virtual void CreateTCP();
    virtual void CreateUDP();

    void Receive() ;
    void Send() ;
    int NonBlock(int sfd) ;
    int GetSocketFd(){
        return m_socketFD;
    }
    void SetSocketFd(int iFD){
        m_socketFD = iFD;
    }
    char *GetDataBuf(){
        return m_strBuf;
    }
    char* GetIP(){
        return m_strIP;
    }
    void SetIP(char *strIP){
        strncpy(m_strIP,strIP,SOCKET_IP);
    }

    CServiceManager* GetService(){
        return m_pServiceManager;
    }
    void  SetService(CServiceManager* pServiceManager){
        m_pServiceManager = pServiceManager;
    }

    clock_t GetTime(){
        return m_time;
    }
    void Alive();
    void CloseSocket();

    void AcceptListen();
    void ReceiveTCP() ;
    void SendTCP() ;
    void ReceiveUDP() ;
    void SendUDP() ;

    void SetPriority(int iPriority){
        m_iPriority = iPriority;
    }
    int GetPriority(){
        return m_iPriority;
    }
    void SetBufOff(int iOff){
        m_iBufOffset = iOff;
    }
    int GetBufOff(){
        return m_iBufOffset;
    }
    void SetStateReadTCP();
    void SetStateWriteTCP(int iSend);
    void SetStateWrite();
    void SetStateRead();

    void SetStateReadUDP();
    void SetStateWriteUDP();
    void SetLoop(CLoopHandle *pPoll){
        m_pPoll = pPoll;
    }
    CLoopHandle* GetLoop(){
        return m_pPoll;
    }
    std::function<void(CSocket*)> m_CallbackWrite;
    std::function<void()> m_FunctionAddress;//socket state : read or write or accept
    CSocket* GetPrev() const {
        return m_pPrev;
    }
    CSocket* GetNext() const {
        return m_pNext;
    }
    void SetNext(CSocket *pNext){
        m_pNext = pNext;
    }
    void SetPrev(CSocket *pPre){
        m_pPrev = pPre;
    }

    void SetSendDataSize(int iSize){
        m_iSendAll = iSize;
    }
    int GetSendDataSize(){
        return m_iSendAll;
    }

    int GetPort(){
        return m_iPort;
    }

    void SetPort(int iPort){
        m_iPort = iPort;
    }

    void SetFileFD(int iFD){
        m_iFileFD = iFD;
    }

    int GetFileFD(){
        return m_iFileFD;
    }

    void FileInit(const std::string &strFile,int iSize,char* pMode);

    void SendFile();
    void ReceiveFile();
    void ReceiveFileAio();
    std::string get_file_name (const int fd);
    void SendFileAio();

    off_t GetFileOff(){
        return m_iFileOffset;
    }
    void SetFileOff(off_t iFileOffset){
        m_iFileOffset = iFileOffset;
    }

    off_t GetFileSize(){
        return m_iFileSize;
    }
    void SetFileSize(off_t iFileOffset){
        m_iFileSize = iFileOffset;
    }

    FILE* GetFile(){
        return m_pFile;
    }

    void CloseFile(){
        fclose(m_pFile);
        m_pFile = nullptr;
    }

protected:
    CSocket();
    bool CheckPort() ;
    CLoopHandle *m_pPoll;
    CServiceManager  *m_pServiceManager;
    int m_iPriority;//
    int m_iPort;
    int m_socketFD;
    int m_iFileFD;
    off_t m_iFileOffset;
    off_t m_iFileSize;
    char m_strIP[SOCKET_IP];//
    char m_strBuf[SOCKET_BUF];//
    off_t m_iBufOffset;
    off_t m_iSendAll;//A terminating character appears after encryption
    CSocket *m_pNext;
    CSocket *m_pPrev;
    time_t m_time;
    FILE *m_pFile;
    //std::mutex m_mutex;//不用加锁，EPOLLONESHOT去除epoll监听.处理两个线程同时操作一个socket的局面
};

#endif //HWZ_SERVERDFC_CSERVER_H
