//
// Created by admin on 3/8/22.
//

#include "CServiceDownload.h"
#include "CSocket.h"
#include <dirent.h>

REGISTER(CServiceDownload)

IMPLEMENT_CLASS(download,CServiceDownload)

CServiceDownload::CServiceDownload() {}

CServiceDownload::~CServiceDownload() {

}

void CServiceDownload::GetFileInDirectoryRecursion(const std::string &strDir,std::string &strOutFiles) const{
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(strDir.c_str())) != NULL){
        while ((dirp = readdir(dp)) != NULL){
            if(dirp->d_type==DT_DIR){
                if(strcmp(dirp->d_name,".")==0||strcmp(dirp->d_name,"..")==0)
                    continue;
                std::string strTemp = strDir + dirp->d_name + "/";
                GetFileInDirectoryRecursion(strTemp,strOutFiles);
            }
//            dirp->d_type : DT_DIR DT_FIFO BLK REG LNK SOCK
            strOutFiles += dirp->d_name;
            strOutFiles += "\n";
        }
        closedir(dp);
    }
}


void CServiceDownload::GetFileInDirectory(const std::string &strDir,std::string &strOutFiles) const{
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(strDir.c_str())) != NULL){
        while ((dirp = readdir(dp)) != NULL){
            if(dirp->d_type==DT_DIR)
                continue;
            strOutFiles += dirp->d_name;
            std::string strSize = strDir+"/"+dirp->d_name;
//            dirp->d_type : DT_DIR DT_FIFO BLK REG LNK SOCK
            strOutFiles += " " + std::to_string(GetFileSize(strSize));
            strOutFiles += "\n";
        }
        closedir(dp);
    }
}

int CServiceDownload::GetFileSize(std::string &strFile) const{
    struct stat statbuff;
    if(-1!=stat(strFile.c_str(), &statbuff))
        return statbuff.st_size;
    return 0;
}

int CServiceDownload::HandleMsg(CSocket *pSocket, boost::json::object &decode_val, const CService *pServiceManage) const {
    std::string strName = decode_val["appName"].as_string().c_str();
    std::string strVersion = decode_val["version"].as_string().c_str();
    std::string strSize = decode_val["size"].as_string().c_str();
    //strcpy(pSocket->GetDataBuf(),"{\"filename\":-6,\"size\":\"or\"}");
    if(strSize=="list"){//send file name list
        std::string strFileList;
        strName +="/";
        GetFileInDirectory(strName,strFileList);
        sprintf(pSocket->GetDataBuf(),"{\"fileList\":\"%s\"}",strFileList.c_str());
        //pServiceManage->HandleSend(pSocket);
        return -1;
    }else{//send file
        int iSize = std::atoi(strSize.c_str());
        pSocket->FileInit(strName,iSize,"rb");//"wb" O_RDONLY
        if(iSize<512000){
            pSocket->m_FunctionAddress = [pSocket]{pSocket->SendFile();};
            pSocket->SendFile();
        }else{
            pSocket->SetPort(0);//set read file flag
            //pS->m_FunctionAddress = [pS]{pS->SendFileAio();};
            pSocket->m_FunctionAddress = [pSocket]{pSocket->SendTCP();};
            pSocket->m_CallbackWrite = [](CSocket* pS){pS->SendFileAio();};
            pSocket->SendFileAio();
        }
    }
    return 0;
}

void CServiceDownload::HandleSend(CSocket *pSocket) const {

}
