#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
// #include <pthread.h>
#include <thread>
#include <string>
#include <iostream>
using namespace std;
struct Person
{
    int m_Num;
    int m_Age;
    Person(){};
    Person(int num, int age) : m_Num(num), m_Age(age)
    {
    }
};

void *callBack(void *arg)
{
    for (int i = 0; i < 10; i++)
    {
        cout << "子线程 i = " << i << endl;
    }
    cout << "子线程：" << pthread_self() << endl;
    Person *thisp = (Person *)arg;
    thisp->m_Age = 10;
    thisp->m_Num = 12;
    pthread_exit(thisp); // 线程结束抛出
    return NULL;
}
int main()
{
    cout << "主线程id = " << pthread_self() << endl;
    pthread_t tid;

    Person p1;
    pthread_create(&tid, NULL, callBack, &p1); // 程序执行至子线程时，调用callBack函数执行子线程任务
    pthread_cancel(tid);  // 终结子线程
    pthread_detach(tid);                       //  使子线程与主线程分离
    // for(int i = 0;i<5;i++)
    // {
    //     cout<<"主线程 i = "<<i<<endl;
    // }
    pthread_exit(NULL);
    return 0;
}