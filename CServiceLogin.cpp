//
// Created by hwz on 1/17/22.
//

#include "CServiceLogin.h"
#include "CSocket.h"

REGISTER(CServiceLogin)

IMPLEMENT_CLASS(login,CServiceLogin)

CServiceLogin::CServiceLogin() {}

CServiceLogin::~CServiceLogin(){}

void CServiceLogin::HandleSend(CSocket *pSocket) const{
    //pSocket->SetStateWriteTCP();
}

//redis : key  value
//        name {password,}

bool CServiceLogin::CheckValidTime(std::string &strValidTime) const{
    time_t timep;
    struct tm *p;
    time(&timep);
    p = localtime(&timep);
    char strTime[12];
    sprintf(strTime, "%d-%d-%d", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday);
    return strValidTime.compare(strTime)<0;
}

int CServiceLogin::HandleMsg(CSocket* pSocket,boost::json::object &decode_valv,const CService* pServiceManage) const {
    std::string strName = decode_valv["name"].as_string().c_str();
    std::string strPassword = decode_valv["password"].as_string().c_str();
    std::string strApp = decode_valv["App"].as_string().c_str();
    std::string strCommand("MGET ");//HGETALL
    strCommand = strCommand + strApp+" "+strName;

    //strcpy(pSocket->GetDataBuf(),"{\"Login\":-5,\"Role\":Redis not found!!}");
    auto funSQL = [strName,pSocket,pServiceManage,strPassword,this](char* pStr){
        std::string strJson(pStr);
        if(strJson.find("nil")!=-1){
            strcpy(pSocket->GetDataBuf(),"{\"Login\":-6,\"Role\":\"user or password error\"}");
            pServiceManage->HandleSend(pSocket);
            return;
        }
        strJson.replace(strJson.find("field0"),6,"password");
        strJson.replace(strJson.find("field1"),6,"role");
        strJson.replace(strJson.find("field2"),6,"validTime");
        //strJson.replace(strJson.find("field3"),6,"version");
        pServiceManage->m_pThreadPool->enqueue(false,[strName,pSocket,strJson,strPassword,pServiceManage,this] {
            boost::json::object redisJson = boost::json::parse(strJson.c_str()).as_object();
            std::string strPasswordRedis = redisJson["password"].as_string().c_str();
            std::string strValidTimeRedis = redisJson["validTime"].as_string().c_str();
            pSocket->SetPriority(strPasswordRedis==strPassword);
            if (strPasswordRedis==strPassword) {//check mac address valid time
                if(this->CheckValidTime(strValidTimeRedis)) {
                    //set redis
                    redisJson["socketAddress"] = std::to_string(long(pSocket));
                    strPasswordRedis = "SET "+strName+" "+serialize(redisJson);
                    this->m_pCache->SetValueAsync(strPasswordRedis,[](char *pstr){;});
                    char strTime[50];
                    sprintf(strTime, "{\"Login\":1,\"Role\":\"%s\",\"Version\":\"%s\"}", redisJson["role"].as_string().c_str(), redisJson["field3"].as_string().c_str());
                    strcpy(pSocket->GetDataBuf(),strTime);
                }else
                    strcpy(pSocket->GetDataBuf(),"{\"Login\":-7,\"Role\":\"out of valid time\"}");

            }else//user or password error~
                strcpy(pSocket->GetDataBuf(),"{\"Login\":-6,\"Role\":\"user or password error\"}");

            pServiceManage->HandleSend(pSocket);
        });

    };

    auto func = [strName,funSQL,pSocket,pServiceManage,strPassword,strApp,this](char* pStr) {
        bool bFlag = false;
        std::string strJson;
        int iIndex = -1;
        if(pStr== nullptr)
            bFlag = true;
        else{
            strJson.append(pStr);
            iIndex = strJson.find("{");
        }
        if(iIndex==-1||bFlag){
            //std::string strSQL = "SELECT password,role,validTime FROM user where user='"+strName+"' and password='"+strPassword+"'";
            std::string strSQL = "CALL login('"+strName+"','"+strPassword+"','"+ strApp.c_str()+"','"+ pSocket->GetIP()+"');";
            pServiceManage->m_pSQL->AddQuery(strSQL,funSQL);
           return;
        }
        std::string strVersion = strJson.substr(0,iIndex-1);
        strJson.erase(0,iIndex);
        pServiceManage->m_pThreadPool->enqueue(false,[strName,pSocket,strJson,strPassword,strVersion,pServiceManage,this] {
            boost::json::object redisJson = boost::json::parse(strJson.c_str()).as_object();
            std::string strPasswordRedis = redisJson["password"].as_string().c_str();
            std::string strValidTimeRedis = redisJson["validTime"].as_string().c_str();
            pSocket->SetPriority(strPasswordRedis==strPassword);
            if (strPasswordRedis==strPassword) {//check mac address valid time
                if(this->CheckValidTime(strValidTimeRedis)) {
                    //set redis
                    redisJson["socketAddress"] = std::to_string(long(pSocket));
                    strPasswordRedis = "SET "+strName+" "+serialize(redisJson);
                    this->m_pCache->SetValueAsync(strPasswordRedis,[](char *pstr){;});
                    char strTime[50];
                    sprintf(strTime, "{\"Login\":1,\"Role\":\"%s\",\"Version\":\"%s\"}", redisJson["role"].as_string().c_str(),strVersion.c_str());
                    strcpy(pSocket->GetDataBuf(),strTime);
                }else
                    strcpy(pSocket->GetDataBuf(),"{\"Login\":-7,\"Role\":\"out of valid time\"}");
            }else//user or password error~
                strcpy(pSocket->GetDataBuf(),"{\"Login\":-6,\"Role\":\"user or password error\"}");

            pServiceManage->HandleSend(pSocket);
        });
    };

    if(m_pCache->GetStop()){//connect SQL
        std::string strSQL = "CALL login('"+strName+"','"+strPassword+"','"+ pSocket->GetIP()+"');";
        m_pSQL->AddQuery(strSQL,funSQL);
    }else
        m_pCache->GetValueAsync(strCommand,func);
    return 0;
}

//int CServiceLogin::SendComplete(CSocket* pSocket) const {
//    pSocket->SetStateReadTCP();
//    return 0;
//}

