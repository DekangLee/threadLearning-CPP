#include "b.h"
#include "a.h"
char str2[] = "world";
namespace sextern2
{
    void Fun2()
    {
        str1[0] = 'o';
        str2[0] = '2';
    }
    void print2()
    {
        cout << "str1 = " << str1 << endl;
        cout << "str1 address  " << &str1 << endl;
        cout << "str2 = " << str2 << endl;
        cout << "str2 address  " << &str2 << endl;
    }
}