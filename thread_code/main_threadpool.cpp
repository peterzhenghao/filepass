#include "ThreadPool.h"
#include <chrono>

int main()
{
    ThreadPool threadPool;
    constexpr int threadNum = 2;
    threadPool.init(threadNum);

    //std::this_thread::sleep_for(std::chrono::seconds(5));
    //

    Task* task = NULL;
    constexpr int taskNum = 50000;
    for (int i = 0; i < 10000; ++i)
    {
        task = new Task(i);
        threadPool.addTask(task);
        //Ӧ�����̴߳����ʱ����add task,���ܹ����߳������������
    }
    std::this_thread::sleep_for(std::chrono::seconds(3));
    for (int i = 10000; i < taskNum; ++i)
    {
        task = new Task(i);
        threadPool.addTask(task);
        //Ӧ�����̴߳����ʱ����add task
    }

    std::this_thread::sleep_for(std::chrono::seconds(50));

    threadPool.stopALLThread();

    return 0;
}