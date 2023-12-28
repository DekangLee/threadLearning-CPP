// #include <iostream>
// #include <thread>
// #include <unistd.h>
// #include <queue>
// using namespace std;
// pthread_cond_t cond;
// pthread_mutex_t mutex;
// const int MAXSIZE = 50;
// queue<int> Q;
// void *producerF(void *arg)
// {
//     int *id = (int *)arg;
//     while (true)
//     {
//         pthread_mutex_lock(&mutex);
//         while (Q.size() >= MAXSIZE)
//             pthread_cond_wait(&cond, &mutex);
//         int element = rand() % 100;
//         Q.push(element);
//         cout << "producer id : " << *id << " number = " << element << "  queue szie = " << Q.size() << endl;
//         pthread_mutex_unlock(&mutex);

//         pthread_cond_broadcast(&cond);

//         usleep(rand() % 3);
//     }
//     return NULL;
// }
// void *consumerF(void *arg)
// {
//     int *id = (int *)arg;
//     while (true)
//     {
//         // queue 头部内容
//         pthread_mutex_lock(&mutex);
//         while (Q.empty())
//             pthread_cond_wait(&cond, &mutex); // 注意cond条件的位置
//         cout << "consumer id : " << *id << " number = " << Q.front() << "  queue szie = " << Q.size() << endl;
//         Q.pop();
//         pthread_mutex_unlock(&mutex);
//         pthread_cond_broadcast(&cond);
//         sleep(rand() % 3);
//     }
//     return NULL;
// }
// int main(int argc, char *argv[])
// {
//     pthread_mutex_init(&mutex, NULL);
//     pthread_cond_init(&cond, NULL);
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
//     pthread_cond_destroy(&cond);
//     return 0;
// }