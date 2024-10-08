#include <unistd.h>
#include <vector>
// #include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
// #include <netinet/in.h>
#include <iostream>
#include <pthread.h>
using namespace std;
void *working(void *arg);
struct sockInfo
{
    sockaddr_in caddr;
    int cfd;
};
vector<sockInfo> infoArr;
int main()
{
    // 1.创建用于监听的套接字
    // int lfd = socket()
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket");
        return -1;
    }
    // 2.绑定本地的IP port
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(10234);
    // saddr.sin_addr.s_addr = inet_addr("172.18.69.25");
    saddr.sin_addr.s_addr = INADDR_ANY;

    // ret 为绑定是否成功的标志
    int ret = bind(sockfd, (sockaddr *)(&saddr), sizeof(saddr));
    if (ret == -1)
    {
        perror("bind");
        return -1;
    }
    ret = listen(sockfd, 128);
    if (ret == -1)
    {
        perror("listen");
        return -1;
    }
    infoArr.resize(512);
    for (auto &a : infoArr)
    {
        a.cfd = -1;
    }
    socklen_t addrlen = sizeof(sockaddr_in);
    // 3.阻塞并等待客户端连接
    while (true)
    {
        sockInfo sInfo;
        sockInfo *pSock;
        for (auto &a : infoArr)
        {
            if (a.cfd == -1)
            {
                pSock = &a;
            }
        }
        cout << "等待客户端连接....." << endl;
        pSock->cfd = accept(sockfd, (sockaddr *)(&(pSock->caddr)), &addrlen);
        if (pSock->cfd == -1)
        {
            perror("accept");
            continue;
        }
        pthread_t tid;
        pthread_create(&tid, NULL, working, pSock);
        pthread_detach(tid);
    }
    close(sockfd);
    return 0;
}
// 4.通信, 将通信功能写在子线程
void *working(void *arg)
{
    sockInfo *pSock = (sockInfo *)arg;
    printf("accept sucessful! IP:%s port:%d cfd:%d\n", inet_ntoa(pSock->caddr.sin_addr), ntohs(pSock->caddr.sin_port), pSock->cfd);
    while (true)
    {
        // 接收数据
        char buff[1024];
        int len = recv(pSock->cfd, buff, sizeof(buff), 0);
        if (len == -1)
        {
            perror("recv");
            break;
        }
        else if (len == 0)
        {
            cout << "客户端已经断开连接...." << endl;
            break;
        }
        else
        {
            cout << "recive:" << buff << endl;
            send(pSock->cfd, buff, sizeof(buff), 0);
            memset(buff, 0, sizeof(buff));
        }
    }
    close(pSock->cfd);
    pSock->cfd = -1;
    return NULL;
}