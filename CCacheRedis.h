//
// Created by hwz on 12/14/21.
//

#ifndef HWZ_SERVERDFC_CCACHEREDIS_H
#define HWZ_SERVERDFC_CCACHEREDIS_H
#include "CCache.h"
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>
#include <signal.h>
#include <queue>
#include <mutex>
#include <condition_variable>

class CCacheRedis : public CCache{
public:
    CCacheRedis(std::string& strIP,int iPort);

    virtual ~CCacheRedis();
    virtual bool Init() ;
    //virtual std::string GetValue(const std::string& str,std::function<void(char*)> &callback) ;
    virtual void GetValueAsync(const std::string& strCommand,std::function<void(char*)> &&callback) ;
    virtual void SetValueAsync(const std::string& strCommand,std::function<void(char*)> &&callback) ;
    virtual void Stop();
private:
    bool DisConnect() ;
    std::queue<std::function<void(char*)>> m_funcList;//prevent memory leaked
    std::queue<std::string> m_strCommand;//prevent memory leaked
    CCacheRedis();
    static void GetCallback(redisAsyncContext *c, void *r, void *privdata);
//    void connectCallback(const redisAsyncContext *c, int status);
//    void disconnectCallback(const redisAsyncContext *c, int status);
//    void getCallback(redisAsyncContext *c, void *r, void *privdata);
    std::string m_strIP;
    int m_iPort;
    pthread_t m_tid_reader;
    std::mutex m_queue_mutex;
    std::condition_variable m_condition;
    //std::mutex m_Redis_mutex;
    //std::shared_mutex m_Redis_mutex;
    redisAsyncContext* m_pRedisContext;
    struct event_base *m_baseloop;
    //uv_async_s m_async_watch;
    //uv_loop_s* m_uvloop;
};


#endif //HWZ_SERVERDFC_CCACHEREDIS_H
