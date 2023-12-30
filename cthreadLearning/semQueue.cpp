// #include <iostream>
// #include <thread>
// #include <unistd.h>
// #include <queue>
// #include <semaphore.h>
// using namespace std;
// pthread_mutex_t mutex;
// // 生产者信号量
// sem_t pSem;
// // 消费者信号量
// sem_t cSem;
// const int MAXSIZE = 50;
// queue<int> Q;
// void *producerF(void *arg)
// {
//     int *id = (int *)arg;
//     while (true)
//     {
//         // pthread_mutex_lock(&mutex);   // 事实上互斥锁不能加到该位置，拿到mutex等待，会造成死锁
//         sem_wait(&pSem);
//         // 这里与cond条件量实现大不相同，原因为当条件量发生阻塞时会主动放弃mutex，被唤醒后重新抢mutex
//         pthread_mutex_lock(&mutex);
//         int element = rand() % 100;
//         Q.push(element);
//         cout << "producer id : " << *id << " number = " << element << "  queue szie = " << Q.size() << endl;
//         pthread_mutex_unlock(&mutex);
//         sem_post(&cSem);
//         usleep(rand() % 3);
//     }
//     return NULL;
// }
// void *consumerF(void *arg)
// {
//     int *id = (int *)arg;
//     while (true)
//     {
//         sem_wait(&cSem);
//         pthread_mutex_lock(&mutex);
//         cout << "consumer id : " << *id << " number = " << Q.front() << "  queue szie = " << Q.size() << endl;
//         Q.pop();
//         pthread_mutex_unlock(&mutex);
//         sem_post(&pSem);
//         sleep(rand() % 3);
//     }
//     return NULL;
// }
// int main(int argc, char *argv[])
// {
//     pthread_mutex_init(&mutex, NULL);
//     sem_init(&pSem, 0, MAXSIZE);
//     sem_init(&cSem, 0, 0);
//     pthread_t producer[5], consumer[5];
//     int pTid[5] = {1, 2, 3, 4, 5};
//     int cTid[5] = {1, 2, 3, 4, 5};
//     for (int i = 0; i < 5; i++)
//     {
//         pthread_create(producer + i, NULL, producerF, pTid + i);
//         pthread_create(consumer + i, NULL, consumerF, cTid + i);
//     }
//     for (int i = 0; i < 5; i++)
//     {
//         pthread_join(producer[i], NULL);
//         pthread_join(consumer[i], NULL);
//     }
//     pthread_mutex_destroy(&mutex);
//     sem_destroy(&pSem);
//     sem_destroy(&cSem);
//     return 0;
// }