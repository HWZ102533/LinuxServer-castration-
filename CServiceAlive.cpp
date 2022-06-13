//
// Created by hwz on 1/17/22.
//

#include "CServiceAlive.h"
#include "CSocket.h"

REGISTER(CServiceAlive)

IMPLEMENT_CLASS(alive,CServiceAlive)

CServiceAlive::CServiceAlive() {}

CServiceAlive::~CServiceAlive() {

}

void CServiceAlive::HandleSend(CSocket *pSocket) const{
    //pSocket->SetStateWriteTCP();
}

int CServiceAlive::HandleMsg(CSocket* pSocket,boost::json::object &decode_valv,const CService* pServiceManage) const {
    std::string strName = decode_valv["name"].as_string().c_str();
    std::string strCommand("GET ");//HGETALL
    strCommand += strName;
    auto func = [pSocket,pServiceManage](char* pStr) {
        std::string strJson;
        if(pStr!= nullptr)
            strJson.append(pStr);

        pServiceManage->m_pThreadPool->enqueue(false,[pSocket,strJson,pServiceManage] {
            //if(strJson.find("nil")!=-1)
            if(strJson.empty())
                strcpy(pSocket->GetDataBuf(),"{\"Login\":-5,\"Role\":\"Redis not found!\"}");
            else{
                boost::json::object redisJson = boost::json::parse(strJson.c_str()).as_object();
                std::string strSocketRedis = redisJson["socketAddress"].as_string().c_str();
                if(strSocketRedis!=std::to_string(long(pSocket)))//reload
                    strcpy(pSocket->GetDataBuf(),"{\"Login\":-3,\"Role\":\"ReLogin\"}");
                else
                    strcpy(pSocket->GetDataBuf(),"{\"Login\":1}");
            }
            //pSocket>m_CallbackWrite = [this](CSocket* pS){this->SendComplete(pS);};
            pServiceManage->HandleSend(pSocket);
        });
    };
    if(m_pCache->GetStop())//connect SQL
        strcpy(pSocket->GetDataBuf(),"{\"Login\":-5,\"Role\":\"Redis Error!\"}");
    else{
        m_pCache->GetValueAsync(strCommand,func);
        return 0;
    }
    return -1;
}

//int CServiceAlive::SendComplete(CSocket* pSocket) const {
//    pSocket->SetStateReadTCP();
//    return 0;
//}

