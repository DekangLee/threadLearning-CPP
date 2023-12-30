#include <iostream>
#include <thread>
#include <semaphore.h>
#include <unistd.h>
using namespace std;
// 定义
void *worker(void *arg)
{
    sleep(3);
    // pthread_exit(NULL);
    return NULL;
}
int main()
{
    // int *num;  // 指针初始化内容为NULL
    // if (num == NULL)
    // {
    //     cout << " num == nullptr" << endl;
    // }
    // else
    // {
    //     cout << "num != nullptr" << endl;
    // }
    // int **a = new int *;
    // int b = 10;
    // *a = &b;
    // cout << **a << endl;
    // cout<<"*a size = "<<sizeof(*a)<<endl;
    // int*(*f)() = nullptr;
    // 函数指针字节为8，就是一般的指针，只是解释时
    // cout<<sizeof(f)<<endl;
    pthread_t tid;
    pthread_create(&tid,NULL,worker,NULL);
    cout<<tid<<endl;
    pthread_join(tid,NULL);
    cout<<tid<<endl;
    return 0;
}