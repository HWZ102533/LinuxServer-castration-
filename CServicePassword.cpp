//
// Created by admin on 3/8/22.
//

#include "CServicePassword.h"
#include "CSocket.h"
REGISTER(CServicePassword)

IMPLEMENT_CLASS(password,CServicePassword)

CServicePassword::CServicePassword() {}

CServicePassword::~CServicePassword() {

}
//CREATE DEFINER=`admin`@`%` PROCEDURE `repassword`(IN `NAME` varchar( 50 ), IN `PASSWORD` varchar( 50 ))
int CServicePassword::HandleMsg(CSocket *pSocket, boost::json::object &decode_val, const CService *pServiceManage) const {
    std::string strName = decode_val["name"].as_string().c_str();
    std::string strNewPassword = decode_val["password"].as_string().c_str();
    std::string strCommand("GET ");//HGETALL
    strCommand += strName;
    auto func = [pSocket,pServiceManage,strName,strNewPassword](char* pStr) {
        std::string strJson;
        if(pStr!= nullptr)
            strJson.append(pStr);
        pServiceManage->m_pThreadPool->enqueue(false,[pSocket,strJson,pServiceManage,strName,strNewPassword] {
            if(strJson.empty()){
                strcpy(pSocket->GetDataBuf(),"{\"Login\":-5,\"Role\":\"Redis not found!\"}");
                pServiceManage->HandleSend(pSocket);
            }
            else{
                boost::json::object redisJson = boost::json::parse(strJson.c_str()).as_object();
                redisJson["password"] = std::string(strNewPassword);
                std::string strTemp = "SET "+strName+" "+serialize(redisJson);
                pServiceManage->m_pCache->SetValueAsync(strTemp,[](char *pstr){;});

                std::string strSQL = "CALL repassword('"+strName+"','"+strNewPassword+"');";
                m_pSQL->AddQuery(strSQL,[pSocket,pServiceManage](char *pstr){
                    std::string strData(pstr);
                    if(strData.find("nil")!=-1)
                        strcpy(pSocket->GetDataBuf(),"{\"Login\":-7,\"Role\":\"SQL Error!\"}");
                    else
                        strcpy(pSocket->GetDataBuf(),"{\"Login\":0,\"Role\":\"ok\"}");
                    pServiceManage->HandleSend(pSocket);
                });
            }
            //pSocket>m_CallbackWrite = [this](CSocket* pS){this->SendComplete(pS);};
        });
    };
    m_pCache->GetValueAsync(strCommand,func);
    return 0;
}

void CServicePassword::HandleSend(CSocket *pSocket) const {
    //pSocket->SetStateWriteTCP();
}
