//
// Created by admin on 3/1/22.
//

#ifndef HWZ_SERVERDFC_CSQLMARIADB_H
#define HWZ_SERVERDFC_CSQLMARIADB_H

#include <string>
#include "CSql.h"
#include <mysql/mysql.h>
#include <event.h>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

class Request{
public:
    Request(const std::string &Query,std::function<void(char *)> &cb);
    std::string strQuery;
    std::function<void(char *)> callback;
};

typedef struct {
    int ST;                                    /* State machine current state */
    struct event *ev_mysql;
    MYSQL mysql;
    MYSQL_RES *result;
    MYSQL *ret;
    int err;
    MYSQL_ROW row;
    void *pSqlMariadb;
    std::shared_ptr<Request> request;
}Connection;


//

class CSqlMariadb :public CSql{
public:
    CSqlMariadb(std::string &strIP,int iPort,int iNumConnection,std::string &strUser,std::string &strPassword,std::string &DatabaseName);

    bool Init() override;

    virtual ~CSqlMariadb();

public:
    int getMIPort() const;

    void setMIPort(int mIPort);

    const std::string &getMStrUser() const;

    void setMStrUser(const std::string &mStrUser);

    const std::string &getMStrPassword() const;

    void setMStrPassword(const std::string &mStrPassword);

    const std::string &getMStrIp() const;

    void setMStrIp(const std::string &mStrIp);

    int getMIConnections() const;

    void setMIConnections(int mIConnections);

    const std::string &getMStrDatabase() const;

    void setMStrDatabase(const std::string &mStrDatabase);

    void next_event(int new_st, int status, Connection *connection);
    static int mysql_status(short event);
    static void state_machine_handler(int fd __attribute__((unused)), short event, void *arg);

    void AddQuery(const std::string &strCommand, std::function<void(char *)> &&callback) override;

    void Stop() override;

private:
    CSqlMariadb();
    int m_iPort;
    int m_iConnections;
    std::string m_strIP;
    std::string m_strUser;
    std::string m_strPassword;
    std::string m_strDatabase;
    struct event_base *m_baseloop;
    pthread_t m_tid_reader;
    std::vector<Connection> m_vecConnects;
    std::queue<std::shared_ptr<Request>> m_listQuery;
    std::mutex m_queue_mutex;
    std::condition_variable m_condition;
};



#endif //HWZ_SERVERDFC_CSQLMARIADB_H
