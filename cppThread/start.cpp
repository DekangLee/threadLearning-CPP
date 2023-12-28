#include <iostream>
// 包括标准库中对多线程支持的声明，管理线程的函数和类
#include <thread>
using namespace std;
void hello()
{
    cout << "This is my first thread" << endl;
    for (int i = 0; i < 100000; i++)
    {
        cout << "thread hello i = " << i << endl;
    }
}
void doSomeWork(int i)
{
    cout << "do some work " << i << endl;
}
void doSomeWorkElse()
{
    cout << "do some work else" << endl;
}
class backGround
{
public:
    void operator()() const
    {
        doSomeWork(1);
        doSomeWorkElse();
    }
};
class fun
{
public:
    fun(int _i) : i(_i)
    {
    }
    void operator()()
    {
        cout << "func thread start" << endl;
        thread t(hello);
        t.detach();
        // for (int j = 0; j < 200000000; j++)
        // {
        //     cout << "fun "<<i << endl;
        //     doSomeWork(i);
        // }
        cout << "fun thread finished" << endl;
        if(t.joinable())
        {
            t.join();
        }
        else
        {
            cout<<"t can't join "<<endl;
        }
    }
    int i;
};
int main()
{

    // backGround task1;
    // // thread thread1(task1); // 第一种声明 实例化对象传入thread构造函数
    // // thread thread1{backGround()}; // 第二种声明使用匿名对象，但是要避免语法解析的错误必须使用{}
    // thread thread1([]
    //                {doSomeWork(1);
    //                doSomeWorkElse(); }); // 使用lamada表达式捕获，创建临时函数
    //                      // 此时线程已经启动
    int local = 1;
    thread thread2{fun(local)};
    // thread2.detach();
    // thread2.join();
    // thread1.join();
    return 0;
}