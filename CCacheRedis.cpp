//
// Created by admin on 2/19/22.
//

#include <sys/syslog.h>
#include <thread>
#include "CCacheRedis.h"

CCacheRedis::CCacheRedis() {

}

CCacheRedis::~CCacheRedis() {
    //redisFree(m_pRedisContext);
    DisConnect();
}

CCacheRedis::CCacheRedis(std::string &strIP, int iPort): m_strIP(strIP), m_iPort(iPort){//
    m_bStop = false;
}

bool CCacheRedis::Init() {
    return true;
}

void CCacheRedis::Stop(){
    CCache::Stop();
    DisConnect();
    //event_base_loopexit(m_baseloop, nullptr);
}

void CCacheRedis::GetCallback(redisAsyncContext *c, void *r, void *privdata){
}

bool CCacheRedis::DisConnect(){
    return true;
}

//读写锁优先级问题，默认为读锁优先，通过pthread_rwlockattr_setkind_np设置互斥量写锁优先。
void CCacheRedis::GetValueAsync(const std::string &strCommand,std::function<void(char*)> &&callback) {
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_funcList.emplace(callback);
        m_strCommand.emplace(strCommand);
    }
    m_condition.notify_one();
    //return iResult==REDIS_OK;
}

void CCacheRedis::SetValueAsync(const std::string& strCommand,std::function<void(char*)> &&callback){
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_funcList.emplace(callback);
        m_strCommand.emplace(strCommand);
    }
    m_condition.notify_one();
}