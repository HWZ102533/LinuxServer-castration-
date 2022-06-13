//
// Created by admin on 4/9/22.
//

#ifndef HWZ_SERVERDFC_CFILEAIO_H
#define HWZ_SERVERDFC_CFILEAIO_H

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <libaio.h>
#include <sys/epoll.h>

//上传下载优化方案：
//方案a.专门的下载服务器，将要更新的文件全部预读进内存。
//方案b.libaio模式下考虑将与套接字对应的用户缓存拓展到1024,并且数据对齐。
//client:sendfile + Server:libaio Writefile .gz error
class CFileAio {
public:
    virtual bool Init() ;
    //virtual std::string GetValue(const std::string& str,std::function<void(char*)> &&callback) = 0;
    virtual void readFile(void* pData);
    virtual void writeFile(void* pData);
    bool GetStop(){
        return m_bStop;
    }
    virtual void Stop(){
        m_bStop = true;
    }
    static CFileAio& GetInstance(){
        static CFileAio fileAio;
        return fileAio;
    }
    static void aio_callback(io_context_t ctx, struct iocb *iocb, long res, long res2);
protected:
    CFileAio();
    virtual ~CFileAio();
    std::atomic<bool> m_bStop;
    pthread_t m_tid_reader;
    std::mutex m_queue_mutex;
    std::condition_variable m_condition;
    std::queue<void*> m_command;//prevent memory leaked

    void *m_buf;
    int m_efd;
    int m_epfd;
    io_context_t m_ctx;
    struct io_event m_events[1];
    struct iocb m_iocbs[1];
    struct iocb *m_iocbps[1];
    struct epoll_event m_epevent;
    unsigned m_nEvents;
};



#endif //HWZ_SERVERDFC_CFILEAIO_H
