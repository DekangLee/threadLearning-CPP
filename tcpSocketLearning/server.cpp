#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
using namespace std;
#define MAXLINE 1024    // buff size
#define SERV_PORT 10231 // 服务器端口号
int main(void)
{
    struct sockaddr_in servaddr, cliaddr; // 定义服务器与客户端地址结构体
    socklen_t cliaddr_len;                // 客户端地址长度
    int listenfd, connfd;
    char rcvbuf[MAXLINE];
    char senbuf[MAXLINE];
    char str[INET_ADDRSTRLEN];
    int i, n;
    // 创建服务器端套接字文件
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    // 初始化服务器端口地址
    // bzero(&servaddr, sizeof(servaddr)); // 将服务器端口地址清零
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("172.18.69.25");
    servaddr.sin_port = htons(SERV_PORT);
    // 将套接字文件与服务器端口地址绑定
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    // 监听，并设置最大连接数为20
    listen(listenfd, 20);
    printf("Accepting connections ...\n");
    cliaddr_len = sizeof(cliaddr);
    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len); // 该返回值为新创建的用于新连接的socket
    printf("received from %s at PORT %d\n",
           inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
           ntohs(cliaddr.sin_port));
    // 接收客户端数据，并处理请求
    while (1)
    {
        memset(rcvbuf, 0, sizeof(rcvbuf));
        // printf("----------------");
        n = recv(connfd, rcvbuf, MAXLINE, 0);
        if (n == 0)
        {
            printf("客户端已断开连接\n");
            break;
        }
        printf("recive: %s\n", rcvbuf);
        cout << "Input what you want to say : ";
        cin >> senbuf;
        send(connfd, senbuf,sizeof(senbuf), 0);
        // 关闭连接
    }
    close(connfd);
    close(listenfd);
    return 0;
}