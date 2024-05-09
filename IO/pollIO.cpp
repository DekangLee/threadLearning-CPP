#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <arpa/inet.h>
#include <string.h>
#include <map>
using namespace std;
int main()
{
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr("172.18.69.25");
    saddr.sin_port = htons(10246);
    int ret = bind(lfd, (sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        cout << "绑定失败！" << endl;
        return 1;
    }
    listen(lfd, 128);
    pollfd *fdarr = new pollfd[1024];
    for (int i = 0; i <= 1024; ++i)
    {
        fdarr[i].fd = -1;
        fdarr[i].events = POLLIN;
    }
    fdarr[0].fd = lfd;
    int maxfd = 0;
    map<int, sockaddr_in> _map;
    while (true)
    {
        // 委托内核检测
        ret = poll(fdarr, maxfd + 1, -1);
        if (ret == -1)
        {
            perror("poll");
            continue;
        }
        // 检测的读缓冲区有变化，意味着有新连接
        if (fdarr[0].revents & POLLIN)
        {
            // 接收连接请求
            sockaddr_in caddr;
            socklen_t len = sizeof(caddr);
            int cfd = accept(lfd, (sockaddr *)&caddr, &len);
            int i;
            for (i = 1; i <= 1024; ++i)
            {
                if (fdarr[i].fd == -1)
                {
                    fdarr[i].fd = cfd;
                    _map[cfd] = caddr;
                    break;
                }
            }
            maxfd = maxfd > i ? maxfd : i;
        }
        // 通信如果有客户端发送过来
        for (int i = 1; i <= maxfd; ++i)
        {
            // 检测读缓冲区是否有数据
            if (fdarr[i].revents & POLLIN)
            {
                char buff[1024];
                int ret = recv(fdarr[i].fd, buff, sizeof(buff), 0);
                if (ret == 0)
                {
                    cout << "IP: " << inet_ntoa(_map[fdarr[i].fd].sin_addr) << "已断开连接...." << endl;
                    close(fdarr[i].fd);
                    fdarr[i].fd = -1;
                    continue;
                }
                else if (ret == -1)
                {
                    cout << "接收失败！" << endl;
                    continue;
                }
                else
                {

                    cout << "From IP: " << inet_ntoa(_map[fdarr[i].fd].sin_addr)
                         << " port: " << ntohs(_map[fdarr[i].fd].sin_port)
                         << " recive:" << buff << endl;
                    for (int i = 0; i < ret; ++i)
                    {
                        buff[i] = toupper(buff[i]);
                    }
                    send(fdarr[i].fd, buff, strlen(buff) + 1, 0);
                }
            }
        }
    }
    delete[] fdarr;
    close(lfd);
    return 0;
}