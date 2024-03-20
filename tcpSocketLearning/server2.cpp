// #include <unistd.h>
// // #include <sys/socket.h>
// #include <string.h>
// #include <arpa/inet.h>
// // #include <netinet/in.h>
// #include <iostream>

// using namespace std;
// int main()
// {
//     // 1.创建用于监听的套接字
//     // int lfd = socket()
//     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd == -1)
//     {
//         perror("socket");
//         return -1;
//     }
//     // 2.绑定本地的IP port
//     struct sockaddr_in saddr;
//     saddr.sin_family = AF_INET;
//     saddr.sin_port = htons(10234);
//     // saddr.sin_addr.s_addr = inet_addr("172.18.69.25");
//     saddr.sin_addr.s_addr = INADDR_ANY;

//     // ret 为绑定是否成功的标志
//     int ret = bind(sockfd, (sockaddr *)(&saddr), sizeof(saddr));
//     if (ret == -1)
//     {
//         perror("bind");
//         return -1;
//     }
//     ret = listen(sockfd, 128);
//     if (ret == -1)
//     {
//         perror("listen");
//         return -1;
//     }
//     // 3.阻塞并等待客户端连接
//     sockaddr_in caddr;
//     socklen_t addrlen = sizeof(caddr);
//     cout << "等待客户端连接....." << endl;
//     int cfd = accept(sockfd, (sockaddr *)(&caddr), &addrlen);
//     if (cfd == -1)
//     {
//         perror("accept");
//         return -1;
//     }
//     printf("accept sucessful! IP:%s port:%d cfd:%d\n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port), cfd);
//     // 4.通信
//     while (true)
//     {
//         // 接收数据
//         char buff[1024];
//         int len = recv(cfd, buff, sizeof(buff), 0);
//         if (len == -1)
//         {
//             perror("recv");
//             break;
//         }
//         else if (len == 0)
//         {
//             cout << "客户端已经断开连接" << endl;
//             break;
//         }
//         else
//         {
//             cout << "recive:" << buff << endl;
//             send(cfd, buff, sizeof(buff), 0);
//             memset(buff, 0, sizeof(buff));
//         }
//     }
//     close(sockfd);
//     close(cfd);
//     return 0;
// }