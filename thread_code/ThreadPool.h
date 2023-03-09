#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <vector>
#include <memory>
#include <iostream>
#include  <stdio.h>

//Task��������������Ļ��࣬���еĴ��麯��handleTask()���������̵���ڣ������߳��ڴ��������ʱ��ʹӴ˴���ʼִ������Ĵ������̡�
//���һ���µ�����ʱ��ֻ��Ҫ����Task�ӿڣ��µ�����Ϳ��Է����̳߳���ִ�С�

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
        //std::cout << "handle a task:"<< taskId << std::endl;//cout���̲߳���ȫ��
        printf("handle a task:%d\n", taskId);

        //only used test
        //std::this_thread::sleep_for(std::chrono::seconds(1));//���̴߳������Щ�����Գ����߳��������������
    }

    virtual ~Task()
    {
        //Ϊ�˿���һ��task�����٣�������ⲹ������������
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
    bool                                        m_shutDown;//�˳��̵߳ı�־λ �����Ϊtrue,����̳߳��˳�
    std::vector<std::shared_ptr<std::thread>>   m_threads;
    std::atomic<int>                            m_runningThreadNum;//���ڹ������߳�����
    int                                         m_maxThreadNum;//�̳߳����߳�������
    time_t                                      m_iLastEmgTime;
};
