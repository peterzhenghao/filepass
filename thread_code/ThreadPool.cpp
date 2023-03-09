#include <iostream>
#include "ThreadPool.h"
#include <functional>

using namespace std;

ThreadPool::ThreadPool() : m_shutDown(true)
{

}

ThreadPool::~ThreadPool()
{
    removeAllTasks();
}

void ThreadPool::init(int threadNum/* = 5*/)
{
    //线程数量的初始化
    if (threadNum <= 0)
        m_maxThreadNum = 5;
    else
        m_maxThreadNum = threadNum;
    

    m_shutDown = false;

    for (int i = 0; i < m_maxThreadNum; ++i)
    {
        std::shared_ptr<std::thread> spThread;
        spThread.reset(new std::thread(std::bind(&ThreadPool::threadFunc, this)));
        m_threads.push_back(spThread);
    }
}

void ThreadPool::threadFunc()
{
    std::shared_ptr<Task> spTask;
    while (true)
    {
        {
            std::unique_lock<std::mutex> guard(m_mutexList);
            while (m_taskList.empty() && !m_shutDown)
            {

                //如果获得了互斥锁，但是条件不满足的话，m_cv.wait()调用会释放锁，且挂起当前线程，因此不往下执行。
                //当发生变化后，条件满足，m_cv.wait() 将唤醒挂起的线程，且获得锁。
                m_cv.wait(guard);
            }

            if (m_shutDown)
                break;

            spTask = m_taskList.front();
            m_taskList.pop_front();
        }
        //最后一个线程去完之后，其他线程再去取的时候为空的 important
        if (spTask == NULL)
            continue;

        ++m_runningThreadNum;//正在工作的线程数+1
        spTask->handleTask();
        spTask.reset();
        --m_runningThreadNum;
    }

    std::cout << "exit thread, threadID: " << std::this_thread::get_id() << std::endl;
}

// 任务是空的，但是所有线程要退出
void ThreadPool::stopALLThread()
{
    m_shutDown = true;
    m_cv.notify_all();//使得所有条件变量被激活

    //等待所有线程退出
    for (auto& iter : m_threads)
    {
        //等待所有所有线程的退出，有一个没返回，就会继续等待
        if (iter->joinable())
            iter->join();
    }
}

void ThreadPool::addTask(Task* task)
{
    if (m_maxThreadNum == m_runningThreadNum)
    {
        time_t currtime = time(NULL);
        if (currtime - m_iLastEmgTime > 1)//2次报告时间不超过1S
        {
            m_iLastEmgTime = currtime;
            printf("CThreadPool::addTask()中发现线程池中当前空闲线程数量为0，要考虑扩容线程池了!");// 这里其实就可以动态扩容
        }
    }

    std::shared_ptr<Task> spTask;
    spTask.reset(task);

    {
        std::lock_guard<std::mutex> guard(m_mutexList);
        m_taskList.push_back(spTask);
        std::cout << "add a Task:"<< spTask->taskId << std::endl;
    }
    //激活正在wait的线程，且只能激活一个线程，多处理器可能会激发多个，属于虚假唤醒
    m_cv.notify_one();

    
}

void ThreadPool::removeAllTasks()
{
    {
        std::lock_guard<std::mutex> guard(m_mutexList);
        for (auto& iter : m_taskList)
        {
            iter.reset();
        }
        m_taskList.clear();
    }
}