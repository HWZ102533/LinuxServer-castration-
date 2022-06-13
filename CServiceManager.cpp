//
// Created by hwz on 10/9/21.
//

#include "CServiceManager.h"
#include "CSocket.h"

CServiceManager::CServiceManager(CCache *pCache,CPoolThread* pThreadPool,CProtocol* pProtocol,CSql* pSQL):m_pProtocol(pProtocol){
    m_pCache = pCache;
    m_pThreadPool = pThreadPool;
    m_pSQL = pSQL;
}

CServiceManager::~CServiceManager(){

}
//int CServiceManager::Register(boost::json::object &obj) const{
//    std::string strVersionOut("20211014");
//    std::string strIPout("192.168.0.1");
//    obj.clear();
//    obj["Login"] = 1;
//    obj["Ver"] = strVersionOut;
//    obj["IP"] = strIPout;
//    obj["Error"] = "已存在相同用户名";
//    return 1;
//}

/*{"ServiceType":"注册/登录/登出/修改密码/上传/下载",
"name":"",
"password":"",
"NewPassword":"",
"PhoneNo":"",
"email":"",
"version":"",
"lastIP":"",
}*/
/*
 //user table//
 * name
 * password
 * role
 * validTime
 * PhoneNo
 * email
 * lastIP
 * lastTime
 * note
 * //version table
 * appName
 * version
 * updateTime
 * */

/*return 0 or 1*/
//int CServiceManager::Login(boost::json::object &obj) const{
//    std::string strVersionOut("20211014");
//    std::string strIPout("192.168.0.1");
//    obj.clear();
//    obj["Login"] = 1;
//    obj["Ver"] = strVersionOut;
//    obj["IP"] = strIPout;
//    return 1;
//}

//int CServiceManager::SendComplete(CSocket* pSocket) const {
//    pSocket->SetStateReadTCP();
//    return 0;
//}

void CServiceManager::HandleSend(CSocket *pSocket) const{
    int iSend = m_pProtocol->HandleSend(pSocket->GetDataBuf(),SOCKET_BUF);
    pSocket->SetStateWriteTCP(iSend);
}

int CServiceManager::HandleRev(char* pDataSocket,int iOffSize,CSocket *pSocketTmp) const{
    //pSocketTmp->m_CallbackWrite = [this](CSocket* pS){this->SendComplete(pS);};
    std::unique_ptr<char> pData(new char[iOffSize+1]);
    int iOff = m_pProtocol->HandleRev(pDataSocket,iOffSize,pData.get());
    if(iOff==-1){
        if(strlen(pDataSocket)==SOCKET_BUF-1){//request size must less than SOCKET_BUF
            strcpy(pDataSocket,"{\"Login\":-5,\"Role\":\"data form error!\"}");
            HandleSend(pSocketTmp);
            return 0;
        }
        return -1;//continue read socket
    }

    pSocketTmp->m_CallbackWrite = [](CSocket* pS){pS->SetStateReadTCP();};
    strcpy(pDataSocket,"{\"Login\":1,\"Role\":\"1\",\"Version\":\"v1.0.01\"}");
    HandleSend(pSocketTmp);
    return 0;
    //redis object struct key:name value: password socketAddress validTime
    char* pStr = pData.get()+iOff;
    boost::json::object decode_val = boost::json::parse(pStr).as_object();
    std::string strServiceType = decode_val["ServiceType"].as_string().c_str();
    int iPermit = pSocketTmp->GetPriority();
    if(strServiceType=="register"||strServiceType=="login")
        iPermit = 1;
    if(iPermit){
        auto Iter = CServiceRegister::m_FuncMap.find(strServiceType);
        if(Iter!=CServiceRegister::m_FuncMap.end()){
            CService *pService = static_cast<CService*>(Iter->second());
            if(pService->HandleMsg(pSocketTmp,decode_val,this)!=0)
                HandleSend(pSocketTmp);
        }else{
            strcpy(pDataSocket,"{\"Login\":-5,\"Role\":\"Service not found!\"}");
            HandleSend(pSocketTmp);
        }
    }else{
        strcpy(pDataSocket,"{\"Login\":-5,\"Role\":\"Login first!\"}");
        HandleSend(pSocketTmp);
    }
    return 0;
}

int CServiceManager::HandleMsg(CSocket* pSocket,boost::json::object &decode_val,const CService* pService) const{
    return 0;
}