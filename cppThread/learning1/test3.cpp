// #include <iostream>
// #include <vector>
// #include <thread>
// #include <list>
// #include <mutex>
// #include <unistd.h>
// using namespace std;
// vector<int> g_v = {1, 2, 3}; // 全局数据，共享数据
// // 创建线程入口函数
// void myprint(int num)
// {

//     cout << "线程开始执行，编号" << num << endl;
//     // cout << "线程id:" << this_thread::get_id() << endl;
//     // cout << "线程执行结束，编号" << num << endl;
//     sleep(1);
//     cout << "id=" << this_thread::get_id() << " g_v:" << g_v[0] << g_v[1] << g_v[2] << endl;
// }
// class A
// {
// public:
//     A()
//     {
//         this->msgRcvQueue.push_back(0);
//     }
//     // 定义成员函数作为线程入口函数
//     void inMsgQueue()
//     {
//         for (int i = 0; i < 100; ++i)
//         {

//             this->myMutex1.lock();
//             // this->myMutex2.lock();
//             // std::lock(myMutex1,myMutex2);
//             // this->myMutex1.lock();
//             int p = this->msgRcvQueue.back();
//             cout << "inMsgQueue()执行，插入命令:" << p + 1 << endl;
//             this->msgRcvQueue.push_back(p + 1);
//             // this->myMutex2.unlock();
//             this->myMutex1.unlock();
//         }
//     }
//     bool outMsgLULProc(int &i)
//     {
//         // this->myMutex1.lock();
//         // this->myMutex2.lock();
//         // std::lock(myMutex2,myMutex1);
//         // lock_guard<mutex> guard1(myMutex1,std::adopt_lock);
//         // lock_guard<mutex> guard2(myMutex2,std::adopt_lock);
//         std::chrono::milliseconds dur(2);
//         std::this_thread::sleep_for(dur);
//         // this->myMutex1.lock();
//         if (!this->msgRcvQueue.empty())
//         {
//             i = this->msgRcvQueue.front();
//             this->msgRcvQueue.pop_front();
//             // this->myMutex1.unlock();
//             // this->myMutex2.unlock();
//             return true;
//         }
//         // this->myMutex1.unlock();
//         // this->myMutex2.unlock();
//         return false;
//     }
//     void outMsgQueue()
//     {
//         int command = 0;
//         for (int i = 0; i < 10; ++i)
//         {
            
//             bool result = outMsgLULProc(command);
//             if (result == true)
//             {
//                 cout << "outMsgQueue()执行，取出命令:" << command << endl;
//             }
//             else
//             {
//                 cout << "outMsgQueue()执行，任务队列为空" << endl;
//             }
//         }
//         cout << "end" << endl;
//     }
//     ~A()
//     {
//         cout << "fornt:" << msgRcvQueue.front() << " back:" << msgRcvQueue.back() << endl;
//         cout << "A的析构:";
//         for (auto it = msgRcvQueue.begin(); it != msgRcvQueue.end(); ++it)
//         {
//             cout << *it << " ";
//         }
//         cout << endl;
//     }

// private:
//     list<int> msgRcvQueue; // 使用stl的容器list构建一个消息队列
//     mutex myMutex1;        // 声明一个互斥量
//     mutex myMutex2;
// };
// int main()
// {
//     // // 创建多个线程
//     // vector<thread> mythreads;
//     // for (int i = 0; i < 10; ++i)
//     // {
//     //     // 线程创建开始执行
//     //     mythreads.push_back(thread(myprint, i));
//     // }
//     // for (auto it = mythreads.begin(); it != mythreads.end(); ++it)
//     // {
//     //     // 阻塞主线程，等待所有线程执行完毕
//     //     it->join();
//     // }
//     // cout << "I love China!" << endl;
//     A myobj;
//     thread inMyObj1(&A::inMsgQueue, &myobj);
//     thread inMyObj2(&A::inMsgQueue, &myobj);
//     thread inMyObj3(&A::inMsgQueue, &myobj);
//     // thread outMyObj(&A::outMsgQueue, &myobj);
//     inMyObj1.join();
//     inMyObj2.join();
//     inMyObj3.join();
//     // outMyObj.join();
//     return 0;
// }