// a.cpp
#include "a.h"
extern char str2[];
namespace sextern1
{
    void Fun1()
    {
        str1[0] = 'l';
        str2[0] = '3';
    }
    void print1()
    {
        cout << "str1 = " << str1 << endl;
        cout << "str1 address  " << &str1 << endl;
        cout << "str2 = " << str2 << endl;
        cout << "str2 address  " << &str2 << endl;
    }
}