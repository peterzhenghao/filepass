#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <vector>
#include <memory>
#include <iostream>
#include  <stdio.h>

//Task类是所有任务类的基类，其中的纯虚函数handleTask()是任务流程的入口，工作线程在处理任务的时候就从此处开始执行任务的处理流程。
//设计一个新的任务时，只需要继续Task接口，新的任务就可以放入线程池中执行。

class ITask
{
public:
    virtual void handleTask() = 0;
};

class Task: public ITask
{
public:
    Task(int id) :taskId(id)
    {

    }
    virtual void handleTask()
    {
        //std::cout << "handle a task:"<< taskId << std::endl;//cout是线程不安全的
        printf("handle a task:%d\n", taskId);

        //only used test
        //std::this_thread::sleep_for(std::chrono::seconds(1));//让线程处理的慢些，可以出现线程总数不够的情况
    }

    virtual ~Task()
    {
        //为了看到一个task的销毁，这里刻意补上其析构函数
        //std::cout << "a task destructed..." << std::endl;
    }

    int taskId;
};

class ThreadPool final
{
public:
    ThreadPool();
    ~ThreadPool();
    ThreadPool(const ThreadPool& rhs) = delete;
    ThreadPool& operator=(const ThreadPool& rhs) = delete;

public:
    void init(int threadNum = 5);
    void stopALLThread();

    void addTask(Task* task);
    void removeAllTasks();

private:
    void threadFunc();

private:
    std::list<std::shared_ptr<Task>>            m_taskList;
    std::mutex                                  m_mutexList;
    std::condition_variable                     m_cv;
    bool                                        m_shutDown;//退出线程的标志位 当标记为true,标记线程池退出
    std::vector<std::shared_ptr<std::thread>>   m_threads;
    std::atomic<int>                            m_runningThreadNum;//正在工作的线程数量
    int                                         m_maxThreadNum;//线程池中线程总数量
    time_t                                      m_iLastEmgTime;
};
