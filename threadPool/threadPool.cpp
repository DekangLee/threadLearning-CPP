#include "threadPool.hpp"
const int NUMBER = 2;
// 创建线程池函数
threadPool *threadPoolCreate(int capacity, int maxNum, int minNum)
{
    threadPool *pool = new threadPool;
    do
    {
        if (pool == NULL)
        {
            cout << "Failed to create the threadPool!" << endl;
            break;
        }
        pool->workId = new pthread_t[maxNum];
        if (pool->workId == NULL)
        {
            cout << "Failed new pool->workID..." << endl;
            break;
        }
        // 初始化线程ID
        memset(pool->workId, 0, sizeof(pool->workId) * maxNum);
        pool->capacity = capacity;
        pool->maxNum = maxNum;
        pool->minNum = minNum;
        pool->busyNum = 0;
        pool->liveNum = minNum;                                // 与最小个数相等
        if (pthread_mutex_init(&pool->mutexPool, NULL) != 0 || // 初始化成功返回值应该为0
            pthread_mutex_init(&pool->mutexBusy, NULL) != 0 ||
            pthread_cond_init(&pool->notEmpty, NULL) != 0 ||
            pthread_cond_init(&pool->notFull, NULL) != 0)
        {
            cout << "Failed to initlizing...." << endl;
            break;
        }
        pool->shutDown = 0;
        // 初始化任务队列
        pool->taskQueue = new Task[capacity];
        pool->capacity = capacity;
        pool->queueFront = 0;
        pool->queueRear = 0;
        pool->sizeQueue = 0;

        // 创建管理者线程
        pthread_create(&pool->managerId, NULL, manager, pool);

        // 创建workerThread
        for (int i = 0; i < minNum; i++)
        {
            pthread_create(pool->workId + i, NULL, worker, pool);
        }
        return pool;
    } while (0);
    if (pool->workId != NULL)
        delete[] pool->workId;
    if (pool->taskQueue != NULL)
        delete[] pool->taskQueue;
    if (pool != NULL)
        delete pool;
}
void *worker(void *arg) // 传进来的arg即为线程池，原因为worker需要不断读取线程池的内容，包括任务队列和状态
{
    threadPool *pool = (threadPool *)arg;
    while (true)
    {
        pthread_mutex_lock(&pool->mutexPool); // 对线程池加锁，不能让其余线程访问/更改线程池的各种属性和内容
        // 阻塞条件为当前任务数为0，并且线程池没有被关闭（如果线程池被关闭，不需要阻塞）
        while (pool->sizeQueue == 0 && pool->shutDown == 0)
        {
            // 阻塞在该部的线程还没有拿到任务，可能会因为，manager线程的设置而自动退出
            pthread_cond_wait(&pool->notEmpty, &pool->mutexPool);
            if (pool->exitNum > 0)
            {
                pool->exitNum--;
                if (pool->liveNum > pool->minNum)
                {
                    pool->liveNum--;
                    pthread_mutex_unlock(&pool->mutexPool);
                    threadExit(pool);
                }
            }
        }
        // 判断线程池是否结束
        if (pool->shutDown == 1)
        {
            threadExit(pool);
        }
        // 上述条件判断通过，开始进行worK
        // 从任务队列中取出任务（函数）
        Task task;
        task.function = pool->taskQueue[pool->queueFront].function;
        task.arg = pool->taskQueue[pool->queueFront].arg;
        // 移动头节点
        pool->queueFront = (pool->queueFront + 1) % pool->capacity; // 这里将队列看成环形队列
        pool->sizeQueue--;
        pthread_cond_signal(&pool->notFull); // 消费者消耗队列里的一个元素，队列一定不为空，唤醒生产者进行任务添加
        pthread_mutex_unlock(&pool->mutexPool);
        // 开始工作
        cout << "Thread " << pthread_self() << " start working ...." << endl;
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexBusy);

        task.function(task.arg);
        delete task.arg;
        task.arg = nullptr;
        cout << "Thread " << pthread_self() << " end of working ...." << endl;
        // 结束工作
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexBusy);
    }

    return NULL;
}
// manager 函数
void *manager(void *arg)
{
    threadPool *pool = (threadPool *)arg;
    while (!pool->shutDown)
    {
        // 每隔3秒钟检测一次
        sleep(3);
        // 读取当前线程池里，活动线程的数量
        pthread_mutex_lock(&pool->mutexPool);
        int queueSize = pool->sizeQueue;
        int liveNum = pool->liveNum;
        pthread_mutex_unlock(&pool->mutexPool);

        pthread_mutex_lock(&pool->mutexBusy);
        int busyNum = pool->busyNum;
        pthread_mutex_unlock(&pool->mutexBusy);

        // 添加线程
        // 制定规则：当前任务数 > 存活线程数 并且 存活线程数 < 最大线程数

        if (queueSize > liveNum && liveNum < pool->maxNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            int counter = 0;
            for (int i = 0; i < pool->maxNum && counter < NUMBER && pool->liveNum < pool->maxNum; ++i)
            {
                if (pool->workId[i] == 0)
                {
                    pthread_create(&pool->workId[i], NULL, worker, pool);
                    counter++;
                    pool->liveNum++;
                }
            }
            pthread_mutex_unlock(&pool->mutexPool);
        }

        // 销毁线程
        // 制定规则：busyNum * 2 <liveNum && liveNum > minNum 说明线程数量太多根本不能充分利用
        if (busyNum * 2 < liveNum && liveNum > pool->minNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitNum = NUMBER;
            pthread_mutex_unlock(&pool->mutexPool);
            // 使线程自动退出
            for (int i = 0; i < NUMBER; ++i)
            {
                pthread_cond_signal(&pool->notEmpty);
            }
        }
    }
    return NULL;
}
void threadExit(threadPool *pool)
{
    pthread_t tid = pthread_self();
    for (int i = 0; i < pool->maxNum; i++)
    {
        if (pool->workId[i] == tid)
        {
            pool->workId[i] = 0;
            cout << "Exiting thread id :" << tid << endl;
            break;
        }
    }
    pthread_exit(NULL);
}
// 给线程池添加任务
void threadPoolAdd(threadPool *pool, void (*func)(void *), void *arg)
{
    pthread_mutex_lock(&pool->mutexPool);
    while (pool->sizeQueue == pool->capacity && pool->shutDown == 0)
    {
        // 阻塞生产者线程
        pthread_cond_wait(&pool->notFull, &pool->mutexPool);
    }
    if (pool->shutDown == 1) // 线程池被关闭
    {
        pthread_mutex_unlock(&pool->mutexPool);
        return;
    }
    // 添加任务
    pool->taskQueue[pool->queueRear].function = func; // 保存函数指针
    pool->taskQueue[pool->queueRear].arg = arg;       // 保存参数变量地址
    // 移动尾节点
    pool->queueRear = (pool->queueRear + 1) % pool->capacity;
    pool->sizeQueue++;
    pthread_cond_signal(&pool->notEmpty); // 任务队列不为空，消费者可以进行消费， 这里唤醒的线程为work线程
    pthread_mutex_unlock(&pool->mutexPool);
}
// 获取线程池工作的线程数
int threadPoolBusyNum(threadPool *pool)
{
    int busyNum;
    pthread_mutex_lock(&pool->mutexBusy);
    busyNum = pool->busyNum;
    pthread_mutex_unlock(&pool->mutexBusy);
    return busyNum;
}
// 获取线程池活着的线程数
int threadPoolLiveNum(threadPool *pool)
{
    int liveNum;
    pthread_mutex_lock(&pool->mutexPool);
    liveNum = pool->liveNum;
    pthread_mutex_unlock(&pool->mutexPool);
    return liveNum;
}
// 销毁线程池函数
int threadPoolDestroy(threadPool *pool)
{
    // 判断pool是否为有效的地址
    if (pool == NULL)
        return -1;
    // 回收管理者线程
    pool->shutDown = 1;
    // 阻塞等待回收
    pthread_join(pool->managerId, NULL);
    // 唤醒阻塞的消费者线程
    for (int i = 0; i < pool->liveNum; i++)
    {
        pthread_cond_signal(&pool->notEmpty);
    }
    // 释放堆内存
    if (pool->taskQueue != NULL)
        delete[] pool->taskQueue;
    if (pool->workId != NULL)
        delete[] pool->taskQueue;
    // 释放互斥锁资源
    pthread_mutex_destroy(&pool->mutexBusy);
    pthread_mutex_destroy(&pool->mutexPool);
    pthread_cond_destroy(&pool->notEmpty);
    pthread_cond_destroy(&pool->notFull);
    delete pool;
    pool = NULL;
    return 0;
}