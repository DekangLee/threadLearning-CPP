#include <iostream>
// #include <pthread.h>
#include <thread>
#include <unistd.h>
using namespace std;
#define MAX 50
pthread_mutex_t pMutex;
int number = int(); // 全局变量默认初始化为零
void *countA(void *arg)
{
    for (int i = 0; i < MAX; i++)
    {
        pthread_mutex_lock(&pMutex);
        int cur = number;
        cur++;
        number = cur;
        cout << "Thread A id = " << pthread_self() % 29 << "  number = " << number << "  countA = " << i << endl;
        pthread_mutex_unlock(&pMutex);
        usleep(50);
    }
    pthread_exit(NULL);
    return NULL;
}
void *countB(void *arg)
{
    for (int i = 0; i < MAX; i++)
    {
        pthread_mutex_lock(&pMutex);
        int cur = number;
        cur++;
        number = cur;
        cout << "Thread B id = " << pthread_self() % 29 << "  number = " << number << "  countB = " << i << endl;
        pthread_mutex_unlock(&pMutex);
        usleep(20);
    }
    pthread_exit(NULL);
    return NULL;
}
void *countC(void *arg)
{
    for (int i = 0; i < MAX; i++)
    {
        pthread_mutex_lock(&pMutex);
        int cur = number;
        cur++;
        number = cur;
        cout << "Thread C id = " << pthread_self() % 29 << "  number = " << number << "  countC = " << i << endl;
        pthread_mutex_unlock(&pMutex);
        usleep(10);
    }
    pthread_exit(NULL);
    return NULL;
}
int main()
{
    pthread_mutex_init(&pMutex, NULL);
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, countA, NULL); // 创建线程A
    pthread_create(&tid2, NULL, countB, NULL); // 创建线程B
    pthread_create(&tid3, NULL, countC, NULL); // 创建线程C
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL); // 阻塞回收资源
    pthread_join(tid3, NULL);
    cout << "Main Thread number = " << number << endl;
    pthread_exit(NULL); // 主线程退出，事实上没必要，前面已经有阻塞函数
    return 0;
}