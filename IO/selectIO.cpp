#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <map>
using namespace std;
int main()
{
    // 1.创建用于监听的文文件描述符
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(10246);
    saddr.sin_addr.s_addr = inet_addr("172.18.69.25");
    int ret = bind(lfd, (sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        perror("bind");
        return 1;
    }
    listen(lfd, 128);
    fd_set readSet; // 创建用于读的文件描述符集合
    FD_ZERO(&readSet);
    FD_SET(lfd, &readSet);
    int maxFD = lfd;
    map<int, sockaddr_in> fdSOCK;
    while (true)
    {
        fd_set temp = readSet;
        int ret = select(maxFD + 1, &temp, NULL, NULL, NULL);
        // 判断是否为监听的文件描述符
        if (FD_ISSET(lfd, &temp))
        {
            // 监听文件描述符为就绪状态，说明有新连接
            sockaddr_in caddr;
            socklen_t lenAddr = sizeof(caddr);
            int cfd = accept(lfd, (sockaddr *)&caddr, &lenAddr);
            FD_SET(cfd, &readSet);
            fdSOCK.insert(make_pair(cfd, caddr));
            maxFD = maxFD > cfd ? maxFD : cfd;
        }
        for (int i = 0; i <= maxFD; ++i)
        {
            if (i != lfd && FD_ISSET(i, &temp))
            {
                // 该文件描述符并非监听文件描述符，而是通信文件描述符
                // 接收数据
                char buff[1024];
                int len = recv(i, buff, sizeof(buff), 0);
                if (len == 0)
                {
                    // 客户端断开连接
                    cout << "客户端 IP:" << inet_ntoa(fdSOCK[i].sin_addr) << " port:" << fdSOCK[i].sin_port << " 已断开连接....." << endl;
                    FD_CLR(i, &readSet); // 将之前产生的文件描述符从readSet中移除
                    close(i);
                }
                else if (len == -1)
                {
                    perror("recive");
                    continue;
                }
                else
                {
                    // 连接成功功能区
                    cout << "From IP:" << inet_ntoa(fdSOCK[i].sin_addr) << " revive: " << buff << endl;
                    for (int i = 0; i < len; ++i)
                    {
                        buff[i] = toupper(buff[i]);
                    }
                    ret = send(i, buff, strlen(buff) + 1, 0);
                    if (ret == -1)
                    {
                        perror("send");
                        exit(1);
                    }
                }
            }
        }
    }
    close(lfd);
    return 0;
}