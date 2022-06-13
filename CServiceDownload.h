//
// Created by admin on 3/8/22.
//

#ifndef HWZ_SERVERDFC_CSERVICEDOWNLOAD_H
#define HWZ_SERVERDFC_CSERVICEDOWNLOAD_H


#include "CService.h"

class CServiceDownload : public CService {
public:
    CServiceDownload();

    virtual ~CServiceDownload();

    int HandleMsg(CSocket *pSocket, boost::json::object &decode_val, const CService *pService) const override;

    void HandleSend(CSocket *pSocket) const override;

private:
    void GetFileInDirectory(const std::string &strDir,std::string &strOutFiles) const;
    void GetFileInDirectoryRecursion(const std::string &strDir,std::string &strOutFiles) const;

    int GetFileSize(std::string &strFile) const;
};


#endif //HWZ_SERVERDFC_CSERVICEDOWNLOAD_H
