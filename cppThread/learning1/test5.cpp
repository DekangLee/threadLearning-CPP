// #include <iostream>
// #include <thread>
// #include <mutex>
// using namespace std;
// std::mutex resource_myCAS;
// std::once_flag g_flag;
// class myCAS // 单例类
// {
//     static void createInstance()
//     {
//         // std::chrono::milliseconds dura(20000);
//         // std::this_thread::sleep_for(dura);
//         cout << "进入createInstance()函数" << endl;
//         mInstance = new myCAS();
//         static Gar cl; // 定义
//     }

// private:
//     myCAS() {} // 构造函数私有化
// private:
//     static myCAS *mInstance; // 静态成员变量
// public:
//     static myCAS *getInstance()
//     {
//         // if (mInstance == nullptr) // 双重锁定，可以提高概率
//         // {
//         //     std::unique_lock<mutex> myCASLock(resource_myCAS);
//         //     if (mInstance == nullptr)
//         //     {
//         //         mInstance = new myCAS();
//         //         static Gar cl; // 定义
//         //     }
//         // }
//         std::call_once(g_flag, createInstance);
//         cout << "call_once执行完毕" << endl;
//         return mInstance;
//     }
//     class Gar // 生命用于析构的类
//     {
//     public:
//         ~Gar()
//         {
//             if (myCAS::mInstance != nullptr)
//             {
//                 delete myCAS::mInstance;
//                 myCAS::mInstance = nullptr;
//             }
//         }
//     };
//     void fun()
//     {
//         cout << "mInstance addr:" << myCAS::mInstance << endl;
//     }
// };
// myCAS *myCAS::mInstance = nullptr;
// class A
// {
// public:
//     static int a;
// };
// int A::a = 10;
// void myThread()
// {
//     myCAS *obj1 = myCAS::getInstance();
// }
// int main()
// {

//     std::thread thread1(myThread);
//     std::thread thread2(myThread);
//     thread1.join();
//     thread2.join();
//     return 0;
// }