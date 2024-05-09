#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <arpa/inet.h>
#include <string.h>
#include <map>
#include <sys/epoll.h>
#include <fcntl.h>
#include <error.h>
using namespace std;
map<int, sockaddr_in> _map;
struct sockInfo
{
    int epfd;
    int fd;
};
void *acceptFun(void *arg);
void *comFun(void *arg);
pthread_mutex_t mapMutex;
epoll_event evs[1024];
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
    int epfd = epoll_create(1);
    epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = lfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
    int size = sizeof(evs) / sizeof(epoll_event);
    pthread_mutex_init(&mapMutex, NULL);
    while (true)
    {
        int num = epoll_wait(epfd, evs, size, -1);
        for (int i = 0; i < num; ++i)
        {
            // 取出当前的文件描述符
            int curfd = evs[i].data.fd;
            sockInfo *info = new sockInfo;
            info->epfd = epfd;
            info->fd = curfd;
            pthread_t tid;
            // 判断该文件描述符是否用于监听
            if (curfd == lfd)
            {
                // 创建连接子线程
                pthread_create(&tid, NULL, acceptFun, info);
                pthread_detach(tid);
            }
            else
            {
                // 建立通信子线程
                pthread_create(&tid, NULL, comFun, info);
                pthread_detach(tid);
            }
        }
    }
}

void *acceptFun(void *arg)
{
    // 建立新链接
    sockInfo *info = (sockInfo *)arg;
    sockaddr_in caddr;
    socklen_t clen = sizeof(caddr);
    int cfd = accept(info->fd, (sockaddr *)&caddr, &clen);
    int flag = fcntl(cfd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(cfd, F_SETFL, flag);
    pthread_mutex_lock(&mapMutex);
    _map[cfd] = caddr;
    pthread_mutex_unlock(&mapMutex);
    // 并且将新的通信文件描述符，插入到红黑树
    epoll_event ev;
    ev.data.fd = cfd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(info->epfd, EPOLL_CTL_ADD, cfd, &ev);
    delete info;
    return NULL;
}
void *comFun(void *arg)
{
    // 处理通信过程
    cout << "tid:" << pthread_self() << endl;
    sockInfo *info = (sockInfo *)arg;
    char buff[5];
    char temp[1024];
    bzero(temp, sizeof(temp));
    while (true)
    {
        memset(buff, 0, sizeof(buff));
        int len = recv(info->fd, buff, sizeof(buff) - 1, 0);
        if (len == -1)
        {
            // 接收失败
            if (errno == EAGAIN)
            {
                cout << "数据接收完成...." << endl;
                send(info->fd, temp, strlen(temp) + 1, 0);
                break;
            }
            else
            {
                perror("recive");
                return NULL;
            }
        }
        else if (len == 0)
        {
            // 客户端断开连接
            epoll_ctl(info->epfd, EPOLL_CTL_DEL, info->fd, NULL);
            close(info->fd);
            pthread_mutex_lock(&mapMutex);
            cout << "IP: " << inet_ntoa(_map[info->fd].sin_addr) << "  断开连接...." << endl;
            _map.erase(info->fd);
            pthread_mutex_unlock(&mapMutex);
            break;
        }
        else
        {
            pthread_mutex_lock(&mapMutex);
            cout << "From IP: " << inet_ntoa(_map[info->fd].sin_addr) << "\tPort: " << ntohs(_map[info->fd].sin_port)
                 << "\trecive: " << buff << endl;
            pthread_mutex_unlock(&mapMutex);
            for (int i = 0; i < len; ++i)
            {
                buff[i] = toupper(buff[i]);
            }
            strcat(temp, buff);
        }
    }
    delete info;
    return NULL;
}