//
// Created by admin on 3/1/22.
//

#ifndef HWZ_SERVERDFC_CSQL_H
#define HWZ_SERVERDFC_CSQL_H
#include <string>
#include <functional>
#include <atomic>

class CSql {
public:
    CSql();
    virtual ~CSql();
    virtual bool Init() = 0;
    virtual void AddQuery(const std::string& strCommand,std::function<void(char*)> &&callback) =0;
    bool GetStop(){
        return m_bStop;
    }
    virtual void Stop(){
        m_bStop = true;
    }

protected:
    std::atomic<bool> m_bStop;

};


#endif //HWZ_SERVERDFC_CSQL_H
