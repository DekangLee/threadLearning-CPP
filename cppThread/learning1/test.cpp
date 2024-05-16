// #include <iostream>
// #include <thread>
// #include <vector>
// // #include<bits/stdc++.h>
// class String
// {
// public:
//     String(const char *cstr) { std::cout << "String(const char* cstr)" << std::endl; }

//     // 1
//     String(const String &v)
//     {
//         std::cout << "String(const String& v)" << std::endl;
//     }

//     // 2
//     String(const String &&v) noexcept
//     {
//         std::cout << "String(const String&& v)" << std::endl;
//     }

//     // 3
//     String &operator=(const String &v)
//     {
//         std::cout << "String& operator=(const String& v)" << std::endl;
//         return *this;
//     }
// };

// void test(int i, const String &s) {}
// // class B
// // {
// // public:
// //     int m_b;
// //     B(int b) : m_b(b) { std::cout << "B的有参构造" << std::endl; }
// //     B(const B &b) : m_b(b.m_b) { std::cout << "B拷贝构造" << std::endl; }
// //     B(B &&b) noexcept : m_b(b.m_b) { std::cout << "B移动构造" << std::endl; }
// // };
// class B
// {
// public:
//     B(int b)
//     {
//         this->num = new int(b);
//         std::cout << "B的有参构造" << std::endl;
//     }
//     B(const B &b)
//     {
//         this->num = new int(*b.num);
//         std::cout << "B拷贝构造" << std::endl;
//     }
//     B(B &&b) noexcept
//     {
//         this->num = b.num;
//         b.num = nullptr;
//         std::cout << "B移动构造" << std::endl;
//     }
//     ~B()
//     {
//         if (this->num != nullptr)
//         {
//             delete this->num;
//             this->num = nullptr;
//         }
//     }
//     int *num;
// };
// B f()
// {
//     B b(2);
//     return b;
// }
// int main()
// {
//     //   String s("hello");
//     //   std::cout << "----------------" << std::endl;

//     //   // 输出 1, 2
//     //   std::thread t1(test, 3, s);
//     //   t1.join();
//     //   std::cout << "----------------" << std::endl;

//     //   // 输出 2, 2
//     //   std::thread t2(test, 3, std::move(s));
//     //   t2.join();
//     //   std::cout << "----------------" << std::endl;

//     //   // 只输出 0
//     //   std::thread t3(test, 3, "hello");
//     //   t3.join();
//     //   std::cout << "----------------" << std::endl;

//     //   // 无输出
//     //   std::thread t4(test, 3, std::ref(s));
//     //   std::cout << "----------------" << std::endl;
//     //   t4.join();
//     // int a = 10;
//     // B b = f();
//     // std::vector<int> v1;
//     // v1.push_back(1);
//     // v1.push_back(2);
//     // std::vector<int> &v2 = v1;

//     // std::cout << "v1 addr " << &v1[0] << std::endl;
//     // std::cout << "v2 addr " << &v2[0] << std::endl;
//     // std::vector<int> v3(std::move(v1));
//     // std::cout << "v3 addr " << &v3[0] << std::endl;
//     B b1(20);
//     B &&b2 = std::move(b1);  // b2的绑定方式是右值引用，与b1的地址相同，但是b2是左值
//     B &b3 = b2; // 这就是b2是左值的证据
//     // B b3(std::move(b1));
//     // *b3.num = 10;
//     std::cout << "v1 addr " << &b1 << std::endl;
//     std::cout << "v2 addr " << &b2 << std::endl;
//     std::cout << "v2 addr " << &b3 << std::endl;
//     // std::cout << "v3 addr " << b3.num << std::endl;
//     // int a = 10;
//     // int &&b = std::move(a);
//     // b = 20;
//     // a = 30;
//     // std::cout << &a << std::endl;
//     // std::cout << &b << std::endl;
//     return 0;
// }