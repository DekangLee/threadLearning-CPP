#include <iostream>
#include <string>
#include <thread>
#include <vector>
using namespace std;
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
    const int &var3 = var2;
    char myBuff[] = "this is a test";
    // thread mythread(myprint, var2, myBuff);
    // mythread.join();
    // cout << "I love China !" << endl;
    return 0;
}