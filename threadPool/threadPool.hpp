#ifndef _THREADPOLL_HPP_
#define _THREADPOLL_HPP_
#include <thread>
#include <iostream>
#include <unistd.h>
#include <string.h>
using namespace std;
// #include <queue>
// 任务结构体
struct Task
{
    void (*function)(void *arg);
    void *arg;
};
// 线程池结构体
struct threadPool
{
    Task *taskQueue; // 任务队列
    int capacity;
    int sizeQueue;
    int queueFront; // 队头
    int queueRear;  // 队尾

    pthread_t managerId;       // 管理线程ID
    pthread_t *workId;         // 工作线程ID
    int minNum;                // 最少线程数
    int maxNum;                // 最大线程数
    int busyNum;               // 在忙线程数
    int liveNum;               // 存活线程数
    int exitNum;               // 退出线程数
    pthread_mutex_t mutexPool; // 锁整个线程池
    pthread_mutex_t mutexBusy; // 给busyNum单独加锁提高效率
    int shutDown;              // 判断当前线程池是否工作, 销毁为1，不销毁为0

    pthread_cond_t notFull;  // 任务队列是否已满
    pthread_cond_t notEmpty; // 任务队列是否已空
};
// 创建线程池函数 capacity 为任务队列容量，maxNum与minNum 分别为worker线程存活的最大值和最小值
threadPool *threadPoolCreate(int capacity, int maxNum, int minNum);
// 销毁线程池函数 pool为要销毁的线程池地址
int threadPoolDestroy(threadPool *pool);
// 给线程池添加任务 pool 为对应线程池地址，func为函数地址，arg为func要穿的参数地址
void threadPoolAdd(threadPool *pool,void (*func)(void *), void *arg);
// 获取线程池工作的线程数 pool为对应线程池地址
int threadPoolBusyNum(threadPool *pool);
// 获取线程池活着的线程数 pool为对应线程池地址
int threadPoolLiveNum(threadPool *pool);
// worker函数 arg为线程回调函数形参地址
void *worker(void *arg);
// manager 函数 arg为线程回调函数形参地址
void *manager(void *arg);
// 线程退出函数
void threadExit(threadPool *pool);
#endif