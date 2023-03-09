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
    //�߳������ĳ�ʼ��
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

                //�������˻���������������������Ļ���m_cv.wait()���û��ͷ������ҹ���ǰ�̣߳���˲�����ִ�С�
                //�������仯���������㣬m_cv.wait() �����ѹ�����̣߳��һ������
                m_cv.wait(guard);
            }

            if (m_shutDown)
                break;

            spTask = m_taskList.front();
            m_taskList.pop_front();
        }
        //���һ���߳�ȥ��֮�������߳���ȥȡ��ʱ��Ϊ�յ� important
        if (spTask == NULL)
            continue;

        ++m_runningThreadNum;//���ڹ������߳���+1
        spTask->handleTask();
        spTask.reset();
        --m_runningThreadNum;
    }

    std::cout << "exit thread, threadID: " << std::this_thread::get_id() << std::endl;
}

// �����ǿյģ����������߳�Ҫ�˳�
void ThreadPool::stopALLThread()
{
    m_shutDown = true;
    m_cv.notify_all();//ʹ��������������������

    //�ȴ������߳��˳�
    for (auto& iter : m_threads)
    {
        //�ȴ����������̵߳��˳�����һ��û���أ��ͻ�����ȴ�
        if (iter->joinable())
            iter->join();
    }
}

void ThreadPool::addTask(Task* task)
{
    if (m_maxThreadNum == m_runningThreadNum)
    {
        time_t currtime = time(NULL);
        if (currtime - m_iLastEmgTime > 1)//2�α���ʱ�䲻����1S
        {
            m_iLastEmgTime = currtime;
            printf("CThreadPool::addTask()�з����̳߳��е�ǰ�����߳�����Ϊ0��Ҫ���������̳߳���!");// ������ʵ�Ϳ��Զ�̬����
        }
    }

    std::shared_ptr<Task> spTask;
    spTask.reset(task);

    {
        std::lock_guard<std::mutex> guard(m_mutexList);
        m_taskList.push_back(spTask);
        std::cout << "add a Task:"<< spTask->taskId << std::endl;
    }
    //��������wait���̣߳���ֻ�ܼ���һ���̣߳��ദ�������ܻἤ�������������ٻ���
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