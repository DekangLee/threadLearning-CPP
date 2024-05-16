// #include <iostream>
// #include <vector>
// #include <thread>
// #include <list>
// #include <mutex>
// #include <condition_variable>
// #include <unistd.h>
// using namespace std;
// class A
// {
// public:
//     // 定义成员函数作为线程入口函数
//     void inMsgQueue()
//     {
//         for (int i = 0; i < 100000; ++i)
//         {
//             std::unique_lock<mutex> guard1(this->myMutex1, std::defer_lock); // 没有加锁的mutex
//             guard1.lock();
//             cout << "向任务队列插入command:" << i << endl;
//             msgRcvQueue.push_back(i);
//             my_cond.notify_all(); // 唤醒被条件变量阻塞的线程
//             guard1.unlock();      // 唤醒其他线程后及时释放互斥量
//         }
//     }
//     bool outMsgLULProc(int &i)
//     {
//         // std::unique_lock<mutex> guard1(this->myMutex1);
//         if (!this->msgRcvQueue.empty()) // 双重锁定，双重检查
//         {
//             this->myMutex1.lock();
//             if (!this->msgRcvQueue.empty())
//             {
//                 i = this->msgRcvQueue.front();
//                 this->msgRcvQueue.pop_front();
//                 this->myMutex1.unlock();
//                 // this->myMutex2.unlock();
//                 return true;
//             }
//             this->myMutex1.unlock();
//             // this->myMutex2.unlock();
//         }
//         return false;
//     }

//     void outMsgQueue()
//     {
//         int command = 0;
//         while (true)
//         {
//             std::unique_lock<mutex> guard(myMutex1);
//             // wait是条件变量的一个成员函数，如果第二个参数的返回值是false，
//             // 该函数会将互斥量解锁(主动放弃互斥量)，并开始阻塞，直到其他线程调用notify_one将该线程解除阻塞
//             // 如果wait没有第二个参数，会默认第二个参数返回false
//             // 阻塞被唤醒的wait会不断地尝试获取互斥量，如果不能立即获取互斥量会一直重复下去获取互斥量
//             // 当获取互斥量后，wait尝试判断第二参数的谓词是否为true，如果是false将会主动放弃互斥量
//             my_cond.wait(guard, [this]
//                          {
//                 if(!this->msgRcvQueue.empty())
//                     return true;
//                 else
//                     return false; });
//             // 当程序走到当前位置，一定获取了互斥量，并且任务队列中至少有一个命令
//             command = this->msgRcvQueue.front();
//             msgRcvQueue.pop_front();
//             cout << "id:" << this_thread::get_id() << "  outMsgQueue()执行成功，取出command:" << command << endl;
//             guard.unlock();
//         }
//     }

// private:
//     list<int> msgRcvQueue; // 使用stl的容器list构建一个消息队列
//     mutex myMutex1;        // 声明一个互斥量
//     mutex myMutex2;
//     std::condition_variable my_cond; // 声明一个条件变量
// };
// int main()
// {
//     A myobj;
//     thread inMyObj(&A::inMsgQueue, &myobj);
//     thread outMyObj(&A::outMsgQueue, &myobj);
//     thread outMyObj1(&A::outMsgQueue, &myobj);
//     inMyObj.join();
//     outMyObj.join();
//     outMyObj1.join();
//     return 0;
// }