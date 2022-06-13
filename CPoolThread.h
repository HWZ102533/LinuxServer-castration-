//
// Created by hwz on 9/27/21.
//

#ifndef HWZ_SERVERDFC_CPOOLTHREAD_H
#define HWZ_SERVERDFC_CPOOLTHREAD_H

#include <vector>
#include <list>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
//std::async<>
//std::promise<>
//std::packaged_task<>

class CPoolThread{
public:
    CPoolThread(int iNum);
    ~CPoolThread();
    virtual void Init();

    template<class F, class... Args>
    auto enqueue(bool bTail,F&& f, Args&&... args)-> std::future<typename std::result_of<F(Args...)>::type>;
protected:
    int m_iNum;
    std::mutex m_ThreadMutex;//处理两个线程同时操作一个socket的局面
    // need to keep track of threads so we can join them
    std::vector< std::thread > m_workers;
    // the task queue
    //std::queue< std::function<void()> > tasks;
    std::list< std::function<void()> > m_tasks;
    // synchronization
    std::mutex m_queue_mutex;
    std::condition_variable condition;
    //std::atomic<bool> m_bStop;
    bool m_bStop;
};

// add new work item to the pool
template<class F, class... Args>
auto CPoolThread::enqueue(bool bTail,F&& f, Args&&... args)
-> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        // don't allow enqueueing after stopping the pool
        if(m_bStop)
            throw std::runtime_error("enqueue on stopped ThreadPool");
        //m_tasks.emplace([task](){ (*task)(); });
       if(bTail)
          m_tasks.emplace_back([task](){ (*task)(); });
       else
          m_tasks.emplace_front([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}

#endif //HWZ_SERVERDFC_CPOOLTHREAD_H
