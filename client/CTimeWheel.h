//
// Created by admin on 4/29/22.
//

#ifndef CLIENT_CTIMEWHEEL_H
#define CLIENT_CTIMEWHEEL_H
#include <vector>
#include <list>
#include <functional>
#include <csignal>
#include <mutex>
#include <atomic>

typedef struct {
    unsigned int iPos;
    std::function<void()> func;
}Task;

class CTimeWheel {
public:
    static CTimeWheel& Instance();
    virtual ~CTimeWheel();
    bool Init(int iType , int iDis,std::function<void()> &&alarmBack);
    void Stop();
    void AddTask(int iTime,std::function<void()> &&callback);//
private:
    CTimeWheel();
    //void InitAlarm(int iDis,std::function<void()> &&callback);//计时器的精度为 s repeat do function
    //void InitSetTimerH();//计时器的精度为 ns timer_create() timer_settime() timer_gettime timer_getoverrun timer_delete
//    static void RunSecond(int signo,siginfo_t *pInfo, void *pData);
//    static void Run(int signo,siginfo_t *pInfo, void *pData);
    static void RunSecond(int signo);
    static void Run(int signo);
    int m_iDis;//100ms

    std::mutex m_queue_mutex;
    std::function<void()> m_alarmFunc;

    pthread_t m_tid_reader;
    std::vector<std::list<Task> > m_hours;//256 2^8
    std::vector<std::list<Task> > m_minutes;//64 2^6
    std::vector<std::list<std::function<void()>> > m_seconds;//64   2^6
    int m_iCurrentS;
    int m_iCurrentM;
    int m_iCurrentH;
    std::atomic<bool> m_bStop;
    int m_iSignal;
};


#endif //CLIENT_CTIMEWHEEL_H
