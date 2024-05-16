// #include <iostream>
// #include <thread>
// #include <mutex>
// #include <future>
// #include<atomic>
// using namespace std;
// std::mutex mVar;
// // int shareVar = 0;
// std::atomic<int> shareVar(0);
// // void wThread()
// // {
// //     for (int i = 0; i < 100000; ++i)
// //     {
// //         mVar.lock();
// //         ++shareVar;
// //         mVar.unlock();
// //     }
// //     return;
// // }
// void wThread()
// {
//     for (int i = 0; i < 100000; ++i)
//     {
//         ++shareVar;
//     }
//     return;
// }
// int main()
// {
//     thread t1(wThread);
//     thread t2(wThread);
//     t1.join();
//     t2.join();
//     cout << "shareVar = " << shareVar << endl;
//     return 0;
// }
#include <future>
#include <iostream>
#include <thread>
int main()
{
    std::promise<bool> prom;

    std::cout << std::boolalpha;
    std::future<bool> f = prom.get_future();
    prom.set_value(true);
    std::chrono::seconds dura(3);
    // std::this_thread::sleep_for(dura);
    f.wait();
    std::cout << f.get() << std::endl;
    return 0;
}