#include "a.h"
#include "b.h"
int main(int argc, char *argv[])
{

    sextern1::Fun1();
    sextern1::print1();
    sextern2::print2();
    sextern2::Fun2();
    sextern1::print1();
    // value  lello     // Func1更改了a.h头文件中的static str
    // address  0xaaaaaaab2010  str在源文件a.cpp中的地址和值
    // value  hello    // 这里是b.cpp对str的打印，发现值与初始化的一致并没有更改为lello
    // address  0xaaaaaaab2018 str在b.cpp中的打印地址结果，与a.cpp中不同，说明不同的源文件操作的静态变量并不是同一个
    // value  lello
    // address  0xaaaaaaab2010
    cout << a << endl;
    return 0;
}