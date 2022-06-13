//
// Created by admin on 4/29/22.
//

#include "CTimeWheel.h"
#include <signal.h>

#include <sys/time.h>
#include <unistd.h>
#include <thread>

CTimeWheel::CTimeWheel():m_iDis(1000),m_iCurrentS(0),m_iCurrentM(0),m_iCurrentH(0),m_bStop(false) {
    m_seconds.reserve(256);
    m_minutes.reserve(64);
    m_hours.reserve(64);
    std::list<std::function<void()> >list;
    std::list<Task> listTask;
    for(int i=0;i<256;++i){
        m_seconds.emplace_back(list);
    }
    for(int i=0;i<64;++i){
        m_minutes.emplace_back(listTask);
        m_hours.emplace_back(listTask);
    }
}

CTimeWheel::~CTimeWheel() {
    pthread_join(m_tid_reader, nullptr);
}

CTimeWheel& CTimeWheel::Instance(){
    static CTimeWheel instance;
    return instance;
}

//pData not use , transfer data use pInfo and use sigqueue
//void CTimeWheel::Run(int signo,siginfo_t *pInfo, void *pData){
//    //CTimeWheel *pThis = (CTimeWheel*)pData;
//}
//
//void CTimeWheel::RunSecond(int signo,siginfo_t *pInfo, void *pData){
//    CTimeWheel *pThis = (CTimeWheel*)pData;
//    if(signo==SIGALRM){
//        alarm(pThis->m_iDis);
//        printf("thread id:%d",std::this_thread::get_id());//std::this_thread::get_id(); gettid()
//    }
//}

void CTimeWheel::Run(int signo){
    //if(SIGPROF==signo){
    if(SIGALRM==signo){
        std::unique_lock<std::mutex> lock(CTimeWheel::Instance().m_queue_mutex);
        for( auto Item : CTimeWheel::Instance().m_seconds[CTimeWheel::Instance().m_iCurrentS])
            Item();
        CTimeWheel::Instance().m_seconds[CTimeWheel::Instance().m_iCurrentS].clear();
        CTimeWheel::Instance().m_iCurrentS = (CTimeWheel::Instance().m_iCurrentS+1)&255;
        if(CTimeWheel::Instance().m_iCurrentS==0){
            CTimeWheel::Instance().m_iCurrentM = (CTimeWheel::Instance().m_iCurrentM+1)&63;
            if(CTimeWheel::Instance().m_iCurrentM==0){
                CTimeWheel::Instance().m_iCurrentH = (CTimeWheel::Instance().m_iCurrentH+1)&63;
                for( auto Item : CTimeWheel::Instance().m_hours[CTimeWheel::Instance().m_iCurrentH]){
                    unsigned int iMinute = (Item.iPos&16128)>>8;//
                    Item.iPos = Item.iPos&255;
                    CTimeWheel::Instance().m_minutes[iMinute].emplace_back(Item);
                }
                CTimeWheel::Instance().m_hours[CTimeWheel::Instance().m_iCurrentH].clear();
            }
            for( auto Item : CTimeWheel::Instance().m_minutes[CTimeWheel::Instance().m_iCurrentM])
                CTimeWheel::Instance().m_seconds[Item.iPos].emplace_back(Item.func);
            CTimeWheel::Instance().m_minutes[CTimeWheel::Instance().m_iCurrentM].clear();
        }
    }
}

void CTimeWheel::Stop(){
    m_bStop = true;
}

void CTimeWheel::RunSecond(int signo){
    if(signo==SIGALRM){
        alarm(CTimeWheel::Instance().m_iDis);
        CTimeWheel::Instance().m_alarmFunc();
    }
}

//void CTimeWheel::InitAlarm(int iDis,std::function<void()> &&callback){
//    m_iDis = iDis;
//    m_alarmFunc = std::move(callback);
//
//    struct sigaction act;
//    act.sa_handler = RunSecond;
//    act.sa_flags = 0;// 0
////act.sa_sigaction = RunSecond;
////    act.sa_flags = SA_SIGINFO;
//    sigemptyset(&act.sa_mask);
//    sigaction(SIGALRM,&act,NULL); //设置信号 SIGPROF 的处理函数为 Run
//    alarm(m_iDis);
//
////    sigset_t wait,old;   //三个信号集
////    sigemptyset(&wait);
////    sigaddset(&wait, SIGALRM);  //SIGINT信号加入到new信号集中
////    sigprocmask(SIG_BLOCK, &wait, &old);//just for single thread
//}


bool CTimeWheel::Init(int iType , int iDis,std::function<void()> &&alarmBack) {
    //屏蔽SIGUSR1等信号，新创建的线程将继承该屏蔽字
    m_iDis = iDis;
    m_iSignal = iType;
    sigset_t tBlockSigs;
    sigemptyset(&tBlockSigs);
    sigaddset(&tBlockSigs, SIGALRM);
    sigaddset(&tBlockSigs, SIGPROF);
    sigaddset(&tBlockSigs, SIGQUIT);
//         sigaddset(&tBlockSigs, SIGRTMIN+2);
//         sigaddset(&tBlockSigs, SIGRTMAX);
//         sigaddset(&tBlockSigs, SIGUSR1);
//         sigaddset(&tBlockSigs, SIGUSR2);
//         sigaddset(&tBlockSigs, SIGINT);
//         sigaddset(&tBlockSigs, SIGSEGV); //试图阻塞SIGSEGV信号
    //设置线程信号屏蔽字
    pthread_sigmask(SIG_BLOCK, &tBlockSigs, NULL);
    //signal(SIGINT, sighandler); //试图捕捉SIGINT信号

    switch(m_iSignal){
        case SIGALRM:
            m_alarmFunc = std::move(alarmBack);
            break;
        case SIGPROF:
            break;
        default:
            return false;
    }

    pthread_create(&m_tid_reader,NULL,
                   [](void *arg)->void*{
                       CTimeWheel *pThis = reinterpret_cast<CTimeWheel*>(arg);
                       //pThis->Run(SIGPROF);
                       //pThis->InitAlarm();
                       sigset_t tBlockSigs;
                       sigemptyset(&tBlockSigs);
                       sigaddset(&tBlockSigs, SIGALRM);
                       sigaddset(&tBlockSigs, SIGPROF);
                       //设置线程信号屏蔽字
                       pthread_sigmask(SIG_UNBLOCK, &tBlockSigs, NULL);
                       struct sigaction act;
                       act.sa_handler = RunSecond;
                       act.sa_flags = 0;
                       sigemptyset(&act.sa_mask);
                       sigaction(SIGALRM,&act,NULL); //设置信号 SIGPROF 的处理函数为 Run
                       if(pThis->m_iSignal==SIGALRM)
                           alarm(pThis->m_iDis);
                       else{
//                           struct sigaction act;
//                           act.sa_handler = Run;
//                           act.sa_flags = 0;
//                           sigemptyset(&act.sa_mask);
//                           //sigaction(SIGPROF,&act,NULL); //设置信号 SIGPROF 的处理函数为 Run
//                           sigaction(SIGALRM,&act,NULL); //设置信号 SIGPROF 的处理函数为 Run
                           struct itimerval value;
                           value.it_value.tv_sec=0; //定时器启动后，每隔2秒将执行相应的函数
                           value.it_value.tv_usec=pThis->m_iDis*1000;
                           value.it_interval=value.it_value;
                           //setitimer(ITIMER_PROF,&value,NULL); //初始化 timer，到期发送 SIGPROF 信号
                           setitimer(ITIMER_REAL,&value,NULL); //初始化 timer，到期发送 SIGPROF 信号
                       }

                       printf("child thread id:%d\n",std::this_thread::get_id());//std::this_thread::get_id(); gettid()

                       while(true){
                           select(0,NULL,NULL,NULL,NULL);
                           if(pThis->m_bStop)
                               break;
                       }

//                       sigset_t wait;
//                       sigemptyset(&wait);
//                       sigaddset(&wait, SIGALRM);  //SIGUSR1信号加入wait
//                       printf("child thread id:%d\n",std::this_thread::get_id());//std::this_thread::get_id(); gettid()
//                       int dwSigNo;
//                       int dwRet;
//                       while(true){
//                           //select(0,NULL,NULL,NULL,NULL);
//                           dwRet = sigwait(&wait,&dwSigNo);
//                           if(dwRet == 0)
//                               RunSecond(SIGALRM);
//                           printf("sigwait out:\n");
//                           if(pThis->m_bStop)
//                               break;
//                       }
                       return nullptr;
                   }
            ,this);
    return false;
}

void CTimeWheel::AddTask(int iTime,std::function<void()> &&callback) {
//    int iHour = std::floor(iTime/3600);
//    int iMinute = std::floor(iTime/60);
//    int iSecond = iTime%60;
    unsigned int iHour = (iTime&1032192)>>14;
    unsigned int iMinute = (iTime&16128)>>8;//
    unsigned int iSecond = iTime&255;//
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        int iSecond2 = (iSecond+m_iCurrentS)&256;
        iMinute += iSecond2;
        int iMinute2 = (iMinute+m_iCurrentM)&64;
        iHour += iMinute2;
        if(iHour>0){
            iHour = (iHour+m_iCurrentH)&63;
            Task task;
            task.iPos = (m_iCurrentM + iMinute)&63<<8+(iSecond+m_iCurrentS)&255;
            task.func = std::move(callback);
            m_hours[iHour].emplace_back(task);
        }else if(iMinute>0){
            iMinute += m_iCurrentM;
            Task task;
            task.iPos = (iSecond+m_iCurrentS)&255;
            task.func = std::move(callback);
            m_minutes[iMinute].emplace_back(task);
        }else if(iSecond>0){
            iSecond = (iSecond+m_iCurrentS)&255;//
            m_seconds[iSecond].emplace_back(std::move(callback));
        }
    }
}
