//
// Created by hwz on 9/27/21.
//

#include "CPoolThread.h"

CPoolThread::CPoolThread(int iNum):m_iNum(iNum),m_bStop(false){
}

CPoolThread::~CPoolThread(){
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_bStop = true;
    }
    condition.notify_all();
    for(std::thread &worker: m_workers)
        worker.join();
}

void CPoolThread::Init(){
    if(m_iNum==-1)
        m_iNum = std::thread::hardware_concurrency();
    for(size_t i = 0;i<m_iNum;++i)
        m_workers.emplace_back(
                [this]
                {
                    for(;;)
                    {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(this->m_queue_mutex);
                            //没有任务一直阻塞
                            this->condition.wait(lock,[this]{ return this->m_bStop || !this->m_tasks.empty(); });
                            if(this->m_bStop && this->m_tasks.empty())
                                return;
                            task = std::move(this->m_tasks.front());
                            //this->m_tasks.pop();
                            this->m_tasks.pop_front();
                        }
                        task();
                    }
                }
        );
}





