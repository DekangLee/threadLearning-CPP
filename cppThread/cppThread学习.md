#### 1 并发基本概念与实现，进程、线程概念

##### 1.1并发

1、一个程序处理多个独立任务，以往的计算机在一个时间片内只能执行一个任务。有操作系统进行任务调度进行任务上下文切换，完成伪并发。这种任务切换是需要时间开销的，操作系统需要保存切换时的状态，执行进度等，复原时也需要时间消耗。
2、硬件的发展出现了处理器(多核)的处理器，可以实现真正的并行。并发的主要目的是提高性能。
3、可执行程序本质上是一个磁盘上的文件，可以直接被执行。可执行程序执行时会创建一个进程.
4、线程：每一进程均有一个主线程，主线程是唯一的。
5、多线程程序开发是实际的需要，开发过程也会有难度。
6、多进程并发：进程之间通信(同一个server上可以使用管道，文件，消息队列，共享内存)，不同的server之间的程序可以使用socket通信。
7、多线程并发：更像是轻量级的多进程并发，每个线程存在自己的独立运行路径。进程有自己的独立地址空间，进程中所有线程共享内存(全局变量、静态变量、堆内存)，多线程的开销远小于多进程的开销。
8、c++11的标准线程库：以往的windows与linux均使用不同的线程实现比如linux的"pthread.h"，编译的时候需要链接动态库。c++11开始在语言层面实现多线程，\<thread>可实现跨平台。

#### 2 线程的启动、结束，创建线程的方法

1、程序运行开始后建立一个进程，进程中的主线程随之建立并运行。主线程将mian函数执行完毕，进程也结束。这里需要注意的是进程是否执行完毕的标志是看主线程是否执行完毕。
2、一般情况下，主线程执行完毕，若其他子线程还未执行结束，会被操作系统强行终止。这意味着要保持子线程的运行必须保证主线程最后执行结束。

```cpp
#include <iostream>
#include <thread>
using namespace std;
void doSomeWork()
{
    cout << "doing some works" << endl;
}
class backGround
{
public:
    void operator()()
    {
       
    }
};
int main()
{
    std::thread my_thread1(doSomeWork);
    backGround f;
    thread my_thread2(f);
    cout << std::thread::hardware_concurrency() << endl;
    my_thread1.detach();
    my_thread2.join();
    return 0;
}
```

以上使用两种方式创建线程，一种是将函数名传入thread类的有参构造，另一种先实例化一个类的对象，再将该对象传入thread的有参构造。`detach`，与`join`不同之处在于，`join`会阻塞主线程，主线程等待my_thread2执行结束，一起退出。传统的多线程设计，主线程应该等待子线程执行结束，但c++11的新标准中，允许将主线程与子线程分离。调用detach之后子线程会与主线程失去联系，该线程会在后台执行，被c++运行时库接管，运行时库会清理该线程的资源。这里注意一但使用detach不能再使用join，反过来也一样join之后不能再detach。并且join与detach只能使用一次。
可以在使用join之前使用thread类的`joinable`函数，如果该线程可以join返回true，这样有效避免了错误写法。

```cpp
class backGround
{
public:
    backGround(int &i) : m_i(i),m_a(10){};
    void operator()()
    {
        cout << "m_i = " << m_i << endl;
        cout << "m_i = " << m_i << endl;
        cout << "m_i = " << m_i << endl;
        cout << "m_i = " << m_i << endl;
    }
    int &m_i;
    const int m_a;
};
int main()
{
    std::thread my_thread1(doSomeWork); // 线程建立，也是线程的入口，这时线程开始执行
    int a = 10;
    backGround f(a);
    thread my_thread2(f);
    // cout << std::thread::hardware_concurrency() << endl;
    my_thread2.detach();
    my_thread1.detach();
    cout << "I love China" << endl;
    return 0;
}
```

以上程序将background类增加两个成员变量，分别是一个引用`m_i`，一个常量`m_a`。在有参构造函数中初始化这两个成员变量。通过上面的程序可以看出对象f的成员变量`f.m_i`引用的是主线程的局部变量`a`，detach之后主线程一但执行完毕，变量`a`会被销毁，f的成员变量`m_i`引用内容被销毁，导致结果出错。这里还有一个注意点，`f`也是主线程在栈区创建的临时对象，主线程结束后`f`也会被销毁，那`my_thread2`执行会出错吗？事实上，不会的，因为`thread my_thread2(f)`这里`f`仅仅是一个参数，子线程会复制`f`的内容到子线程中。所以`f`中不能含有引用，指针等，否则可能会导致出错。除了使用函数与类创建线程，还可以使用lambda表达式，来创建线程。

```cpp
int main()
{
    int a = 10;
    
    auto threadlam = [&]() -> void {
        cout << a << endl;
        cout << a << endl;
        cout << a << endl;
        cout << a << endl;
        cout << "线程3开始执行" << endl;
        cout << "线程3执行结束" << endl;
    };
    thread myThread3(threadlam);
    myThread3.detach();
    cout << "I love China" << endl;
    return 0;
}
```

以上代码存在问题，`threadlam = [&]() -> void`以引用方式捕获变量，并且子线程会与主线程分离，一但主线程运行结束，`a`会被销毁，子线程中的`a`值会“不可预测”。

#### 3 线程传参，detach注意事项，成员函数做线程函数

```cpp
void myprint(const int &i, char *pmybuf)
{
    cout << "i = " << i << endl;
    cout << "buff = " << pmybuf << endl;
}
int main()
{
    // 临时对象作为线程参数
    int var1 = 1;
    int &var2 = var1;
    char myBuff[] = "this is a test";
    thread mythread(myprint, var2, myBuff);
    mythread.detach();
    cout << "I love China !" << endl;
    return 0;
}
```

以上是线程传参的写法，子线程中线程传参后，
