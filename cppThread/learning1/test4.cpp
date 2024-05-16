// #include <iostream>
// #include <vector>
// #include <thread>
// #include <list>
// #include <mutex>
// #include <unistd.h>
// using namespace std;
// class A
// {
// public:
//     // 定义成员函数作为线程入口函数
//     void inMsgQueue()
//     {
//         for (int i = 0; i < 200000; ++i)
//         {
//             // myMutex1.lock();
//             // std::unique_lock<mutex> guard1(this->myMutex1,std::adopt_lock);
//             // myMutex1.lock();
//             // std::unique_lock<mutex> guard1(this->myMutex1, std::try_to_lock);
//             // if (guard1.owns_lock())
//             // {
//             //     // 拿到锁
//             //     cout << "inMsgQueue()执行，插入命令:" << i << endl;
//             //     this->msgRcvQueue.push_back(i);
//             // }
//             // else
//             // {
//             //     // 未拿到锁
//             //     cout << "inMsgQueue()执行，未拿到锁" << endl;
//             // }
//             // cout << "inMsgQueue()执行，插入命令:" << i << endl;
//             // this->msgRcvQueue.push_back(i);
//             // myMutex1.unlock();
//             // std::unique_lock<mutex> guard1(this->myMutex1, std::defer_lock); // 没有加锁的mutex
//             // // 如果需要处理共享代码
//             // guard1.lock();
//             // // 共享代码

//             // // 如果处理非共享代码
//             // guard1.unlock();
//             // // 非共享代码

//             // guard1.lock();
//             // cout << "inMsgQueue()执行，插入命令:" << i << endl;
//             // this->msgRcvQueue.push_back(i);
//             // if (guard1.try_lock())
//             // {
//             //     // 拿到锁
//             //     cout << "inMsgQueue()执行，插入命令:" << i << endl;
//             //     this->msgRcvQueue.push_back(i);
//             // }
//             // else
//             // {
//             //     cout << "inMsgQueue()执行，未拿到锁" << endl;
//             // }
//             // std::unique_lock<mutex> guard1(myMutex1);
//             // mutex *pMutex1 = guard1.release();
//             // pMutex1->unlock();
//             // guard1.lock();
//             std::unique_lock<mutex> guard1(myMutex1);
//             std::unique_lock<mutex> guard2(std::move(guard1));
            
//         }
//     }
//     bool outMsgLULProc(int &i)
//     {
//         // std::unique_lock<mutex> guard1(this->myMutex1);
//         this->myMutex1.lock();
//         if (!this->msgRcvQueue.empty())
//         {
//             i = this->msgRcvQueue.front();
//             this->msgRcvQueue.pop_front();
//             this->myMutex1.unlock();
//             // this->myMutex2.unlock();
//             return true;
//         }
//         this->myMutex1.unlock();
//         // this->myMutex2.unlock();
//         return false;
//     }
//     void outMsgQueue()
//     {
//         int command = 0;
//         for (int i = 0; i < 100000; ++i)
//         {
//             bool result = outMsgLULProc(command);
//             if (result == true)
//             {
//                 cout << "outMsgQueue()执行，取出命令command：" << command << endl;
//             }
//             else
//             {
//                 cout << "outMsgQueue()执行，队列为空" << endl;
//             }
//         }
//         cout << "end" << endl;
//     }

// private:
//     list<int> msgRcvQueue; // 使用stl的容器list构建一个消息队列
//     mutex myMutex1;        // 声明一个互斥量
//     mutex myMutex2;
// };
// int main()
// {
//     A myobj;
//     thread inMyObj(&A::inMsgQueue, &myobj);
//     thread outMyObj(&A::outMsgQueue, &myobj);

//     inMyObj.join();
//     outMyObj.join();
//     return 0;
// }