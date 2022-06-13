//
// Created by hwz on 12/14/21.
//

#ifndef HWZ_SERVERDFC_CCACHE_H
#define HWZ_SERVERDFC_CCACHE_H


#include <string>
#include <functional>
#include <atomic>

class CCache {
public:
    CCache();
    virtual ~CCache();
    virtual bool Init() = 0;
    //virtual std::string GetValue(const std::string& str,std::function<void(char*)> &&callback) = 0;
    virtual void GetValueAsync(const std::string& strCommand,std::function<void(char*)> &&callback) =0;
    virtual void SetValueAsync(const std::string& strCommand,std::function<void(char*)> &&callback) =0;
    bool GetStop(){
        return m_bStop;
    }
    virtual void Stop(){
        m_bStop = true;
    }

protected:
    std::atomic<bool> m_bStop;
};


#endif //HWZ_SERVERDFC_CCACHE_H
