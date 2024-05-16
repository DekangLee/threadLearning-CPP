// #include <iostream>
// #include <thread>
// #include <mutex>
// #include <future>
// using namespace std;
// class A
// {
// public:
//     int mythread(int par)
//     {
//         cout << "thread:" << this_thread::get_id() << " start!" << endl;
//         cout << "par = " << par << endl;
//         chrono::milliseconds dura(500);
//         this_thread::sleep_for(dura);
//         return par;
//     }
// };
// int mythread1(int p)
// {
//     cout << "thread:" << this_thread::get_id() << " start!" << endl;
//     chrono::milliseconds dura(5000);
//     this_thread::sleep_for(dura);
//     return 5;
// }
// void mythread(std::promise<int> &pro, int a, int b)
// {
//     int res = a + b;
//     chrono::milliseconds dura(200);
//     this_thread::sleep_for(dura);
//     pro.set_value(res);
// }

// void mythread2(std::shared_future<int> fut)
// {
//     cout << "res1 = " << fut.get() << endl;
//     return;
// }
// void mythread3(std::shared_future<int> fut)
// {
//     cout << "res2 = " << fut.get() << endl;
//     return;
// }
// static void f() {}
// int main()
// {
//     cout << "main thread id:" << this_thread::get_id() << " start!" << endl;

//     // // std::future<int> result = std::async(mythread);
//     // A a;
//     // std::future<int> result = std::async(std::launch::deferred,&A::mythread, &a, 2);
//     // std::future_status status = result.wait_for(std::chrono::seconds(1));
//     // if (status == std::future_status::timeout)
//     // {
//     //     // 线程运行超时，表示线程没有执行完成
//     //     cout << "线程执行超时" << endl;
//     // }
//     // else if (status == std::future_status::ready)
//     // {
//     //     // 线程成功返回
//     //     cout << "线程执行完毕" << endl;
//     // }
//     // else if (status == std::future_status::deferred)
//     // {
//     //     cout << "线程未开始执行" << endl;
//     // }
//     // // std::future<int> result = std::async(std::launch::deferred, &A::mythread, &a, 2);
//     // std::future<int> result = std::async(std::launch::async, &A::mythread, &a, 2);
//     // int def = 0;
//     // def = 5;
//     // cout << "continue.........." << endl;
//     // cout << "result = " << result.get() << endl; // 阻塞等待mythread执行完毕,get函数只能调用一次
//     // // result.wait();

//     // std::packaged_task<int(int)> mpt(mythread);
//     // thread t1(std::ref(mpt), 1);
//     // t1.detach();
//     // future<int> result = mpt.get_future(); // 这使得future里面包含了线程入口函数的执行结果
//     // cout << result.get() << endl;
//     // // t1.join();
//     // cout << "I love China!" << endl;
//     // std::packaged_task<int(int)> mpt(mythread1);
//     // mpt(100);                              // packaged_task的对象可以直接调用，其实就是函数调用
//     // future<int> result = mpt.get_future(); // 这使得future里面包含了线程入口函数的执行结果
//     // cout << result.get() << endl;
//     // 还需说明的是packaged_task的拷贝函数被禁用，是禁止拷贝的，可以使用移动语义

//     // share_future
//     std::promise<int> prom;
//     thread t1(mythread, std::ref(prom), 2, 3);
//     t1.detach();
//     // 获取结果值
//     future<int> res = prom.get_future();
//     shared_future<int> share_res(std::move(res)); // 这里使用的是移动语义，将future的管理对象移动到share_future
//     // cout << "res = " << res.get() << endl;
//     // future<int> res;
//     thread t2(mythread2, std::ref(share_res));
//     thread t3(mythread3, std::ref(share_res));
//     t2.join();
//     t3.join();
//     return 0;
// }