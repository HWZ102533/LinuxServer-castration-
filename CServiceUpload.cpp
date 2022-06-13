//
// Created by admin on 3/8/22.
//

#include "CServiceUpload.h"
#include "CSocket.h"
REGISTER(CServiceUpload)

IMPLEMENT_CLASS(upload,CServiceUpload)

CServiceUpload::CServiceUpload() {}

CServiceUpload::~CServiceUpload() {

}

//name: total:
int CServiceUpload::HandleMsg(CSocket *pSocket, boost::json::object &decode_val, const CService *pService) const {
    std::string strName = decode_val["name"].as_string().c_str();
    long lTotal = atol(decode_val["total"].as_string().c_str());
//    long lCurrentIndex = atol(decode_val["CurrentIndex"].as_string().c_str());
//    long lLength = atol(decode_val["length"].as_string().c_str());
//    boost::json::string strData = decode_val["data"].as_string();

    off_t iSize = ceilf(lTotal/SOCKET_BUF)*SOCKET_BUF;
    pSocket->SetPort(1);
    //////////////////////libaio///////////////////////////////////////////////////
    pSocket->FileInit(strName,lTotal,"wb");
    ftruncate(pSocket->GetFileFD(), iSize);
    pSocket->m_CallbackWrite = [lTotal](CSocket* pS){
        pS->SetBufOff(0);
        pS->SetSendDataSize(lTotal);
        pS->m_FunctionAddress = [pS]{pS->ReceiveFileAio();};
        pS->SetStateRead();
    };
    /////////////////////////////////////////////////////////////////////////
//    pSocket->FileInit(strName,lTotal,"wb");
//    pSocket->m_CallbackWrite = [lTotal](CSocket* pS){
//        pS->SetBufOff(0);
//        pS->SetSendDataSize(lTotal);
//        pS->m_FunctionAddress = [pS]{pS->ReceiveFile();};
//        pS->SetStateRead();
//    };
    //////////////////////////////////////////////////////////////////////////

    sprintf(pSocket->GetDataBuf(),"{\"fileName\":\"OK\"}");

    return -1;
}

void CServiceUpload::HandleSend(CSocket *pSocket) const {

}
