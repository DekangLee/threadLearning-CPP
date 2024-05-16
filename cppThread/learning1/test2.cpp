// #include <iostream>
// #include <thread>
// using namespace std;
// class A
// {
// public:
//     mutable int m_i;
//     A(int i) : m_i(i) { cout << "A的有参构造函数执行 " << this << " thread id = " << this_thread::get_id() << endl; }
//     A(const A &a) : m_i(a.m_i) { cout << "A的拷贝构造函数执行 " << this << " thread id = " << this_thread::get_id() << endl; }
//     A(const A &&a) : m_i(a.m_i)
//     {
//         cout << "A的移动构造函数执行 " << this << " thread id = " << this_thread::get_id() << endl;
//     }
//     ~A() { cout << "A的析构函数执行 " << this << " thread id = " << this_thread::get_id() << endl; }
//     void thread_work(int num)
//     {
//         cout << "thread id = " << this_thread::get_id() << endl;
//     }
// };
// // void myprint(const int &i, char *pmybuf)
// // void myprint(int i, const string &pmybuff)
// // {
// //     cout << "i = " << i << endl;
// //     cout << "buff addr= " << (void *)pmybuff.c_str() << endl;
// // }
// void myprint(int i, A a)
// {
//     a.m_i = 100;
//     cout << "a addr = " << &a << endl;
// }
// int main()
// {
//     // 临时对象作为线程参数
//     cout << "主线程id:" << this_thread::get_id() << endl;
//     int var1 = 1;
//     int &var2 = var1;
//     // char myBuff[] = "this is a test";
//     // cout << "myBuff addr = " << (void *)myBuff << endl;
//     thread mythread(myprint, var2, A(var1));
//     // A a(100);
//     // thread mythread(myprint, var2, std::ref(a));
//     // cout << "main thread a addr = " << &a << endl;
//     // cout << "子线程id:" << mythread.get_id() << endl;
//     // cout << "a.m_i = " << a.m_i << endl;
//     // 成员函数指针作为线程入口函数

//     // thread mythread2(&A::thread_work, a, 10);
//     // mythread2.join();
//     mythread.join();
//     // cout << "I love China !" << endl;
//     return 0;
// }