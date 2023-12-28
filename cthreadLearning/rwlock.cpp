// // 该例子中有8个子线程，其中3个进行对临界区的write操作，其余5个只进行read操作
// #include <stdio.h>
// #include <iostream>
// #include <pthread.h>
// #include <unistd.h>
// using namespace std;
// const int MAXSIZE = 50;
// const int max = 0;
// int number = 0;
// pthread_rwlock_t rwLock;
// void *writeNum(void *arg)
// {
//     int *n = (int *)arg;
//     for (int i = 0; i < MAXSIZE; i++)
//     {
//         pthread_rwlock_wrlock(&rwLock);
//         int cur = number;
//         cur++;
//         number = cur;
//         cout << "thread :" << *n << "  write number = " << number << endl;
//         pthread_rwlock_unlock(&rwLock);
//         usleep(rand()%5);
//     }
//     return NULL;
// }
// void *readNum(void *arg)
// {
//     int *n = (int *)arg;
//     for (int i = 0; i < MAXSIZE; i++)
//     {
//         pthread_rwlock_rdlock(&rwLock);
//         cout << "thread :" << *n;
//         cout << "  read number = " << number << endl;
//         pthread_rwlock_unlock(&rwLock);
//         usleep(rand()%5);
//     }
//     return NULL;
// }
// int main(int argc, char *argv[])
// {
//     pthread_rwlock_init(&rwLock, NULL);
//     pthread_t tid[8];
//     for (int i = 0; i < 8; i++)
//     {
//         if (i < 3)
//             pthread_create(tid + i, NULL, writeNum, &i);
//         else
//             pthread_create(tid + i, NULL, readNum, &i);
//     }
//     for (int i = 0; i < 8; i++)
//         pthread_join(tid[i], NULL);
//     cout << "End number = " << number << endl;
//     pthread_rwlock_destroy(&rwLock);
//     return 0;
// }