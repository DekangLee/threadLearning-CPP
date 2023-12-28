// #include <stdio.h>
// #include <string.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <netdb.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <pthread.h>
// #include <string>
// #include <iostream>
// using namespace std;
// struct Person
// {
//     int m_Num;
//     int m_Age;
//     Person(){};
//     Person(int num, int age) : m_Num(num), m_Age(age)
//     {
//     }
// };

// void *callBack(void *arg)
// {
//     for (int i = 0; i < 10; i++)
//     {
//         cout << "子线程 i = " << i << endl;
//     }
//     cout << "子线程：" << pthread_self() << endl;
//     // Person *p1 = new Person(190,16);
//     Person *thisp = (Person *)arg;
//     thisp->m_Age = 10;
//     thisp->m_Num = 12;
//     pthread_exit(thisp); // 线程结束抛出
//     return NULL;
// }
// int main()
// {
//     pthread_t tid;

//     Person p1;
//     pthread_create(&tid, NULL, callBack, &p1); // 程序执行至子线程时，调用callBack函数执行子线程任务
//     // sleep(1); // 相当于有限执行子线程任务，主线程放弃1s钟时间
//     // for (int i = 0; i < 5; i++)
//     // {
//     //     cout << "主线程 i = " << i << endl;
//     // }
//     // cout << "主线程：" << pthread_self() << endl;
//     void *acp;
//     // pthread_join(tid, &acp); // acp为指向子线程任务结束后抛出的堆地址
//     // Person *realAcp = (Person *)acp;
//     // cout << "num = " << realAcp->m_Num << " age = " << realAcp->m_Age << endl;
//     pthread_join(tid, NULL);
//     cout << "num = " << p1.m_Num << " age = " << p1.m_Age << endl;
//     pthread_exit(NULL);

//     return 0;
// }