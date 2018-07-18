/*
功能:  半同步半异步，添加任务到同步队列，线程池异步处理
技术:  C++11标准
要点:  本代码使用list

20170715
vector,liast,deque的使用区别：
1 如果你需要高效的随即存取，而不在乎插入和删除的效率，使用vector
2 如果你需要大量的插入和删除，而不关心随即存取，则应使用list
3 如果你需要随即存取，而且关心两端数据的插入和删除，则应使用deque
*/

#include <mutex>
#include <list>
#include <thread>
#include <condition_variable>
#include <iostream>

using namespace std;

template<class T>
class SyncQueue
{
public:
    SyncQueue(int size) : m_maxSize(size)
    {

    }

    void Put(const T& x)
    {
        //写法①
        //std::lock_quard<std::mutex> locker(m_mutex);

        //写法②
        //std::unique_lock<std::mutex> locker(m_mutex);

        //写法①
        //while (IsFull())
        //{
        //    cout << "缓冲区满了，需要等待..." << endl;
        //    m_notFull.wait(m_mutex);
        //}

        //写法②
        //m_notFull.wait(locker, [this] void (){return m_needStop || NotFull(); });

        //if (m_needStop)
        //{
        //    return;
        //}

        //m_queue.push_back(x);
        //m_notEmpty.notify_one();
        Add(x);
    }

    void Put(T&& x)
    {
        //Add(std::forward(x));
        Add(x);
    }

    void Take(std::list<T>& list)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notEmpty.wait(locker, [this]{return m_needStop || NotEmpty(); });

        if (m_needStop)
        {
            return;
        }

        list = std::move(m_queue);
        m_notFull.notify_one();
    }

    void Take(T& x)
    {
        std::lock_guard<std::mutex> locker(m_mutex);

        m_notEmpty.wait(locker, [this]{return m_needStop || NotEmpty(); });

        //while (IsEmpty())
        //{
        //    cout << "缓冲区空了，需要等待..." << endl;
        //    m_notEmpty.wait(m_mutex);
        //}
        if (m_needStop)
        {
            return;
        }

        x = m_queue.front();
        m_queue.pop_front();
        m_notFull.notify_one();
    }

    void Stop()
    {
        {
            std::lock_guard<std::mutex> locker(m_mutex);

            m_needStop = true;
        }

        m_notFull.notify_all();
        m_notEmpty.notify_all();
    }

    bool Empty()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.empty();
    }

    bool Full()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size() == m_maxSize;
    }

    size_t Size()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size();
    }

private:
    bool NotFull() const
    {
        bool full = m_queue.size() >= m_maxSize;

        if (full)
        {
            cout << "缓冲区满了，需要等待..." << endl;
        }

        return !full;
    }

    bool NotEmpty() const
    {
        bool empty = m_queue.empty();

        if (empty)
        {
            cout << "缓冲区空了，需要等待...，异步层的线程ID：" << std::this_thread::get_id() << endl;
        }

        return !empty;
    }

    template<typename F>
    void Add(F&& x)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notFull.wait(locker, [this]{return m_needStop || NotFull(); });

        if (m_needStop)
        {
            return;
        }

        m_queue.emplace_back(std::forward<F>(x));
        m_notEmpty.notify_one();
    }

private:
    std::list<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_notEmpty;
    std::condition_variable m_notFull;
    int m_maxSize;
    bool m_needStop;
};
