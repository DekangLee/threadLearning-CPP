#include "threadPool.hpp"
pthread_mutex_t pMutex;
void taskFunc(void *arg)
{
    int *num = (int *)arg;
    pthread_mutex_lock(&pMutex);
    cout << "threadId:" << pthread_self() << " num = " << *num << endl;
    pthread_mutex_unlock(&pMutex);
    sleep(1);
}
int main()
{
    pthread_mutex_init(&pMutex, NULL);
    threadPool *pool = threadPoolCreate(50, 200, 5);
    for (int i = 0; i < 50; i++)
    {
        int *num = new int(i + 100);
        threadPoolAdd(pool, taskFunc, num);
    }
    pthread_mutex_lock(&pMutex);
    cout << "size task queue = " << pool->sizeQueue << endl;
    pthread_mutex_unlock(&pMutex);
    sleep(10);
    cout << "size task queue = " << pool->sizeQueue << endl;
    threadPoolDestroy(pool);
}