
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "syncQueue.h"

#include <thread>
#include <memory>
#include <list>
#include <atomic>



using namespace std;


const int MaxTaskCount = 100;

class ThreadPool
{
public:
    using Task = std::function<void()>;

    ThreadPool(int numThreads = std::thread::hardware_concurrency()) : m_queue(MaxTaskCount)
    {
        Start(numThreads);
    }

    ~ThreadPool()
    {
        Stop();
    }

    void Stop()
    {
        //保证多线程情况下只调用一次StopThreadGroup
        std::call_once(m_flag, [this]{StopThreadGroup(); });
    }

    void AddTask(Task&& task)
    {
        //m_queue.Put(std::forward<Task>(task));
        m_queue.Put(task);
    }

    void AddTask(const Task& task)
    {
        m_queue.Put(task);
    }

private:
    void Start(int numThreads)
    {
        m_running = true;

        //创建线程组
        for (size_t i = 0; i < numThreads; ++i)
        {
            m_threadGroup.push_back(std::make_shared<std::thread>(&ThreadPool::RunInThread, this));
			//写法二
			//m_threadGroup.push_back(std::make_shared<std::thread>(std::bind(&ThreadPool::RunInThread, this)));
			//写法三
			//m_threadGroup.push_back(std::make_shared<std::thread>([this](){ ThreadPool::RunInThread(); }));
			
        }
    }

    void RunInThread()
    {
        while (m_running)
        {
	
            //取任务分别执行
            std::list<Task> list;
            m_queue.Take(list);

            for (auto& task : list)
            {
                if (!m_running)
                {
                    return;
                }

                cout << "异步层线程处理任务ID：" << std::this_thread::get_id() << endl;
                task();
            }
        }
    }

    void StopThreadGroup()
    {
        m_queue.Stop();
        m_running = false;

        for (auto thread : m_threadGroup)
        {
            if (thread)
            {
                thread->join();
            }
        }

        m_threadGroup.clear();
    }

private:
    std::list<std::shared_ptr<std::thread>> m_threadGroup;
    SyncQueue<Task> m_queue;
    atomic_bool m_running;      ///是否停止标志
    std::once_flag m_flag;

};



#endif //THREADPOOL_H
