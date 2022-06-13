//
// Created by admin on 3/8/22.
//

#include "CServiceRegUser.h"
#include "CSocket.h"

REGISTER(CServiceRegUser)

IMPLEMENT_CLASS(register,CServiceRegUser)

CServiceRegUser::CServiceRegUser() {}

CServiceRegUser::~CServiceRegUser() {

}

/*{"ServiceType":"注册/登录/登出/修改密码/上传/下载",
"name":"",
"password":"",
"PhoneNo":"",
"email":"",
}*/
//CREATE DEFINER=`admin`@`%` PROCEDURE `regester`(IN `NAME` varchar(50), IN `PASSWORD` varchar(50),  IN PHONE bigint, IN EMAIL varchar(50),
//IN LASTIP varchar(50))
void CServiceRegUser::HandleSend(CSocket *pSocket) const {
    //pSocket->SetStateWriteTCP();
}

int CServiceRegUser::HandleMsg(CSocket *pSocket, boost::json::object &decode_val, const CService *pServiceManage) const {
    std::string strName = decode_val["name"].as_string().c_str();
    std::string strPassword = decode_val["password"].as_string().c_str();
    std::string strPhoneNo = decode_val["PhoneNo"].as_string().c_str();
    std::string strEmail = decode_val["Email"].as_string().c_str();
    std::string strSQL = "CALL regester('"+strName+"','"+strPassword+"',"+ strPhoneNo+",'"+ strEmail+"','"+pSocket->GetIP()+"');";

    auto funSQL = [strName,pSocket,pServiceManage,strPassword,this](char* pStr){
        std::string strJson(pStr);
        if(strJson=="{\"field0\":\"1\"}")//
            strcpy(pSocket->GetDataBuf(),"{\"Login\":0,\"Role\":\"ok\"}");
        else
            strcpy(pSocket->GetDataBuf(),"{\"Login\":-7,\"Role\":\"name exist!\"}");
        pServiceManage->HandleSend(pSocket);
    };
    m_pSQL->AddQuery(strSQL,funSQL);
    return 0;
}


