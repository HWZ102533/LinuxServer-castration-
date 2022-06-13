//
// Created by admin on 3/1/22.
//

#include "CSqlMariadb.h"
#include <signal.h>

/* Shortcut for going to new state immediately without waiting. */
//#define NEXT_IMMEDIATE(sd_, new_st) do { sd_->ST= new_st; goto again; } while (0)
Request::Request(const std::string &Query,std::function<void(char *)> &cb):
        strQuery(Query),callback(cb){
}

CSqlMariadb::CSqlMariadb() {}

CSqlMariadb::~CSqlMariadb() {

}

CSqlMariadb::CSqlMariadb(std::string &strIP, int iPort, int iNumConnection, std::string &strUser,
                         std::string &strPassword, std::string &DatabaseName)
                         :m_strIP(strIP), m_iPort(iPort),m_iConnections(iNumConnection),m_strUser(strUser),
                         m_strPassword(strPassword),m_strDatabase(DatabaseName){
    m_bStop = false;
}

void CSqlMariadb::Stop() {
    CSql::Stop();
    m_condition.notify_one();
}

void CSqlMariadb::next_event(int new_st, int status, Connection *connection) {
    return;
}
//mysql_affected_rows
void CSqlMariadb::state_machine_handler(int fd, short event, void *arg) {
}

void CSqlMariadb::AddQuery(const std::string &strCommand, std::function<void(char *)> &&callback) {
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_listQuery.emplace(std::make_shared<Request>(strCommand, callback));
    }
    m_condition.notify_one();
}

bool CSqlMariadb::Init() {
    return true;
}

const std::string &CSqlMariadb::getMStrIp() const {
    return m_strIP;
}

void CSqlMariadb::setMStrIp(const std::string &mStrIp) {
    m_strIP = mStrIp;
}

int CSqlMariadb::getMIPort() const {
    return m_iPort;
}

void CSqlMariadb::setMIPort(int mIPort) {
    m_iPort = mIPort;
}

const std::string &CSqlMariadb::getMStrUser() const {
    return m_strUser;
}

void CSqlMariadb::setMStrUser(const std::string &mStrUser) {
    m_strUser = mStrUser;
}

const std::string &CSqlMariadb::getMStrPassword() const {
    return m_strPassword;
}

void CSqlMariadb::setMStrPassword(const std::string &mStrPassword) {
    m_strPassword = mStrPassword;
}

int CSqlMariadb::getMIConnections() const {
    return m_iConnections;
}

void CSqlMariadb::setMIConnections(int mIConnections) {
    m_iConnections = mIConnections;
}

const std::string &CSqlMariadb::getMStrDatabase() const {
    return m_strDatabase;
}

void CSqlMariadb::setMStrDatabase(const std::string &mStrDatabase) {
    m_strDatabase = mStrDatabase;
}


