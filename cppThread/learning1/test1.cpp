// #include <iostream>
// #include <thread>
// #include <time.h>
// #include <unistd.h>
// using namespace std;
// void doSomeWork()
// {
//     cout << "线程1开始运################################################################行1" << endl;
//     sleep(1);
//     cout << "线程1开始运行2" << endl;
//     sleep(1);
//     cout << "线程1开始运行3" << endl;
//     sleep(1);
//     cout << "线程1执行结束" << endl;
// }
// void doSomeWork1(int a)
// {
//     cout << "arg :" << a << endl;
// }
// class backGround
// {
// public:
//     backGround(int &i) : m_i(i), a(10)
//     {
//         cout << "backGround 有参构造函数" << endl;
//     }
//     backGround(const backGround &t) : m_i(t.m_i), a(t.a)
//     {
//         cout << "backGround 拷贝构造函数" << endl;
//     }
//     ~backGround()
//     {
//         cout << "backGround 析构函数" << endl;
//     }
//     void operator()()
//     {
//         cout << "m_i = " << m_i << endl;
//         cout << "m_i = " << m_i << endl;
//         cout << "m_i = " << m_i << endl;
//         cout << "m_i = " << m_i << endl;
//     }
//     int &m_i;
//     const int a;
// };
// int main()
// {
//     // std::thread my_thread1(doSomeWork); // 线程建立，也是线程的入口，这时线程开始执行
//     int a = 10;
//     // backGround f(a);
//     // thread my_thread2(f);
//     // cout << st d::thread::hardware_concurrency() << endl;
//     // my_thread2.detach();
//     // my_thread1.detach();
//     auto threadlam = [=]() -> void {
//         cout << a << endl;
//         cout << a << endl;
//         cout << a << endl;
//         cout << a << endl;
//         cout << "线程3开始执行" << endl;
//         cout << "线程3执行结束" << endl;
//     };
//     thread myThread3(threadlam);
//     myThread3.detach();
//     cout << "I love China" << endl;
//     return 0;
// }
